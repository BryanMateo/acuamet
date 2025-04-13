#include "control_valvula.h"

void ledc_init(gpio_num_t pin_valvula, ledc_channel_t channel)
{
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = channel,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = pin_valvula,
        .duty = 410, // Set duty to 5%
        .hpoint = 0};
    ledc_channel_config(&ledc_channel);
}

void valvulas_init(void)
{
    // Inicializa los canales de PWM Ledc para las servovalvulas
    ledc_init(pin_valvula_1, LEDC_VALVULA_1);
    ledc_init(pin_valvula_2, LEDC_VALVULA_2);
    ledc_init(pin_valvula_3, LEDC_VALVULA_3);
    ledc_init(pin_valvula_4, LEDC_VALVULA_4);
}

void control_valvula(ledc_channel_t LEDC_VALVULA, bool pos)
{
    if (pos == VALV_ABIERTA)
        ledc_set_duty(LEDC_MODE, LEDC_VALVULA, 605); // 90° aprox

    else
        ledc_set_duty(LEDC_MODE, LEDC_VALVULA, 410); // 0° aprox
        
    ledc_update_duty(LEDC_MODE, LEDC_VALVULA);
}