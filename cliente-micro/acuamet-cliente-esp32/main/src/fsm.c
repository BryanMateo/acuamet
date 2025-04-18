#include "fsm.h"

int ESTADO_SIGUIENTE = EST_INIT;
int ESTADO_ACTUAL = EST_INIT;
int ESTADO_ANTERIOR = EST_INIT;

int fun_init(void)
{
    ESP_LOGW(TAG, "Estado Init");

    // Inicializacion de la nvs y parametros de wifi
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

    gpio_init(); // Inicializar todos los GPIO

    valvulas_init(); // Inicializa las valvulas

    obtener_flujo_flash(); // obtiene el token y el flujo anterior si estuvo offline

    while (1)
    {
        if (gpio_get_level(config_pin) == 0 || !nvs_init()) // modo config si no hay red guardada o pin config a gnd
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
            return EST_CONFIG;
        }
        else
        {
            if (!flag.wifi_cfg) // configura una sola vez el wifi
            {
                wifi_init_sta();
                flag.wifi_cfg = true;
            }

            if (!flag.mqtt_cfg) // configura una sola vez el cliente mqtt
            {
                create_topics(); // funcion que crea los strings de topicos con el numero de serie del ESP
                mqtt5_app_start();
                flag.mqtt_cfg = true;
            }

            return EST_MQTTCONN;
        }
        vTaskDelay(pdMS_TO_TICKS(delay_estados));
    }
}

int fun_config(void)
{
    ESP_LOGW(TAG, "Estado Config");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    config();
    ESTADO_ACTUAL = EST_CONFIG;

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
        if (flag.wifi_connected)
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

    while (1)
    {
        if (flag.wifi_connected && flag.mqtt_connected)
        {

            if (!flag.token_asignado)
            {
                pub_pedir_token_sesion_mqtt(true);
                // vTaskDelay(pdMS_TO_TICKS(1000)); // tiempo de espera de 5 seg para recibir token
            }
            else // recibio token nuevo
            {
                pub_info_sensores_mqtt();
                sensores.flujo_apt1 = 0;
                sensores.flujo_apt2 = 0;
                sensores.flujo_apt3 = 0;
                sensores.flujo_apt4 = 0;
                pub_pedir_token_sesion_mqtt(false);
                return EST_ONLINE;
            }
        }
        else
            return EST_OFFLINE;

        vTaskDelay(pdMS_TO_TICKS(delay_estado_mqttconn));
    }
}

int fun_online(void)
{
    ESP_LOGW(TAG, "Estado Online");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_ONLINE;
    limpiar_flujo_flash();

    while (1)
    {
        if (flag.wifi_connected && flag.mqtt_connected)
        {
            esp_err_t err = pub_info_sensores_mqtt();
            if (err == ESP_OK)
            {
               // ESP_LOGI(TAG, "Sensores publicados");
            }
        }
        else
        {
            printf("no wifi o mqtt \n");
            return EST_OFFLINE;
        }
        vTaskDelay(pdMS_TO_TICKS(delay_estado_online));
    }
}

int fun_offline(void)
{
    ESP_LOGW(TAG, "Estado Offline");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_OFFLINE;

    while (1)
    {
        if (flag.wifi_connected && flag.mqtt_connected)
        {
            if (!flag.token_asignado)
            {
                return EST_MQTTCONN;
            }
            else
            {
                // limpiar_flujo_flash();
                return EST_ONLINE;
            }
        }
        else
            guardar_flujo_flash();

        vTaskDelay(pdMS_TO_TICKS(delay_estado_offline));
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
