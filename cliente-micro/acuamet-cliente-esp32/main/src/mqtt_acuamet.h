#ifndef MQTT_ACUAMET_H
#define MQTT_ACUAMET_H
#include "../main.h"

#define CONFIG_BROKER_URL "mqtt://192.168.1.21"

void mqtt5_app_start();
void create_topics();

esp_err_t pub_info_sensores_mqtt();
esp_err_t pub_pedir_token_sesion_mqtt(bool req);

extern char *token_sesion;

#endif