#include "contador_pulsos.h"

void set_pin_pcnt(pcnt_unit_t unit, gpio_num_t pin) // funcion para declarar el contador y el pin del flujometro
{
    pcnt_config_t pcnt_config = {
        .pulse_gpio_num = pin,
        .ctrl_gpio_num = PCNT_PIN_NOT_USED,
        .channel = PCNT_CHANNEL_0,
        .unit = unit,
        .pos_mode = PCNT_COUNT_INC,
        .neg_mode = PCNT_COUNT_DIS,
        .lctrl_mode = PCNT_MODE_KEEP,
        .hctrl_mode = PCNT_MODE_KEEP,
        .counter_h_lim = 23000,
        .counter_l_lim = 0,
    };

    pcnt_unit_config(&pcnt_config);
    pcnt_counter_pause(unit);
    pcnt_counter_clear(unit);
    pcnt_counter_resume(unit);
}
