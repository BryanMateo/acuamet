#include "gpio_config.h"
bool gpio_inicializado = false;
// Interrupciones

void IRAM_ATTR pulso_interrupt(void *arg)
{
    if (gpio_get_level(pin_ultrasonico_cisterna_echo))
    {
        // tiempo de inicio
        tiempo_inicio = esp_timer_get_time();
    }
    else
    {
        // tiempo de fin y calcular duración
        tiempo_fin = esp_timer_get_time();
        duracion_pulso = tiempo_fin - tiempo_inicio;
    }
}

void gpio_init(void)
{
    gpio_config_t input_config_pin_ultrasonico_cisterna_echo = {
        .pin_bit_mask = (1ULL << pin_ultrasonico_cisterna_echo),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE // Interrupción en ambos flancos
    };

    gpio_config(&input_config_pin_ultrasonico_cisterna_echo);

    /* Interrupciones */
    gpio_install_isr_service(0); // instala servicio de interrupciones de pines
    gpio_isr_handler_add(pin_ultrasonico_cisterna_echo, pulso_interrupt, NULL);

    // pines flujometros

    // Inicializar contador de pulsos por hardware para flujometro en el pin designado
    set_pin_pcnt(pcnt_unit_flujo_1, pin_flujometro_1); // cambiar a luego de que se haga la configuracion mas a delante
    set_pin_pcnt(pcnt_unit_flujo_2, pin_flujometro_2);
    set_pin_pcnt(pcnt_unit_flujo_3, pin_flujometro_3);
    set_pin_pcnt(pcnt_unit_flujo_4, pin_flujometro_4);

    /* Sensor presion */
    set_pin_presion(pin_sensor_presion);

    /* ENTRADAS */
    gpio_set_direction(configPin, GPIO_MODE_INPUT); // Pin config mode
    gpio_set_pull_mode(configPin, GPIO_PULLUP_ONLY);

    /* SALIDAS */
    gpio_set_direction(pin_ultrasonico_cisterna_trig, GPIO_MODE_OUTPUT); // pin trigger sensor nivel cisterna ultrasonico

    gpio_inicializado = true;
}