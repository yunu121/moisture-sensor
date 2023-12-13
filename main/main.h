
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "led.h"
#include "server.h"
#include "sensor.h"
#include "user_defined.h"


#define S_TO_HOURS_1 3600

/** Handles HTTP server events.
    @param  void                *arg the pointer to an argument
    @param  esp_event_base_t    event_base the base of the event
    @param  int32_t             event_id the id of the event
    @param  void                *event_data the pointer to the event data
    @return None
*/
static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

/** Connects to the Wi-Fi.
    @return None
*/
void connect_wifi(void);

/** Sends webpage.
    @param  httpd_req_t *req the pointer to the request
    @return esp_err_t the error id.
*/
esp_err_t send_web_page(httpd_req_t *req);

/** Gets the request handler.
    @param  httpd_req_t *req the pointer to the request
    @return esp_err_t the error id.
*/
esp_err_t get_req_handler(httpd_req_t *req);

/** Sends webpage.
    @return httpd_handle_t the http server handle.
*/
httpd_handle_t setup_server(void);

/** Logs values using ESP_LOGI.
    @return None
*/
void log_values(void);

/** Checks timers to see when plants were last watered.
    @return None
*/
void check_timers(void);

#endif