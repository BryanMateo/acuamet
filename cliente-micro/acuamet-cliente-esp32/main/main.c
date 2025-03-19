#include "main.h"

const char *TAG = "cliente_acuamet";

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

void app_main(void)
{

    xTaskCreate(
        fsmTask,
        "FSMtask",
        4096,
        NULL,
        5,
        NULL);
}
