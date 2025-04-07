#include "fsm.h"

int ESTADO_SIGUIENTE = EST_INIT;
int ESTADO_ACTUAL = EST_INIT;
int ESTADO_ANTERIOR = EST_INIT;

int fun_init(void)
{
    ESP_LOGW(TAG, "Estado Init");

    gpio_init();                                                             // Inicializar todos los GPIO
    LCD_init(i2c_lcd_address, pin_lcd_sda, pin_lcd_scl, lcd_cols, lcd_rows); // Inicializa el LCD
    obtener_mac();

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

    valvulas_init(); // Inicializa las valvulas

    LCD_home();
    LCD_clearScreen();
    LCD_writeStr("Acuamet");
    LCD_setCursor(0, 1);
    LCD_writeStr("v1.0");

    while (1)
    {
        if (gpio_get_level(config_pin) == 0 || !nvs_init()) // modo config si no hay red guardada o pin config a gnd
        {
            vTaskDelay(pdMS_TO_TICKS(3000));
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

    LCD_home();
    LCD_clearScreen();
    LCD_writeStr("Config mode");
    LCD_setCursor(0, 1);
    LCD_writeStr("Wf: ");
    LCD_setCursor(4, 1);
    LCD_writeStr(ap_ssid);
    LCD_setCursor(0, 2);
    LCD_writeStr("Pwd: ");
    LCD_setCursor(5, 2);
    LCD_writeStr("acuametcfg1");

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
    create_topics(); // funcion que crea el string de topicos con el numero de serie del ESP
    mqtt5_app_start();

    while (1)
    {
        if (mqtt_connected)
        {
            return EST_ONLINE;
        }

        vTaskDelay(pdMS_TO_TICKS(delay_estados));
    }
}

int fun_online(void)
{
    ESP_LOGW(TAG, "Estado Online");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_ONLINE;
    LCD_home();
    LCD_clearScreen();
    LCD_writeStr("Online");
    while (1)
    {
        if (wifi_connected & mqtt_connected)
        {
            esp_err_t err = pub_info_sensores_mqtt();
            if (err != ESP_OK)
            {
                // printf("Sensores publicados \n");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(delay_estado_online));
    }
}

int fun_offline(void)
{
    ESP_LOGW(TAG, "Estado Offline");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = EST_OFFLINE;
    LCD_home();
    LCD_clearScreen();
    LCD_writeStr("Offline");

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
