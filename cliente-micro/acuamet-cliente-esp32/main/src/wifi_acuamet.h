#ifndef WIFI_ACUAMET_H
#define WIFI_ACUAMET_H
#include "../main.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1
#define WIFI_NAMESPACE "wifi_creds" // nvs name
#define SSID_KEY "ssid"                // nvs ssid
#define PASSWORD_KEY "password"        // nvs wifi password
#define KEY_KEY "key"                  // nvs key

#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern char mac_end[5];
extern char mac_str[13];
extern uint8_t mac[6];
extern char ssid[33], password[33], key[33];
extern char ap_ssid[13];
bool nvs_init();
void config();
void wifi_init_sta();
void obtener_mac();

#endif