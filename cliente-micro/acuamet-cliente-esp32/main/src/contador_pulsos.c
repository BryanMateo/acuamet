#include "contador_pulsos.h"

volatile uint32_t pulsos_flujo_1 = 0;

// pcnt_unit_handle_t pcnt_unit_1 = NULL;

// void setup_pcnt(pcnt_unit_handle_t pcnt_unit, gpio_num_t SENSOR_PIN)
// {
//     // Configuración del contador de pulsos
//     pcnt_unit_config_t unit_config = {
//         .high_limit = 32767,
//         .low_limit = 0,
//         .flags.accum_count = true,
//     };
//     ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

//     // Configurar el canal de entrada de pulsos
//     pcnt_chan_config_t chan_config = {
//         .edge_gpio_num = SENSOR_PIN, // Pin de entrada del sensor
//         .level_gpio_num = -1,
//         .flags.invert_edge_input = false,
//     };
//     pcnt_channel_handle_t pcnt_chan = NULL;
//     ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config, &pcnt_chan));
//     ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_LEVEL_ACTION_KEEP));
//     ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
//     ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
//     ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

//     ESP_LOGI(TAG, "PCNT configurado en el pin %i", SENSOR_PIN);
// }

// Interrupciones
void IRAM_ATTR pulsos_flujo_1_interrupt(void *arg) // funcion que maneja la interrupcion del flujometro 1
{
    pulsos_flujo_1++;
}

void set_pin_pcnt(pcnt_unit_t unit, gpio_num_t pin) //funcion para declarar el contador y el pin del flujometro
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
