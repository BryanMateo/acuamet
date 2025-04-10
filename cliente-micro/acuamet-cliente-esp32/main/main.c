#include "main.h"

const char *TAG = "cliente_acuamet";

struct SENSORES sensores;
struct SALIDA salida;

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
    sensores.flujo_apt1 = 0;
    sensores.flujo_apt2 = 0;
    sensores.flujo_apt3 = 0;
    sensores.flujo_apt4 = 0;
    sensores.presion = 0;

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
            // printf("Presion = %.2fPSI \n", presion);
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
            ESP_LOGE(TAG, "Ultras. err rango");
            break;
        case SENS_ERR_TIMEOUT:
            ESP_LOGE(TAG, "Ultras. timeout");
            break;
        default:
            ESP_LOGI(TAG, "Distancia %icm", sensores.nivel_cisterna);
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(delay_lectura_cisterna));
    }
}

// void salidas_task(void *pvParameters)
// {
//     while (1)
//     {

//         vTaskDelay(pdMS_TO_TICKS(delay_salidas_task))
//     }
// }

/*
void LCD_DemoTask(void *param)
{
    LCD_init(i2c_lcd_address, pin_lcd_sda, pin_lcd_scl, lcd_cols, lcd_rows);
    char txtBuf[8];
    while (true)
    {
        int row = 0, col = 0;
        LCD_home();
        LCD_clearScreen();
        LCD_writeStr("---16x4 LCD---");
        LCD_setCursor(0, 1);
        LCD_writeStr("LCD LibraryDemo");
        LCD_setCursor(0, 2);
        LCD_writeStr("Time: ");
        for (int i = 10; i >= 0; i--)
        {
            LCD_setCursor(6, 3);
            sprintf(txtBuf, "%02d", i);
            LCD_writeStr(txtBuf);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        LCD_clearScreen();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
*/

void app_main(void)
{
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

    // xTaskCreate(
    //     salidas_task,
    //     "nivel cisterna",
    //     4096,
    //     NULL,
    //     5,
    //     NULL);
}
