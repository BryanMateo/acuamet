#include <stdio.h>
#include <string.h>

// header de configuracion de variables
#include "main.h"
#include "src/wifi_acuamet.h"
#include "src/mqtt_acuamet.h"

// Librerias esp
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"

/////////////
#include "esp_err.h"
#include "esp_mac.h"
/////////////

#include "cJSON.h"

char mac_end[5] = "";
uint8_t mac[6] = "";

void obtener_mac(void)
{
    
    esp_err_t err = esp_efuse_mac_get_default(mac);
    if (err == ESP_OK)
    {
        snprintf(mac_end, sizeof(mac_end), "%02X%02X", mac[4], mac[5]);
        //printf("mac = %s", mac_end);
    }
    else
    {
        printf("Error MAC: %s\n", esp_err_to_name(err));
    }
}

void app_main(void)
{
    obtener_mac();
    ESP_LOGW(TAG, "Inicializando Wi-Fi...");
    wifi_init_sta();

    // Iniciar MQTT


    while (1)
    {
        // ESP_LOGI(TAG, "Test");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
