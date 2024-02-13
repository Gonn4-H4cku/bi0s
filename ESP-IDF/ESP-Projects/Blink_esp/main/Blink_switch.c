#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    // initialize gpio 22 pin and input pullup AND 26 o/p.
    gpio_set_direction(GPIO_NUM_22,GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_26,GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(GPIO_NUM_22,GPIO_PULLUP_ONLY);

    while (true)
    {
        printf("State of GPIO 22 : %d \n",gpio_get_level(GPIO_NUM_22));
        if(gpio_get_level(GPIO_NUM_22))
        {
            gpio_set_level(GPIO_NUM_26,0);
        }
        else
        {
            gpio_set_level(GPIO_NUM_26,1);
            
        }
            vTaskDelay(10);
    }
    

}
