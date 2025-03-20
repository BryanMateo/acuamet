#include "gpio_config.h"

volatile uint32_t pulsos_flujo_1 = 0;

// Interrupciones
// void IRAM_ATTR pulsos_flujo_1_interrupt(void *arg) // funcion que maneja la interrupcion del flujometro 1
// {
//     pulsos_flujo_1++;
// }

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

    void gpio_init(void)
    {
        // pines flujometros
        set_pin_pcnt(PCNT_UNIT_0, flujometro_1); //cambiar a luego de que se haga la configuracion mas a delante

        /*
                gpio_config_t flujometro_1 = {
                    .pin_bit_mask = (1ULL << flujo_1), // pin flujometro 1
                    .mode = GPIO_MODE_INPUT,
                    .pull_up_en = GPIO_PULLUP_ENABLE,
                    .pull_down_en = GPIO_PULLDOWN_DISABLE,
                    .intr_type = GPIO_INTR_POSEDGE // Detectar flanco de subida
                };
                gpio_config(&flujometro_1);

                gpio_install_isr_service(0); // servicio que maneja interrupciones
                gpio_isr_handler_add(flujo_1, pulsos_flujo_1_interrupt, NULL);
        */

        // entradas
        gpio_set_direction(configPin, GPIO_MODE_INPUT); // Pin config mode
        gpio_set_pull_mode(configPin, GPIO_PULLUP_ONLY);

        // salidas
    }