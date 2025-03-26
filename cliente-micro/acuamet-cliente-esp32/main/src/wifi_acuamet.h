#ifndef WIFI_ACUAMET_H
#define WIFI_ACUAMET_H
#include "../main.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define STORAGE_NAMESPACE "wifi_creds"
#define SSID_KEY "ssid"
#define PASSWORD_KEY "password"
#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern char mac_end[5];
extern char mac_str[13];
extern uint8_t mac[6];
extern char ssid[32], password[32];
extern bool wifi_connected;

bool nvs_init();
void config();
void wifi_init_sta();
void obtener_mac();

#endif