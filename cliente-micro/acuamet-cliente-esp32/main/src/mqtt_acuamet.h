#ifndef MQTT_ACUAMET_H
#define MQTT_ACUAMET_H
#include "../main.h"
// #include "esp_err.h"

#define CONFIG_BROKER_URL "mqtt://192.168.1.21"

void mqtt5_app_start();
void create_topics();

esp_err_t pub_info_sensores_mqtt();

#endif