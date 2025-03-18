#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "src/fsm.h"
#include "src/wifi_acuamet.h"
#include "src/mqtt_acuamet.h"
#include "src/gpio_config.h"

// Librerias esp
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_mac.h"
#include "cJSON.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "mqtt_client.h"

extern const char *TAG;


#endif