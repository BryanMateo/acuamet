#include "fsm.h"

int ESTADO_SIGUIENTE = EST_INIT;
int ESTADO_ACTUAL = EST_INIT;
int ESTADO_ANTERIOR = EST_INIT;

int fun_init(void)
{
    gpio_init(); // Inicializar todos los GPIO
    obtener_mac();
    ESP_LOGW(TAG, "Estado Init");

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

    while (1)
    {
        if (gpio_get_level(configPin) == 0 || !nvs_init()) // modo config si no hay red guardada o pin config a gnd
        {
            return EST_CONFIG;
        }
        else
        {
            return EST_WIFICONN;
        }
        vTaskDelay(pdMS_TO_TICKS(delay_estados));
    }
}

int fun_config(void)
{
    ESP_LOGW(TAG, "Estado Config");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_CONFIG;
    config();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(delay_estados));
    }
}

int fun_wificonn(void)
{
    ESP_LOGW(TAG, "Estado Wificonn");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_WIFICONN;
    wifi_init_sta();

    while (1)
    {
        if (wifi_connected)
        {
            return EST_MQTTCONN;
        }

        vTaskDelay(pdMS_TO_TICKS(delay_estados));
    }
}

int fun_mqttconn(void)
{
    ESP_LOGW(TAG, "Estado MQTTconn");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_MQTTCONN;
    mqtt5_app_start();

    while (1)
    {
        if (mqtt_connected)
        {
            return EST_LECTSENSMQTT;
        }

        vTaskDelay(pdMS_TO_TICKS(delay_estados));
    }
}

int fun_lectsensmqtt(void)
{
    ESP_LOGW(TAG, "Estado LectsensMQTT");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_LECTSENSMQTT;

    while (1)
    {
        // if (wifi_connected)

        vTaskDelay(pdMS_TO_TICKS(delay_estados));
    }
}

int fun_salidas(void)
{
    ESP_LOGW(TAG, "Estado Salidas");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_SALIDAS;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(delay_estados));
    }
}

int fun_pubmqtt(void)
{
    ESP_LOGW(TAG, "Estado PubMQTT");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_PUBMQTT;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(delay_estados));
    }
}

int fun_error(void)
{
    ESP_LOGW(TAG, "Estado Error");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_ERROR;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(delay_estados));
    }
}
