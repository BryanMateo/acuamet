#include "gpio_config.h"

volatile uint32_t pulsos_flujo_1 = 0;

// Interrupciones
void IRAM_ATTR pulsos_flujo_1_interrupt(void *arg) // funcion que maneja la interrupcion del flujometro 1
{
    pulsos_flujo_1++;
}

void gpio_init(void)
{
    // pines flujometros
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

    // entradas
    gpio_set_direction(configPin, GPIO_MODE_INPUT); // Pin config mode
    gpio_set_pull_mode(configPin, GPIO_PULLUP_ONLY);

    // salidas
}