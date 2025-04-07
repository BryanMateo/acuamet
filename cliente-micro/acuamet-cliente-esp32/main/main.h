#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Librerias esp
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs.h"
#include "nvs_flash.h"
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
#include "driver/pcnt.h"
#include "esp_rom_sys.h"
#include "esp_timer.h"
#include "driver/timer.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "driver/i2c.h"
#include "driver/ledc.h"

#include "src/fsm.h"
#include "src/wifi_acuamet.h"
#include "src/mqtt_acuamet.h"
#include "src/gpio_config.h"
#include "src/sensores.h"
#include "src/lcd.h"
#include "src/control_valvula.h"

#define delay_lectura_flujometros 1000
#define delay_lectura_cisterna 1000
#define delay_salidas_task 1000
extern const char *TAG;

struct SENSORES
{
    int nivel_cisterna;
    float presion;
    float flujo_apt1;
    float flujo_apt2;
    float flujo_apt3;
    float flujo_apt4;
};

struct SALIDA
{
    bool bomba;
    bool valvula_apt1;
    bool valvula_apt2;
    bool valvula_apt3;
    bool valvula_apt4;
};

extern struct SENSORES sensores;
extern struct SALIDA salida;

#endif