#include "mqtt_acuamet.h"

char client_root_topic[22] = "";
// char client_id_topic[30] = "";
char client_info_topic[36] = "";
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
            const char *json_string = (char *)event_data;

            cJSON *json = cJSON_Parse(json_string);
            if (json == NULL)
            {
                const char *error_ptr = cJSON_GetErrorPtr();
                if (error_ptr != NULL)
                {
                    fprintf(stderr, "Error antes de: %s\n", error_ptr);
                }
                break;
            }

            // Array con las claves de interés
            const char *claves[] = {"valvula_apt1", "valvula_apt2", "valvula_apt3", "valvula_apt4", "bomba"};
            size_t num_claves = sizeof(claves) / sizeof(claves[0]);

            // Iterar sobre las claves y extraer sus valores
            for (size_t i = 0; i < num_claves; ++i)
            {
                cJSON *item = cJSON_GetObjectItemCaseSensitive(json, claves[i]);
                if (cJSON_IsBool(item))
                {
                    printf("%s: %s\n", claves[i], cJSON_IsTrue(item) ? "true" : "false");
                }
                else
                {
                    printf("La clave '%s' no es de tipo booleano o no existe en el JSON.\n", claves[i]);
                }
            }

            cJSON_Delete(json);

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
    sprintf(client_root_topic, "acuamet/%s", mac_str);              // topico principal
    sprintf(client_info_topic, "%s/info", client_root_topic);       // topico de informacion solo PUB
    sprintf(client_control_topic, "%s/control", client_root_topic); // topico de control solo SUB
}

char *sensores_json()
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        // ESP_LOGE("JSON", "Error al crear objeto JSON");
        return NULL;
    }

    // cJSON_AddStringToObject(root, "id", mac_str);
    cJSON_AddStringToObject(root, "key", key);
    cJSON_AddStringToObject(root, "ssid", ssid);
    cJSON_AddNumberToObject(root, "presion", sensores.presion);
    cJSON_AddNumberToObject(root, "nivel_cisterna", sensores.nivel_cisterna);
    cJSON_AddNumberToObject(root, "flujo_apt_1", sensores.flujo_apt1);
    cJSON_AddNumberToObject(root, "flujo_apt_2", sensores.flujo_apt2);
    cJSON_AddNumberToObject(root, "flujo_apt_3", sensores.flujo_apt3);
    cJSON_AddNumberToObject(root, "flujo_apt_4", sensores.flujo_apt4);
    cJSON_AddBoolToObject(root, "valvula_apt1", salida.valvula_apt1);
    cJSON_AddBoolToObject(root, "valvula_apt2", salida.valvula_apt2);
    cJSON_AddBoolToObject(root, "valvula_apt3", salida.valvula_apt3);
    cJSON_AddBoolToObject(root, "valvula_apt4", salida.valvula_apt4);
    cJSON_AddBoolToObject(root, "bomba", salida.bomba);

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

    client_mqtt = esp_mqtt_client_init(&mqtt5_cfg);
    esp_mqtt_client_register_event(client_mqtt, ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL);
    esp_mqtt_client_start(client_mqtt);
}

esp_err_t pub_info_sensores_mqtt(void)
{
    char *json_message = sensores_json();
    if (json_message != NULL)
    {
        int msg_id = esp_mqtt_client_publish(client_mqtt, client_info_topic, json_message, 0, 1, 1);
        free(json_message);
        if (msg_id == 0)
            return ESP_OK;
    }
    return ESP_FAIL;
}
