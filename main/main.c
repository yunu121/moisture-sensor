/** @file   main.c
    @author Yunu Cho
    @date   7 December 2023
    @brief  Implementation of a moisture sensor. 
            Can connect to Wi-Fi and displays moisture 
            percentage as a HTML page by accessing 
            <IP-Address>/sensor.
    @note   This file contains the HTTP server implementations
            and main logic for the Automated Watering System.
*/

#include "main.h"

/*  LED Strip Related Variable Initialisation Start  */
#ifdef SENSOR_1
    static led_strip_handle_t led_strip_1;
    led_strip_config_t strip_config_1;
    led_strip_rmt_config_t rmt_config_1;
    static int s_led_state_1 = 0;
#endif

#ifdef SENSOR_2
    static led_strip_handle_t led_strip_2;
    led_strip_config_t strip_config_2;
    led_strip_rmt_config_t rmt_config_2;
    static int s_led_state_2 = 0;
#endif

#ifdef SENSOR_3
    static led_strip_handle_t led_strip_3;
    led_strip_config_t strip_config_3;
    led_strip_rmt_config_t rmt_config_3;
    static int s_led_state_3 = 0;
#endif
/*  LED Strip Related Variable Initialisation End  */

/*  Sensor Related Variable Initialisation Start  */
#ifdef SENSOR_1
    static const char *SENSOR1 = "Moisture Sensor 1";
    adc_oneshot_unit_handle_t sensor_config;
    adc_oneshot_unit_init_cfg_t adc_config;
    adc_oneshot_chan_cfg_t config_1;
    int raw_value_1;
    float moisture_1;
    float relative_moisture_1;
#endif

#ifdef SENSOR_2
    static const char *SENSOR2 = "Moisture Sensor 2";
    adc_oneshot_chan_cfg_t config_2;
    int raw_value_2;
    float moisture_2;
    float relative_moisture_2;
#endif

#ifdef SENSOR_3
    static const char *SENSOR3 = "Moisture Sensor 3";
    adc_oneshot_chan_cfg_t config_3;
    int raw_value_3;
    float moisture_3;
    float relative_moisture_3;
#endif
/*  Sensor Related Variable Initialisation End  */

/*  Pump Related Variable Initialisation Start  */
#ifdef PUMP_ENABLED
    // Initialise pump related variables here
    #ifdef PUMP_2

    #endif

    #ifdef PUMP_3

    #endif
#endif
/*  Pump Related Variable Initialisation End  */

/*  Server Related Variable Initialisation Start  */
static const char *SERVER = "Server";

static int s_retry_num = 0;
static EventGroupHandle_t s_wifi_event_group;
esp_err_t get_req_handler(httpd_req_t *req);

httpd_uri_t uri_get = {
    .uri = "/sensor",
    .method = HTTP_GET,
    .handler = get_req_handler,
    .user_ctx = NULL
};
/*  Server Related Variable Declarations End  */

static float timers[3] = {0, 0, 0};


/*  Server Related Method Definitions Start  */
static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
       
        if (s_retry_num < CONFIG_ESP_MAX_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }

    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;

        ESP_LOGI(SERVER, "Server running @ " IPSTR, IP2STR(&event->ip_info.ip));
        
        s_retry_num = 0; // Resets the number of retries
        
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void connect_wifi(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
   
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_ESP_WIFI_SSID,
            .password = CONFIG_ESP_WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(SERVER, "Connected to AP -> SSID: %s, Password: %s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(SERVER, "Failed to connect -> SSID: %s, Password: %s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
    } else {
        ESP_LOGE(SERVER, "Unexpected Error");
    }
    
    vEventGroupDelete(s_wifi_event_group);
}

esp_err_t send_web_page(httpd_req_t *req)
{
    char moisture_str_1[6];
    char moisture_str_2[6];
    char moisture_str_3[6];
    char optimal_str[6];
    
    char timer_str_1[5];
    char timer_str_2[5];
    char timer_str_3[5];

    esp_err_t response;

    snprintf(moisture_str_1, sizeof(moisture_str_1), "%.1f%%", moisture_1);
    #ifdef SENSOR_2
        snprintf(moisture_str_2, sizeof(moisture_str_2), "%.1f%%", moisture_2);
    #else
        snprintf(moisture_str_2, sizeof(moisture_str_2), "%.1f%%", 0.0);
    #endif

    #ifdef SENSOR_3
        snprintf(moisture_str_3, sizeof(moisture_str_3), "%.1f%%", moisture_3);
    #else
        snprintf(moisture_str_2, sizeof(moisture_str_3), "%.1f%%", 0.0);
    #endif

    snprintf(optimal_str, sizeof(optimal_str), "%d%%", OPTIMAL_MOISTURE);
    snprintf(timer_str_1, sizeof(timer_str_1), "%.2f", timers[0] / S_TO_HOURS_1);
    snprintf(timer_str_2, sizeof(timer_str_2), "%.2f", timers[1] / S_TO_HOURS_1);
    snprintf(timer_str_3, sizeof(timer_str_3), "%.2f", timers[2] / S_TO_HOURS_1);

    const char resp[] = "<style>"
                        "body {padding: 25px; background-color: black; color: white; font-family: helvetica;}"
                        "h1, h2, h3 {font-family: helvetica;}"
                        "p {font-family: helvetica;}"
                        ".p1 {font-family: helvetica; font-size: 12px;}"
                        "</style>"

                        "<h1>Plant Moisture Levels</h1>"
                        "<p class='p1'><i>The plants in this planter are all expected to have similar optimal moistures of: %s</i></p>"

                        "<h2>Plant 1</h1>"
                        "<p>Moisture: %s</p>"
                        "<p class='p1'><i>Last watered: %s hrs ago</i></p>"

                        "<h2>Plant 2</h2>"
                        "<p>Moisture: %s</p>"
                        "<p class='p1'><i>Last watered: %s hrs ago</i></p>"

                        "<h2>Plant 3</h2>"
                        "<p>Moisture: %s</p>"
                        "<p class='p1'><i>Last watered: %s hrs ago</i></p>";

    char html_response[sizeof(resp) + (sizeof(moisture_str_1) * 4) + (sizeof(timer_str_1) * 3)];

    snprintf(html_response, sizeof(html_response), resp, optimal_str, moisture_str_1, timer_str_1, moisture_str_2, timer_str_2, moisture_str_3, timer_str_3);

    response = httpd_resp_send(req, html_response, HTTPD_RESP_USE_STRLEN);
    
    return response;
}

esp_err_t get_req_handler(httpd_req_t *req)
{
    return send_web_page(req);
}

httpd_handle_t setup_server(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_get);
    }

    return server;
}
/*  Server Related Method Definitions End  */

/*  Miscellaneous Method Definitions Start  */

void calculate_values(int sensor, float raw_value, float *moisture, float *relative_moisture)
{
    int upper;
    int lower;
    switch (sensor) {
        case 1: upper = UPPER_BOUND_1; lower = LOWER_BOUND_1; break;
        case 2: 
            #ifdef SENSOR_2 
                upper = UPPER_BOUND_2; lower = LOWER_BOUND_2; 
            #endif 
            break;
        case 3: 
            #ifdef SENSOR_3
                upper = UPPER_BOUND_3; lower = LOWER_BOUND_3;
            #endif
            break;
        default: upper = UPPER_BOUND_1; lower = LOWER_BOUND_1;
    }
    *moisture = ((float)(raw_value) - upper) / (lower - upper) * 100;
    *relative_moisture = ((*moisture) / OPTIMAL_MOISTURE) * 100;
}

void log_values(void) 
{
    ESP_LOGI(SENSOR1, "Raw Value = %d", raw_value_1);   
    calculate_values(1, raw_value_1, &moisture_1, &relative_moisture_1); 
    ESP_LOGI(SENSOR1, "Moisture = %.1f%%", moisture_1); 
    ESP_LOGI(SENSOR1, "Relative Moisture = %.1f%%", relative_moisture_1); 
    
    #ifdef SENSOR_2
        ESP_LOGI(SENSOR2, "Raw Value = %d", raw_value_2);   
        calculate_values(2, raw_value_2, &moisture_2, &relative_moisture_2); 
        ESP_LOGI(SENSOR2, "Moisture = %.1f%%", moisture_2); 
        ESP_LOGI(SENSOR2, "Relative Moisture = %.1f%%", relative_moisture_2); 
    #endif

    #ifdef SENSOR_3
        ESP_LOGI(SENSOR3, "Raw Value = %d", raw_value_3);   
        calculate_values(3, raw_value_3, &moisture_3, &relative_moisture_3); 
        ESP_LOGI(SENSOR3, "Moisture = %.1f%%", moisture_3); 
        ESP_LOGI(SENSOR3, "Relative Moisture = %.1f%%", relative_moisture_3); 
    #endif
}

void check_timers(void)
{
    for (int i = 0; i < 3; i++) {
        if (timers[i] >= S_TO_HOURS_1) { // Checks if plants haven't been watered in the last hour
            #ifdef PUMP_ENABLED
                float volume = calculate_volume(SOIL_VOLUME, moisture, OPTIMAL_MOISTURE);
                int seconds = calculate_duration(volume, FLOW_RATE);
               
                drive(gpio, seconds);
               
                #ifndef PUMP_2
                    drive(gpio, seconds);
                #endif
                #ifndef PUMP_3
                    drive(gpio, seconds);
                #endif
                
                timers[i] = 0;
            #endif
        }
        timers[i]++;
    }
}
/*  Miscellaneous Method Definitions End  */

/*  Main function  */
void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    
    ESP_ERROR_CHECK(ret);
    
    connect_wifi();
    setup_server();

    configure_unit(SENSOR_UNIT_1, &adc_config, &sensor_config); // Sensor 1 is enabled by default
    configure_channel(SENSOR_CHANNEL_1, &sensor_config, &config_1);
    configure_led(LED_STRIP_PIN_1, &strip_config_1, &rmt_config_1, &led_strip_1, MAX_LEDS);
   
    #ifdef SENSOR_2
        configure_channel(SENSOR_CHANNEL_2, &sensor_config, &config_2);
        configure_led(LED_STRIP_PIN_2, &strip_config_2, &rmt_config_2, &led_strip_2, MAX_LEDS);
    #endif

    #ifdef SENSOR_3
        configure_channel(SENSOR_CHANNEL_3, &sensor_config, &config_3);
        configure_led(LED_STRIP_PIN_3, &strip_config_3, &rmt_config_3, &led_strip_3, MAX_LEDS);
    #endif

    /** @note Add pump related code here as required  */
    #ifdef PUMP_ENABLED
        
        // Add code specific to pump 1 (assuming it is enabled by default)
        #ifdef PUMP_2
            // Add code specific to pump 2
        #endif
        #ifdef PUMP_3
            // Add code specific to pump 3
        #endif
    #endif
    
    while (1) {
        read_sensor(sensor_config, SENSOR_CHANNEL_1, &raw_value_1);
        drive_led(led_strip_1, &s_led_state_1, relative_moisture_1, moisture_1);
       
        #ifdef SENSOR_2
            read_sensor(sensor_config, SENSOR_CHANNEL_2, &raw_value_2);
            drive_led(led_strip_2, &s_led_state_2, relative_moisture_2, moisture_2);
        #endif

        #ifdef SENSOR_3
            read_sensor(sensor_config, SENSOR_CHANNEL_3, &raw_value_3);
            drive_led(led_strip_3, &s_led_state_3, relative_moisture_3, moisture_3);
        #endif


        log_values();

        /** @note Add pump related code here as required  */
        #ifdef PUMP_ENABLED
            // Add code specific to pump 1 (assuming it is enabled by default)
            
            #ifdef PUMP_2
                // Add code specific to pump 2
            #endif
            
            #ifdef PUMP_3
                // Add code specific to pump 3
            #endif
        #endif
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        check_timers();
    }
}
