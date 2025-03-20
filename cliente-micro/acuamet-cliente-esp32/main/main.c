#include "main.h"

const char *TAG = "cliente_acuamet";

float litros_flujo_1 = 0;

void fsmTask(void *pvParameters)
{
    while (1)
    {
        switch (ESTADO_SIGUIENTE)
        {
        case EST_INIT:
            ESTADO_SIGUIENTE = fun_init();
            break;

        case EST_CONFIG:
            ESTADO_SIGUIENTE = fun_config();
            break;

        case EST_WIFICONN:
            ESTADO_SIGUIENTE = fun_wificonn();
            break;

        case EST_MQTTCONN:
            ESTADO_SIGUIENTE = fun_mqttconn();
            break;

        case EST_LECTSENSMQTT:
            ESTADO_SIGUIENTE = fun_lectsensmqtt();
            break;

        case EST_SALIDAS:
            ESTADO_SIGUIENTE = fun_salidas();
            break;

        case EST_PUBMQTT:
            ESTADO_SIGUIENTE = fun_pubmqtt();
            break;

        case EST_ERROR:
            ESTADO_SIGUIENTE = fun_error();
            break;

        default:
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void flujometrosTask(void *pvParameters)
{
    while (1)
    {
        uint32_t pulsos_flujo_1 = 0;
        pcnt_get_counter_value(PCNT_UNIT_0, (int16_t *)&pulsos_flujo_1);
        if (pulsos_flujo_1 > 0)
        {
            pcnt_counter_clear(PCNT_UNIT_0);
        }
        litros_flujo_1 += (float)pulsos_flujo_1 / constante_ppl_flujometro;
        ESP_LOGW(TAG, "Litros Flujometro 1 = %.2f", litros_flujo_1);

        vTaskDelay(pdMS_TO_TICKS(delay_lectura_flujometros));
    }
}

void app_main(void)
{
    xTaskCreate(
        fsmTask,
        "fsmTask",
        4096,
        NULL,
        5,
        NULL);

    xTaskCreate(
        flujometrosTask,
        "flujometrosTask",
        2048,
        NULL,
        5,
        NULL);
}
