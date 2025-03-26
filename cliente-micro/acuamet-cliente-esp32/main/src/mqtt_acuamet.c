#include "mqtt_acuamet.h"

char client_root_topic[22] = "";
char client_id_topic[30] = "";
char client_sensores_topic[36] = "";
char client_control_topic[30] = "";

bool mqtt_connected = false;

static void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch ((esp_mqtt_event_id_t)event_id)
    {

    case MQTT_EVENT_CONNECTED:
        esp_mqtt_client_subscribe(client, client_control_topic, 1);
        mqtt_connected = true;
        break;

    case MQTT_EVENT_DISCONNECTED:
        mqtt_connected = false;
        break;

    case MQTT_EVENT_DATA:

        if (strncmp(event->topic, client_control_topic, event->topic_len) == 0)
        {

            // char received_data[2]; // Solo esperamos "0" o "1"
            // snprintf(received_data, sizeof(received_data), "%.*s", event->data_len, event->data);
            // if (strcmp(received_data, "1") == 0)
            // {
            //     // sppButtonMQTT = 1;
            //     ESP_LOGI(TAG, "SPP");
            // }
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
    sprintf(client_root_topic, "acuamet/%s", mac_str);                     // topico principal
    sprintf(client_id_topic, "%s/info/id", client_root_topic);             // topico de informacion id solo PUB
    sprintf(client_sensores_topic, "%s/info/sensores", client_root_topic); // topico de informacion sensores solo PUB
    sprintf(client_control_topic, "%s/control", client_root_topic);        // topico de control solo SUB

    // ESP_LOGW(TAG, "ROOT TOPIC: %s", client_root_topic);
    // ESP_LOGW(TAG, "TOPICO INFO: %s", client_info_topic);
}

char *id_json()
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        // ESP_LOGE("JSON", "Error al crear objeto JSON");
        return NULL;
    }

    cJSON_AddStringToObject(root, "id", mac_str);
    // cJSON_AddNumberToObject(root, "temperature", 25.4);
    // cJSON_AddBoolToObject(root, "status", true);

    // Serializar el JSON a una cadena
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    return json_str;
}

char *sensores_json()
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        // ESP_LOGE("JSON", "Error al crear objeto JSON");
        return NULL;
    }

    cJSON_AddNumberToObject(root, "presion", sensores.presion);
    cJSON_AddNumberToObject(root, "nivel_cisterna", sensores.nivel_cisterna);
    cJSON_AddNumberToObject(root, "flujo_apt_1", sensores.flujo_apt1);
    cJSON_AddNumberToObject(root, "flujo_apt_2", sensores.flujo_apt2);
    cJSON_AddNumberToObject(root, "flujo_apt_3", sensores.flujo_apt3);
    cJSON_AddNumberToObject(root, "flujo_apt_4", sensores.flujo_apt4);

    // Serializar el JSON a una cadena
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    return json_str;
}
esp_mqtt_client_handle_t client_mqtt;

void mqtt5_app_start(void)
{
    esp_mqtt_client_config_t mqtt5_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
        .session.protocol_ver = MQTT_PROTOCOL_V_5,
        .network.disable_auto_reconnect = false,
    };

    // esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt5_cfg);
    client_mqtt = esp_mqtt_client_init(&mqtt5_cfg);
    esp_mqtt_client_register_event(client_mqtt, ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL);
    esp_mqtt_client_start(client_mqtt);

    // create_topics();

    char *json_message = id_json();
    if (json_message != NULL)
    {
        // Publicar el mensaje en el tópico MQTT
        esp_mqtt_client_publish(client_mqtt, client_id_topic, json_message, 0, 1, 1);
        free(json_message);
    }
}

esp_err_t pub_info_sensores_mqtt(void)
{
    char *json_message = sensores_json();
    if (json_message != NULL)
    {
        // Publicar el mensaje en el tópico MQTT
        int msg_id = esp_mqtt_client_publish(client_mqtt, client_sensores_topic, json_message, 0, 1, 1);
        free(json_message);
        if (msg_id == 0)
            return ESP_OK;
        }
    return ESP_FAIL;
}
