#include "mqtt_acuamet.h"

char client_root_topic[22] = "";      // base de los topicos - cliente_id
char client_info_topic[27] = "";      // topico de informacion de sensores
char client_req_token_topic[32] = ""; // topico para solicitar token
char client_res_token_topic[32] = ""; // topico donde se recibe el token de la sesion actual de contabilizacion de consumo
char client_control_topic[30] = "";   // topico de control
char *token_sesion;

esp_mqtt_client_handle_t client_mqtt;

static void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch ((esp_mqtt_event_id_t)event_id)
    {

    case MQTT_EVENT_CONNECTED:
        esp_mqtt_client_subscribe(client, client_control_topic, 1);
        esp_mqtt_client_subscribe(client, client_res_token_topic, 1);
        flag.mqtt_connected = true;
        break;

    case MQTT_EVENT_DISCONNECTED:
        flag.mqtt_connected = false;
        break;

    case MQTT_EVENT_DATA:

        if (strncmp(event->topic, client_control_topic, event->topic_len) == 0) // topico de control
        {
            char *data = strndup(event->data, event->data_len);
            // printf("control topic: %s \n", data);

            cJSON *json = cJSON_Parse(data);
            if (json == NULL)
            {
                const char *error_ptr = cJSON_GetErrorPtr();
                if (error_ptr != NULL)
                {
                    fprintf(stderr, "Error antes de: %s\n", error_ptr);
                }
                break;
            }
            cJSON *item = cJSON_GetObjectItemCaseSensitive(json, "valvula_apt1");
            if (cJSON_IsBool(item))
                salida.valvula_apt1 = cJSON_IsTrue(item) ? true : false;

            item = cJSON_GetObjectItemCaseSensitive(json, "valvula_apt2");
            if (cJSON_IsBool(item))
                salida.valvula_apt2 = cJSON_IsTrue(item) ? true : false;

            item = cJSON_GetObjectItemCaseSensitive(json, "valvula_apt3");
            if (cJSON_IsBool(item))
                salida.valvula_apt3 = cJSON_IsTrue(item) ? true : false;

            item = cJSON_GetObjectItemCaseSensitive(json, "valvula_apt4");
            if (cJSON_IsBool(item))
                salida.valvula_apt4 = cJSON_IsTrue(item) ? true : false;

            item = cJSON_GetObjectItemCaseSensitive(json, "bomba");
            if (cJSON_IsBool(item))
                salida.bomba = cJSON_IsTrue(item) ? true : false;

            // printf("%s, %s, %s, %s, %s\n", salida.valvula_apt1 ? "true" : "false", salida.valvula_apt2 ? "true" : "false", salida.valvula_apt3 ? "true" : "false", salida.valvula_apt4 ? "true" : "false", salida.bomba ? "true" : "false");

            cJSON_Delete(json);
            free(data);
            flag.control = true;
        }

        if (strncmp(event->topic, client_res_token_topic, event->topic_len) == 0 && !flag.token_asignado) // topico de token de sesion
        {
            char *data = strndup(event->data, event->data_len);
            // printf("token topic: %s \n", data);

            cJSON *json = cJSON_Parse(data);
            if (json == NULL)
            {
                const char *error_ptr = cJSON_GetErrorPtr();
                if (error_ptr != NULL)
                {
                    fprintf(stderr, "Error antes de: %s\n", error_ptr);
                }
                break;
            }
            cJSON *item = cJSON_GetObjectItemCaseSensitive(json, "token");
            if (cJSON_IsString(item))
            {
                token_sesion = strdup(item->valuestring);
                // printf("token: %s", token_sesion);
            }

            cJSON_Delete(json);
            free(data);
            flag.token_asignado = true;
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

void create_topics(void)
{
    sprintf(client_root_topic, "acuamet/%s", mac_str);                  // topico principal
    sprintf(client_info_topic, "%s/info", client_root_topic);           // topico de informacion solo PUB
    sprintf(client_req_token_topic, "%s/token/req", client_root_topic); // topico de token de sesion solo PUB
    sprintf(client_res_token_topic, "%s/token/res", client_root_topic); // topico de token de sesion solo SUB
    sprintf(client_control_topic, "%s/control", client_root_topic);     // topico de control solo SUB
}

char *sensores_json()
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        // ESP_LOGE("JSON", "Error al crear objeto JSON");
        return NULL;
    }
    
    cJSON_AddStringToObject(root, "key", key);
    cJSON_AddStringToObject(root, "ssid", ssid);
    cJSON_AddStringToObject(root, "token", token_sesion);
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

char *token_sesion_json(bool req)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        // ESP_LOGE("JSON", "Error al crear objeto JSON");
        return NULL;
    }
    cJSON_AddBoolToObject(root, "solicitar_token", req);
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str;
}

esp_err_t pub_info_sensores_mqtt(void)
{
    char *json_message = sensores_json();
    if (json_message != NULL)
    {
        int msg_id = esp_mqtt_client_publish(client_mqtt, client_info_topic, json_message, 0, 0, 1);
        free(json_message);
        if (msg_id > -1)
        {
            return ESP_OK;
        }
    }
    return ESP_FAIL;
}

esp_err_t pub_pedir_token_sesion_mqtt(bool req) // funcion para pedir token de sesion al servidor para evitar informacion duplicada
{
    char *json_message = token_sesion_json(req);
    if (json_message != NULL)
    {
        int msg_id = esp_mqtt_client_publish(client_mqtt, client_req_token_topic, json_message, 0, 1, 0);
        free(json_message);
        if (msg_id > -1)
            return ESP_OK;
        else
            printf("Pedir token fail");
    }
    return ESP_FAIL;
}
