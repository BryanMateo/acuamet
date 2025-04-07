#ifndef GPIO_CONFIG_H
#define GPIO_CONFIG_H
#include "../main.h"

extern bool gpio_inicializado;
// Entradas
#define config_pin 17 // (GPIO 17) Pin para subir modo configuracion
#define boton_0 39    // (GPIO 39) Pin boton 0 - hacer pullup en pcb
#define boton_1 34    // (GPIO 34) Pin boton 1 - hacer pullup en pcb
#define boton_2 35    // (GPIO 35) Pin boton 2 - hacer pullup en pcb

#define pin_sensor_presion ADC_CHANNEL_0 // (GPIO 36) Pin Sensor VP - capacitor de desacople 0.1uf

#define pin_flujometro_1 4  // (GPIO 4) Pin pulse counter para flujometro
#define pin_flujometro_2 14 // (GPIO 14) Pin pulse counter para flujometro
#define pin_flujometro_3 13 // (GPIO 13) Pin pulse counter para flujometro
#define pin_flujometro_4 25 // (GPIO 25) Pin pulse counter para flujometro

// Salidas
#define pin_pwr_on_led 2    // (GPIO 2) Pin para indicar PWR
#define pin_signal_led_r 15 // (GPIO 15) Pin señal Wifi R
#define pin_signal_led_g 16 // (GPIO 16) Pin señal Wifi G
#define pin_signal_led_b 5  // (GPIO 5) Pin señal Wifi B

#define pin_valvula_1 26 // (GPIO 26) Pin PWM para servovalvula
#define pin_valvula_2 27 // (GPIO 27) Pin PWM para servovalvula
#define pin_valvula_3 23 // (GPIO 23) Pin PWM para servovalvula
#define pin_valvula_4 12 // (GPIO 12) Pin PWM para servovalvula

#define pin_relay_bomba 32 // (GPIO 32) Pin del relay de encendido/apagado bomba

#define pin_lcd_scl 22 // (GPIO 22) Pin I2C para LCD
#define pin_lcd_sda 21 // (GPIO 21) Pin I2C para LCD

// Sensor cisterna Ultrasonico
#define pin_ultrasonico_cisterna_echo 18 // (GPIO 18) Pin echo para sensor ultrasonico nivel cisterna
#define pin_ultrasonico_cisterna_trig 19 // (GPIO 19) Pin trig para sensor ultrasonico nivel cisterna

// Funciones
void gpio_init(); // Funcion para inicializar los GPIO como I/O

#endif