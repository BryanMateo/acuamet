#ifndef WIFI_ACUAMET_H
#define WIFI_ACUAMET_H

#define configPin 13 // Pin para subir modo configuracion

#define STORAGE_NAMESPACE "wifi_creds"
#define SSID_KEY "ssid"
#define PASSWORD_KEY "password"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void wifi_init_sta();

#endif