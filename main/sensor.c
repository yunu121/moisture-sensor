/** @file   sensor.c
    @author Yunu Cho
    @date   7 December 2023
    @brief  Implementation of a moisture sensor. 
            Can connect to Wi-Fi and displays moisture 
            percentage as a HTML page by accessing 
            <IP-Address>/sensor.
*/

#include <stdio.h>
#include "driver/gpio.h"
#include "led.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "server.h"

/** USER SET VALUES START */
//  Change these values based on your setup.
#define SENSOR_PIN GPIO_NUM_36
#define LED_STRIP_PIN GPIO_NUM_14

static const char *TAG = "Moisture Sensor";
int optimal_moisture = 60;

static int upper_bound = 4095;
static int lower_bound = 2095;
/** USER SET VALUES END */

static uint8_t s_led_state = 0;
static led_strip_handle_t led_strip;
led_strip_config_t strip_config;
led_strip_rmt_config_t rmt_config;

adc_oneshot_unit_handle_t sensor;
adc_oneshot_unit_init_cfg_t adc_config;
adc_oneshot_chan_cfg_t config;

static int s_retry_num = 0;
static EventGroupHandle_t s_wifi_event_group;
esp_err_t get_req_handler(httpd_req_t *req);

int moisture;

httpd_uri_t uri_get = {
    .uri = "/sensor",
    .method = HTTP_GET,
    .handler = get_req_handler,
    .user_ctx = NULL
};

/** Configures the sensor.
    @param gpio_num_t adc_pin the pin that is connected to the sensor
    @param adc_channel_t channel the channel of the used pin
    @param adc_oneshot_unit_init_cfg_t *adc_config pointer to the initial ADC configuration
    @param adc_oneshot_chan_cfg_t *config pointer to ADC configuration
    @param adc_oneshot_unit_handle_t *sensor the pointer to the handle for the sensor
    @return None
*/
void configure_sensor(gpio_num_t adc_pin, adc_channel_t channel, adc_oneshot_unit_init_cfg_t *adc_config, adc_oneshot_chan_cfg_t *config, adc_oneshot_unit_handle_t *sensor)
{
    adc_config -> unit_id = ADC_UNIT_1;
    ESP_ERROR_CHECK(adc_oneshot_new_unit(adc_config, sensor));
    
    config -> bitwidth = ADC_BITWIDTH_DEFAULT;
    config -> atten = ADC_ATTEN_DB_12;
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*sensor, channel, config));
}

/** Reads from the sensor.
    @param adc_oneshot_unit_handle_t *sensor the pointer to the handle for the sensor
    @param adc_channel_t channel the pin that is connected to the sensor
    @param int *out the pointer to the moisture integer
    @return None
*/
void read_sensor(adc_oneshot_unit_handle_t sensor, adc_channel_t channel, int *out)
{
    adc_oneshot_read(sensor, channel, out);
}

/** Handles events.
    @param void *arg the pointer to an argument
    @param esp_event_base_t event_base the base of the event
    @param int32_t event_id the id of the event
    @param void *event_data the pointer to the event data
    @return None
*/
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
        ESP_LOGI(TAG, "Server running @ " IPSTR, IP2STR(&event->ip_info.ip));
        
        s_retry_num = 0;
        
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

/** Connects to the Wi-Fi.
    @return None
*/
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
        ESP_LOGI(TAG, "Connected to AP -> SSID: %s, Password: %s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect -> SSID: %s, Password: %s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
    } else {
        ESP_LOGE(TAG, "Unexpected Error");
    }
    
    vEventGroupDelete(s_wifi_event_group);
}

/** Sends webpage.
    @param httpd_req_t *req the pointer to the request
    @return esp_err_t the error id.
*/
esp_err_t send_web_page(httpd_req_t *req)
{
    char moisture_str[5];
    char optimal_str[5];
    int response;

    snprintf(moisture_str, sizeof(moisture_str), "%d%%", moisture);
    snprintf(optimal_str, sizeof(optimal_str), "%d%%", optimal_moisture);

    const char resp[] = "<h1 style='font-family:verdana'>Plant 1</h1>"
                        "<p style='font-family:verdana'>Moisture: %s</p>"
                        "<p style='font-family:verdana'>Optimal Moisture: %s</p>";
    
    char html_response[sizeof(resp) + sizeof(moisture_str) + sizeof(optimal_str)];

    snprintf(html_response, sizeof(html_response), resp, moisture_str, optimal_str);

    response = httpd_resp_send(req, html_response, HTTPD_RESP_USE_STRLEN);
    
    return response;
}

/** Gets the request handler.
    @param httpd_req_t *req the pointer to the request
    @return esp_err_t the error id.
*/
esp_err_t get_req_handler(httpd_req_t *req)
{
    return send_web_page(req);
}

/** Sends webpage.
    @return httpd_handle_t the http server handle.
*/
httpd_handle_t setup_server(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_get);
    }

    return server;
}

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

    configure_sensor(SENSOR_PIN, ADC_CHANNEL_0, &adc_config, &config, &sensor);
    configure_led(LED_STRIP_PIN, &strip_config, &rmt_config, &led_strip);
    
    while (1) {
        read_sensor(sensor, ADC_CHANNEL_0, &moisture);
        ESP_LOGI(TAG, "Raw Value = %d", moisture);
        moisture = ((moisture - lower_bound) * (0 - 100)) / (upper_bound - lower_bound) + 100;
        ESP_LOGI(TAG, "Moisture = %d%%", moisture);

        if (moisture < 25) {
            blink_led(1, s_led_state, led_strip);
        } else if (moisture > optimal_moisture) {
            blink_led(4, s_led_state, led_strip);
        } else {   
            int index = moisture / 25;
            toggle_led(index, led_strip);
        }

        s_led_state = !s_led_state;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
