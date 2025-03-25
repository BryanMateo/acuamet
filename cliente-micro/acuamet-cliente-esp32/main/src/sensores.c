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
}

// sensor de presion ADC

adc_oneshot_unit_handle_t sensor_presion_handle; // adc sin calibrar
adc_cali_handle_t sensor_presion_cali_handle;    // adc calibrado

void set_pin_presion(adc_channel_t ADC_sensor_presion) // Funcion para establecer el pin del sensor de presion
{
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT};

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1};
    adc_oneshot_new_unit(&init_config, &sensor_presion_handle);

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT, // 12 bits por defecto
        .atten = ADC_ATTEN_DB_12          // Atenuación para rango de 0-3.3V
    };
    adc_oneshot_config_channel(sensor_presion_handle, ADC_sensor_presion, &config);

    if (adc_cali_create_scheme_line_fitting(&cali_config, &sensor_presion_cali_handle) == ESP_OK)
    {
        printf("ADC Calibración habilitada.\n");
    }
    else
    {
        printf("ADC sin calibración.\n");
        sensor_presion_cali_handle = NULL;
    }
}

float read_pin_presion(void)
{
    int adc_raw = 0;
    int voltage_mv = 0;
    float psi = 0;

    // adc_oneshot_read(sensor_presion_handle, pin_sensor_presion, &adc_raw);
    // float presion = (adc_raw / 4095.0) * 3.3;
    // printf("Lectura ADC: %d, Voltaje: %.2fV\n", adc_raw, presion);

    adc_oneshot_read(sensor_presion_handle, pin_sensor_presion, &adc_raw);
    adc_cali_raw_to_voltage(sensor_presion_cali_handle, adc_raw, &voltage_mv);
   // printf("Lectura ADC: %i, Voltaje calibrado: %imV\n", adc_raw, voltage_mv);

    if (voltage_mv > 367)
    {
        psi = (float)(voltage_mv - 367) * 0.0511;
    }
    else
        psi = 0;

    return psi;
}