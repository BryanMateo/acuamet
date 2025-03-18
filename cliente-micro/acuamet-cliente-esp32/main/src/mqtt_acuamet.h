#ifndef MQTT_ACUAMET_H
#define MQTT_ACUAMET_H
#include "../main.h"

#define CONFIG_BROKER_URL "mqtt://test.mosquitto.org"

extern bool mqtt_connected;

void mqtt5_app_start();

#endif