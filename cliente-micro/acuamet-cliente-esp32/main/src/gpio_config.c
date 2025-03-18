#include "gpio_config.h"

void gpio_init(void)
{
    //entradas
    gpio_set_direction(configPin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(configPin, GPIO_PULLUP_ONLY);

    //salidas

}