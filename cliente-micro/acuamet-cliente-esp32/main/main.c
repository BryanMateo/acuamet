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

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

    esp_netif_init();
    esp_event_loop_create_default();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    obtener_mac();
    // ESP_LOGW(TAG, "Inicializando Wi-Fi...");
    // wifi_init_sta();

    xTaskCreate(
        fsmTask,
        "FSMtask",
        4096,
        NULL,
        5,
        NULL);

    // Iniciar MQTT

    // while (1)
    // {
        

    //     // ESP_LOGI(TAG, "Test");
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }
}
