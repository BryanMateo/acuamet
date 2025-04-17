#include "main.h"

const char *TAG = "cliente_acuamet";

struct CONTADORES contadores;
struct SENSORES sensores;
struct SALIDA salida;
struct FLAG flag;


void fsm_task(void *pvParameters)
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

        case EST_ONLINE:
            ESTADO_SIGUIENTE = fun_online();
            break;

        case EST_OFFLINE:
            ESTADO_SIGUIENTE = fun_offline();
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

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void flujo_presion_task(void *pvParameters)
{

    while (1)
    {
        if (gpio_inicializado) // espera a que todos los gpio se inicialicen
        {
            sensores.flujo_apt1 += calc_galon_flujo(pcnt_unit_flujo_1);
            sensores.flujo_apt2 += calc_galon_flujo(pcnt_unit_flujo_2);
            sensores.flujo_apt3 += calc_galon_flujo(pcnt_unit_flujo_3);
            sensores.flujo_apt4 += calc_galon_flujo(pcnt_unit_flujo_4);

            sensores.presion = read_pin_presion();

            // printf("1: %.2f 2: %.2f 3: %.2f 4: %.2f \n", sensores.flujo_apt1, sensores.flujo_apt2, sensores.flujo_apt3, sensores.flujo_apt4);
            //  printf("Presion = %.2fPSI \n", sensores.presion);
        }
        vTaskDelay(pdMS_TO_TICKS(delay_lectura_flujometros));
    }
}

void nivel_cisterna_task(void *pvParameters)
{
    sensores.nivel_cisterna = 0;

    while (1)
    {
        sensores.nivel_cisterna = nivel_cisterna_distance();

        switch (sensores.nivel_cisterna)
        {
        case SENS_ERR_RANGE:
            //   ESP_LOGE(TAG, "Ultras. err rango");
            break;
        case SENS_ERR_TIMEOUT:
            // ESP_LOGE(TAG, "Ultras. timeout");
            break;
        default:
            // ESP_LOGI(TAG, "Distancia %icm", sensores.nivel_cisterna);
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(delay_lectura_cisterna));
    }
}

void salidas_task(void *pvParameters)
{
    while (1)
    {
        if (flag.control)
        {
            control_valvula(LEDC_VALVULA_1, salida.valvula_apt1);
            control_valvula(LEDC_VALVULA_2, salida.valvula_apt2);
            control_valvula(LEDC_VALVULA_3, salida.valvula_apt3);
            control_valvula(LEDC_VALVULA_4, salida.valvula_apt4);

            printf("Control\n");
            flag.control = false;
        }

        vTaskDelay(pdMS_TO_TICKS(delay_salidas_task));
    }
}

void lcd_task(void *pvParameters)
{
    LCD_init(i2c_lcd_address, pin_lcd_sda, pin_lcd_scl, lcd_cols, lcd_rows); // Inicializa el LCD
    while (1)
    {
        LCD_home();
        LCD_clearScreen();
        switch (ESTADO_ACTUAL)
        {
        case EST_INIT:
            LCD_writeStr("Acuamet");
            LCD_setCursor(0, 1);
            LCD_writeStr("v1.0");
            break;

        case EST_CONFIG:
            LCD_writeStr("Config mode");
            LCD_setCursor(0, 1);
            LCD_writeStr("Wf: ");
            LCD_setCursor(4, 1);
            LCD_writeStr(ap_ssid);
            LCD_setCursor(0, 2);
            LCD_writeStr("Pwd: ");
            LCD_setCursor(5, 2);
            LCD_writeStr("acuametcfg1");
            break;

        case EST_ONLINE:
            LCD_writeStr("Online");
            break;

        case EST_OFFLINE:
            LCD_writeStr("Offline");
            break;
        default:
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(delay_lcd_task));
    }
}

void app_main(void)
{
    // Inicializacion de flags
    flag.control = false;
    flag.wifi_cfg = false;
    flag.wifi_connected = false;
    flag.mqtt_cfg = false;
    flag.mqtt_connected = false;
    flag.token_asignado = false;

    // Inicializacion de contadores y sensores
    sensores.flujo_apt1 = 0;
    sensores.flujo_apt2 = 0;
    sensores.flujo_apt3 = 0;
    sensores.flujo_apt4 = 0;
    sensores.presion = 0;

    token_sesion = "null";

    contadores.apt_1 = 0;
    contadores.apt_2 = 0;
    contadores.apt_3 = 0;
    contadores.apt_4 = 0;

    xTaskCreate(
        fsm_task,
        "fsm",
        4096,
        NULL,
        5,
        NULL);

    xTaskCreate(
        flujo_presion_task,
        "flujo_presion",
        4096,
        NULL,
        5,
        NULL);

    xTaskCreate(
        nivel_cisterna_task,
        "nivel cisterna",
        4096,
        NULL,
        5,
        NULL);

    xTaskCreate(
        salidas_task,
        "salidas",
        4096,
        NULL,
        5,
        NULL);

    xTaskCreate(
        lcd_task,
        "salidas",
        4096,
        NULL,
        5,
        NULL);
}
