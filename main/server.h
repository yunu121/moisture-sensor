/** @file   server.h
    @author Yunu Cho
    @date   8 December 2023
    @brief  Dependencies and macros for server implementation.
    @note   There aren't any method declarations for this module,
            only includes and defines.
*/

#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <esp_http_server.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netdb.h>

#define HTTP_QUERY_MAX_LEN 64
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

#endif