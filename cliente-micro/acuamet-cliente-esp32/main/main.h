#ifndef MAIN_H
#define MAIN_H

// Definición de pines
#define PIN_LED 2

// Definición de parámetros generales
#define TIEMPO_ESPERA 1000 // Tiempo en milisegundos
#define UMBRAL_SENSOR 500  // Valor umbral para el sensor

// wifi
// #define WIFI_SSID "Nexxt"
// #define WIFI_PASSWORD "ab123456cd"
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

// mqtt




static const char *TAG = "cliente_acuamet";
extern char mac_end[5];
extern uint8_t mac[6];

#endif