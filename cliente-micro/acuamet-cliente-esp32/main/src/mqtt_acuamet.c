#include <stdio.h>

#include "../main.h"
#include "mqtt_acuamet.h"

#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "cJSON.h"

char root_topic[22] = "";

char client_info_topic[28] = "";

static void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id)
    {

    case MQTT_EVENT_CONNECTED:
        msg_id = esp_mqtt_client_subscribe(client, "/2022-1151/SPP", 1);
        break;

    case MQTT_EVENT_DATA:

        if (strncmp(event->topic, "/2022-1151/SPP", event->topic_len) == 0)
        {
            char received_data[2]; // Solo esperamos "0" o "1"
            snprintf(received_data, sizeof(received_data), "%.*s", event->data_len, event->data);
            if (strcmp(received_data, "1") == 0)
            {
                // sppButtonMQTT = 1;
                ESP_LOGI(TAG, "SPP");
            }
        }
        break;

    case MQTT_EVENT_ERROR:
        // ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;

    default:
        // ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void create_topics(void)
{
    snprintf(root_topic, sizeof(root_topic), "acuamet/%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    ESP_LOGW(TAG, "ROOT TOPIC: %s", root_topic);
    sprintf(client_info_topic, "%s/info", root_topic);
    ESP_LOGW(TAG, "TOPICO INFO: %s", client_info_topic);
}

char *json()
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE("JSON", "Error al crear objeto JSON");
        return NULL;
    }

    // Crear el JSON con datos de ejemplo
    cJSON_AddStringToObject(root, "device", "ESP32");
    cJSON_AddNumberToObject(root, "temperature", 25.4);
    cJSON_AddBoolToObject(root, "status", true);

    // Serializar el JSON a una cadena
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    return json_str;
}

void mqtt5_app_start(void)
{
    esp_mqtt_client_config_t mqtt5_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
        .session.protocol_ver = MQTT_PROTOCOL_V_5,
        .network.disable_auto_reconnect = false,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt5_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL);
    esp_mqtt_client_start(client);

    create_topics();

    char *json_message = json();
    if (json_message != NULL)
    {
        // Publicar el mensaje en el tópico MQTT
        esp_mqtt_client_publish(client, client_info_topic, json_message, 0, 1, 0);
        free(json_message);
    }
}
