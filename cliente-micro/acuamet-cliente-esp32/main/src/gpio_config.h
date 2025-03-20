#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H
#include "../main.h"


// Entradas
#define configPin 13 // Pin para subir modo configuracion
#define flujometro_1 4

extern volatile uint32_t pulsos_flujo_1;

// Salidas
#define PIN_LED 2

// Funciones
void gpio_init();

#endif