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
        .counter_h_lim = 30000,
        .counter_l_lim = 0,
    };

    pcnt_unit_config(&pcnt_config);
    pcnt_counter_pause(unit);
    pcnt_counter_clear(unit);
    pcnt_counter_resume(unit);
}

float calc_galon_flujo(pcnt_unit_t unit) // lee el contador del flujometro y devuelve los galones contados
{
    uint16_t pulsos_flujo = 0;
    pcnt_get_counter_value(unit, (int16_t *)&pulsos_flujo);
    if (pulsos_flujo > 0)
    {
        pcnt_counter_clear(unit);
    }
    float galones_flujo = (float)pulsos_flujo / constante_ppg_flujometro;

    return galones_flujo;
}

void guardar_flujo_flash() // funcion para guardar flujo acumulado en la flash en caso de que se pierda conexion
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(FLUJO_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "No se pudo abrir NVS flujo");
        return;
    }

    uint32_t apt1 = 0;
    uint32_t apt2 = 0;
    uint32_t apt3 = 0;
    uint32_t apt4 = 0;

    nvs_get_u32(nvs_handle, nvs_flujo_apt_1, &apt1);
    nvs_get_u32(nvs_handle, nvs_flujo_apt_2, &apt2);
    nvs_get_u32(nvs_handle, nvs_flujo_apt_3, &apt3);
    nvs_get_u32(nvs_handle, nvs_flujo_apt_4, &apt4);

    float f1, f2, f3, f4;
    memcpy(&f1, &apt1, sizeof(float));
    memcpy(&f2, &apt2, sizeof(float));
    memcpy(&f3, &apt3, sizeof(float));
    memcpy(&f4, &apt4, sizeof(float));

    if (sensores.flujo_apt1 != f1 || sensores.flujo_apt2 != f2 || sensores.flujo_apt3 != f3 || sensores.flujo_apt4 != f4)
    {
        memcpy(&apt1, &sensores.flujo_apt1, sizeof(sensores.flujo_apt1));
        memcpy(&apt2, &sensores.flujo_apt2, sizeof(sensores.flujo_apt2));
        memcpy(&apt3, &sensores.flujo_apt3, sizeof(sensores.flujo_apt3));
        memcpy(&apt4, &sensores.flujo_apt4, sizeof(sensores.flujo_apt4));

        // if (token_sesion != NULL)
        //     nvs_set_str(nvs_handle, nvs_token, token_sesion);

        nvs_set_u32(nvs_handle, nvs_flujo_apt_1, apt1);
        nvs_set_u32(nvs_handle, nvs_flujo_apt_2, apt2);
        nvs_set_u32(nvs_handle, nvs_flujo_apt_3, apt3);
        nvs_set_u32(nvs_handle, nvs_flujo_apt_4, apt4);

        memcpy(&f1, &apt1, sizeof(float));
        memcpy(&f2, &apt2, sizeof(float));
        memcpy(&f3, &apt3, sizeof(float));
        memcpy(&f4, &apt4, sizeof(float));

        nvs_commit(nvs_handle);
        // printf("Flujos Aptos: 1: %.2f, 2: %.2f, 3: %.2f, 4: %.2f\n", f1, f2, f3, f4);
        printf("Valores guardados en NVS \n");
    }
    else
    {
        // printf("Flujos Aptos: 1: %.2f, 2: %.2f, 3: %.2f, 4: %.2f\n", f1, f2, f3, f4);
        printf("Valores de nvs igual a actual, no sobreescrito \n");
    }

    nvs_close(nvs_handle);
}

void obtener_flujo_flash() // obtiene los flujos y token de la sesion anterior
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(FLUJO_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        // ESP_LOGE(TAG, "Obtener flujo No se pudo abrir NVS flujo ");
        switch (err)
        {
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGE(TAG, "not found");
            break;
        case ESP_ERR_NVS_NOT_INITIALIZED:
            ESP_LOGE(TAG, "NOT_INITIALIZED:");
            break;
        case ESP_ERR_NVS_PART_NOT_FOUND:
            ESP_LOGE(TAG, "PART_NOT_FOUND");
            break;
        case ESP_ERR_NVS_INVALID_NAME:
            ESP_LOGE(TAG, "INVALID_NAME");
            break;
        case ESP_ERR_NO_MEM:
            ESP_LOGE(TAG, "NO_MEM");
            break;
        case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
            ESP_LOGE(TAG, "NOT_ENOUGH_SPACE");
            break;
        case ESP_ERR_NOT_ALLOWED:
            ESP_LOGE(TAG, "NOT_ALLOWED");
            break;

        default:
            break;
        }
    }

    uint32_t apt1 = 0;
    uint32_t apt2 = 0;
    uint32_t apt3 = 0;
    uint32_t apt4 = 0;

    // size_t longitud = 0;
    // err = nvs_get_str(nvs_handle, nvs_token, NULL, &longitud); // Obtener tamaño de el token en flash
    // if (err == ESP_OK)
    // {
    //     token_sesion = malloc(longitud);
    //     err = nvs_get_str(nvs_handle, nvs_token, token_sesion, &longitud);
    // }

    nvs_get_u32(nvs_handle, nvs_flujo_apt_1, &apt1);
    nvs_get_u32(nvs_handle, nvs_flujo_apt_2, &apt2);
    nvs_get_u32(nvs_handle, nvs_flujo_apt_3, &apt3);
    nvs_get_u32(nvs_handle, nvs_flujo_apt_4, &apt4);

    memcpy(&sensores.flujo_apt1, &apt1, sizeof(float));
    memcpy(&sensores.flujo_apt2, &apt2, sizeof(float));
    memcpy(&sensores.flujo_apt3, &apt3, sizeof(float));
    memcpy(&sensores.flujo_apt4, &apt4, sizeof(float));

    printf("Valores en nvs 1: %.3f, 2: %.3f, 3: %.3f, 4: %.3f \n", sensores.flujo_apt1, sensores.flujo_apt2, sensores.flujo_apt3, sensores.flujo_apt4);

    nvs_close(nvs_handle);
}

void limpiar_flujo_flash()
{
    nvs_handle_t nvs_handle;
    nvs_open(FLUJO_NAMESPACE, NVS_READWRITE, &nvs_handle);

    nvs_erase_key(nvs_handle, nvs_flujo_apt_1);
    nvs_erase_key(nvs_handle, nvs_flujo_apt_2);
    nvs_erase_key(nvs_handle, nvs_flujo_apt_3);
    nvs_erase_key(nvs_handle, nvs_flujo_apt_4);
    // nvs_erase_key(nvs_handle, nvs_token);

    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
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

    for (size_t i = 0; i < promedio_presion; i++)
    {
        int lectura = 0;
        adc_oneshot_read(sensor_presion_handle, pin_sensor_presion, &adc_raw);
        adc_cali_raw_to_voltage(sensor_presion_cali_handle, adc_raw, &lectura);
        voltage_mv += lectura;
    }
    voltage_mv = voltage_mv / promedio_presion;

    if (voltage_mv > 367)
    {
        psi = (float)(voltage_mv - 367) * 0.0511;
    }
    else
        psi = 0;

    return psi;
}
