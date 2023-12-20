/** @file   main.h
    @author Yunu Cho
    @date   14 December 2023
    @brief  Declarations of methods for the moisture sensor. 
*/

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"

#include <esp_http_server.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netdb.h>

#include "led.h"
#include "sensor.h"
#include "user_config.h"

#ifdef PUMP_ENABLED
    #include "pump.h"
#endif

#define HTTP_QUERY_MAX_LEN 64
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#define S_TO_HOURS_1 3600

/** Handles HTTP server events.
    @param  arg         the pointer to an argument
    @param  event_base  the base of the event
    @param  event_id    the id of the event
    @param  event_data  the pointer to the event data
    @return None
*/
static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/** Connects to the Wi-Fi.
    @return None
*/
void connect_wifi(void);

/** Sends webpage.
    @param  req the pointer to the HTTP request
    @return The error ID.
*/
esp_err_t send_web_page(httpd_req_t *req);

/** Gets the request handler.
    @param  req the pointer to the HTTP request
    @return The error ID.
*/
esp_err_t get_req_handler(httpd_req_t *req);

/** Sends webpage.
    @return The HTTP server handle.
*/
httpd_handle_t setup_server(void);

/** Calculates moisture value and relative moisture (relative to optimal moisture).
    This is based on the upper and lower bounds defined by the user.
    @param  sensor            the sensor number
    @param  raw_value         the raw input value 
    @param  moisture          the pointer to the moisture value
    @param  relative_moisture the pointer to the relative moisture value
    @return None
*/
void calculate_values(int sensor, float raw_value, float *moisture, float *relative_moisture);

/** Logs values using ESP_LOGI.
    The values logged include:
     -- The raw input value, which can be used for calibration purposes;
     -- The moisture value, which was calculated by the calculate_values() method;
     -- The relative moisture value, which was also calculated by the above method.
    @return None
*/
void log_values(void);

/** Checks timers to see when plants were last watered.
    @return None
*/
void check_timers(void);

#endif