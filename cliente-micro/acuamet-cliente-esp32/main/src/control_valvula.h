#ifndef CONTROL_VALVULA_H
#define CONTROL_VALVULA_H
#include "../main.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE

#define LEDC_DUTY_RES LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY (4096)                // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_FREQUENCY (50)             // Frequency in Hertz. Set frequency at 50 HZ

#define LEDC_VALVULA_1 LEDC_CHANNEL_0
#define LEDC_VALVULA_2 LEDC_CHANNEL_1
#define LEDC_VALVULA_3 LEDC_CHANNEL_2
#define LEDC_VALVULA_4 LEDC_CHANNEL_3

#define VALV_ABIERTA 1
#define VALV_CERRADA 0

void valvulas_init();
void control_valvula(ledc_channel_t, bool);

#endif