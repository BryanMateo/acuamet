#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H
#include "../main.h"

extern bool gpio_inicializado;
// Entradas
#define configPin 13                     // Pin para subir modo configuracion
#define pin_ultrasonico_cisterna_echo 18 // Pin tx para sensor ultrasonico nivel cisterna
#define pin_ultrasonico_cisterna_trig 19 // Pin rx para sensor ultrasonico nivel cisterna
#define pin_sensor_presion ADC_CHANNEL_0 // Pin Sensor VP

#define pin_flujometro_1 4
#define pin_flujometro_2 14
#define pin_flujometro_3 13
#define pin_flujometro_4 25

#define pin_valvula_1 26
#define pin_valvula_2 27
#define pin_valvula_3 23
#define pin_valvula_4 22

// Salidas
#define PIN_LED 2

// Funciones
void gpio_init();

#endif