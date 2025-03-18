#include "wifi_acuamet.h"

char ssid[32] = {0}, password[32] = {0};
char mac_end[5] = "";
uint8_t mac[6] = "";
bool wifi_connected = false;

const char *html_page = "<!DOCTYPE html>"
                        "<html>"
                        "<head>"
                        "<meta charset=\"UTF-8\">"
                        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">" // 🔹 Ajusta la escala en móviles
                        "<title>Configurar WiFi</title>"
                        "<style>"
                        "body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }"
                        "form { max-width: 300px; margin: auto; padding: 20px; background: #f8f8f8; border-radius: 10px; box-shadow: 0px 0px 10px rgba(0,0,0,0.1); }"
                        "input { width: 100%; padding: 8px; margin: 8px 0; border: 1px solid #ccc; border-radius: 5px; }"
                        "input[type='submit'] { background: #007bff; color: white; border: none; cursor: pointer; }"
                        "input[type='submit']:hover { background: #0056b3; }"
                        "</style>"
                        "</head>"
                        "<body>"
                        "<h2>Ingrese SSID y Contraseña</h2>"
                        "<form action='/wifi' method='post'>"
                        "SSID: <input type='text' name='ssid' placeholder='Ingrese SSID'><br>"
                        "Contraseña: <input type='password' name='password' placeholder='Ingrese contraseña'><br>"
                        "<input type='submit' value='Guardar'>"
                        "</form>"
                        "</body>"
                        "</html>";

static EventGroupHandle_t wifi_event_group;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI(TAG, "Conexión fallida, intentando reconectar...");
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
        wifi_connected = false;
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Conectado con IP:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        wifi_connected = true;
    }
}

bool get_wifi_credentials(char *ssid, char *password, size_t max_len)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "No se pudo abrir NVS");
        return false;
    }

    size_t ssid_len = max_len;
    size_t password_len = max_len;

    err = nvs_get_str(nvs_handle, SSID_KEY, ssid, &ssid_len);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "No se encontró SSID en NVS");
        nvs_close(nvs_handle);
        return false;
    }

    err = nvs_get_str(nvs_handle, PASSWORD_KEY, password, &password_len);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "No se encontró contraseña en NVS");
        nvs_close(nvs_handle);
        return false;
    }

    nvs_close(nvs_handle);
    return true;
}

void save_wifi_credentials(const char *ssid, const char *password)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "No se pudo abrir NVS");
        return;
    }

    nvs_set_str(nvs_handle, SSID_KEY, ssid);
    nvs_set_str(nvs_handle, PASSWORD_KEY, password);
    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "Credenciales WiFi guardadas en NVS");
}

esp_err_t wifi_get_handler(httpd_req_t *req)
{
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t wifi_post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (ret <= 0)
    {
        return ESP_FAIL;
    }

    buf[ret] = '\0';

    char ssid[32] = {0}, password[32] = {0};

    char *ssid_param = strstr(buf, "ssid=");
    char *password_param = strstr(buf, "&password=");

    if (ssid_param && password_param)
    {
        *password_param = '\0'; // Reemplazar '&' por fin de string
        password_param += 10;   // Mover puntero después de "&password="

        strncpy(ssid, ssid_param + 5, sizeof(ssid) - 1);         // Extraer ssid
        strncpy(password, password_param, sizeof(password) - 1); // Extraer password
    }

    save_wifi_credentials(ssid, password);

    const char *resp_str = "<html><body><h2>Credenciales guardadas. Reiniciando...</h2></body></html>";
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    vTaskDelay(3000 / portTICK_PERIOD_MS);
    esp_restart();
    return ESP_OK;
}

httpd_handle_t start_webserver()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t get_uri = {.uri = "/", .method = HTTP_GET, .handler = wifi_get_handler, .user_ctx = NULL};
        httpd_uri_t post_uri = {.uri = "/wifi", .method = HTTP_POST, .handler = wifi_post_handler, .user_ctx = NULL};

        httpd_register_uri_handler(server, &get_uri);
        httpd_register_uri_handler(server, &post_uri);
    }
    return server;
}

void start_softap()
{
    char ap_ssid[13] = "acuamet_";
    strcat(ap_ssid, mac_end);

    esp_netif_create_default_wifi_ap();
    wifi_config_t wifi_config = {
        .ap = {
            //.ssid = "ESP32_Config",
            //.ssid_len = strlen("ESP32_Config"),
            .channel = 1,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .max_connection = 4,
            .password = "12345678"},
    };
    strncpy((char *)wifi_config.ap.ssid, ap_ssid, sizeof(wifi_config.ap.ssid));
    wifi_config.ap.ssid[sizeof(wifi_config.ap.ssid) - 1] = '\0'; // Asegurar terminación

    if (strlen((char *)wifi_config.ap.password) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "SoftAP iniciado: SSID=%s, IP=192.168.4.1", ap_ssid);
    return;
}

void start_wifi_sta(const char *ssid, const char *password)
{
    wifi_event_group = xEventGroupCreate();
    esp_netif_create_default_wifi_sta();

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip);

    wifi_config_t wifi_config = {

    };
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, password, sizeof(wifi_config.sta.password));

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGW(TAG, "Intentando conectar a Wifi");

    EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGW(TAG, "Wifi OK");
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGE(TAG, "Wifi Fail");
    }
    else
    {
        ESP_LOGE(TAG, "Wifi Error inesperado");
    }
}

bool nvs_init(void)
{
    if (get_wifi_credentials(ssid, password, sizeof(ssid)))
        return true;
    else
        return false;
}

void config(void)
{
    ESP_LOGW(TAG, "Inicializando AP");
    start_softap();
    ESP_LOGW(TAG, "Inicializando WS");
    start_webserver();
}

void wifi_init_sta(void)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    start_wifi_sta(ssid, password);
}

void obtener_mac(void)
{
    esp_err_t err = esp_efuse_mac_get_default(mac);
    if (err == ESP_OK)
    {
        snprintf(mac_end, sizeof(mac_end), "%02X%02X", mac[4], mac[5]);
        // printf("mac = %s", mac_end);
    }
    else
    {
        printf("Error MAC: %s\n", esp_err_to_name(err));
    }
}
