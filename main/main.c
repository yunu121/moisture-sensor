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
static uint8_t s_led_state = 0;
static led_strip_handle_t led_strip;
led_strip_config_t strip_config;
led_strip_rmt_config_t rmt_config;
/*  LED Strip Related Variable Initialisation End  */

/*  Sensor Related Variable Initialisation Start  */
static const char *SENSOR1 = "Moisture Sensor 1";
static const char *SENSOR2 = "Moisture Sensor 2";
static const char *SENSOR3 = "Moisture Sensor 3";

adc_oneshot_unit_handle_t sensor;
adc_oneshot_unit_init_cfg_t adc_config;
adc_oneshot_chan_cfg_t config;
/*  Sensor Related Variable Initialisation End  */

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

int moisture;
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
    char moisture_str[5];
    char optimal_str[5];
    char timer_str_1[5];
    char timer_str_2[5];
    char timer_str_3[5];
    esp_err_t response;

    snprintf(moisture_str, sizeof(moisture_str), "%d%%", moisture);
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

    char html_response[sizeof(resp) + sizeof(moisture_str) + sizeof(optimal_str) + sizeof(timer_str_1) + sizeof(timer_str_2) + sizeof(timer_str_3)];

    snprintf(html_response, sizeof(html_response), resp, optimal_str, moisture_str, timer_str_1, moisture_str, timer_str_2, moisture_str, timer_str_3);

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
void log_values(void) 
{
        ESP_LOGI(SENSOR1, "Raw Value = %d", moisture);
        moisture = ((moisture - LOWER_BOUND) * (0 - OPTIMAL_MOISTURE)) / (UPPER_BOUND - LOWER_BOUND) + OPTIMAL_MOISTURE;
        ESP_LOGI(SENSOR1, "Moisture = %d%%", moisture); 
}

void check_timers(void)
{
    for (int i = 0; i < 3; i++) {
        if (timers[i] >= S_TO_HOURS_1) {
            //water
            timers[i] = 0;
        }
        timers[i]++;
    }
}
/*  Miscellaneous Method Definitions End  */

/** Main function  */
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

    configure_sensor(SENSOR_PIN_1, SENSOR_CHANNEL_1, &adc_config, &config, &sensor);
    configure_led(LED_STRIP_PIN, &strip_config, &rmt_config, &led_strip);
    
    while (1) {
        read_sensor(sensor, ADC_CHANNEL_0, &moisture);
        log_values();
        
        if (moisture < 25) {
            blink_led(1, s_led_state, led_strip);
        } else if (moisture > OPTIMAL_MOISTURE) {
            blink_led(4, s_led_state, led_strip);
        } else {   
            int index = moisture / 25;
            toggle_led(index, led_strip);
        }

        s_led_state = !s_led_state;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        check_timers();
    }
}
