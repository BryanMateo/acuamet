#include "sensores.h"

// flujometros
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

// sensor ultrasonico nivel cisterna
volatile uint64_t tiempo_inicio = 0;
volatile uint64_t tiempo_fin = 0;
volatile uint64_t duracion_pulso = 0;

int nivel_cisterna_distance()
{
    int distance = 0;

    for (size_t i = 0; i < promedio_nivel_cisterna; i++)
    {
        gpio_set_level(pin_ultrasonico_cisterna_trig, 1);
        esp_rom_delay_us(10); // delay de 10 microsegundos
        gpio_set_level(pin_ultrasonico_cisterna_trig, 0);

        while (1)
        {
            if (duracion_pulso > 0)
            {
                distance += duracion_pulso / 58;
                duracion_pulso = 0; // Reiniciar medición
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(1));
        }
        vTaskDelay(pdMS_TO_TICKS(55)); // tiempo espera para siguiente lectura

    }
    distance = distance / promedio_nivel_cisterna;
    return distance;

    // gpio_set_level(pin_ultrasonico_cisterna_trig, 1);
    // esp_rom_delay_us(10); // delay de 10 microsegundos
    // gpio_set_level(pin_ultrasonico_cisterna_trig, 0);

    // uint8_t timeout = 0;
    // while (1)
    // {
    //     if (duracion_pulso > 0)
    //     {
    //         // printf("Duración del pulso: %llu us\n", duracion_pulso);
    //         distance = duracion_pulso / 58;
    //         duracion_pulso = 0; // Reiniciar medición

    //         if (distance < 550)
    //             return distance;
    //         else
    //             return SENS_ERR_RANGE; // lectura fuera de rango
    //     }
    //     else if (timeout > 10)
    //     {
    //         return SENS_ERR_TIMEOUT; // tiempo de espera agotado
    //     }

    //     timeout++;
    //     vTaskDelay(pdMS_TO_TICKS(100));
    // }
}