#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define GPIO_PIN GPIO_NUM_12

void app_main(void)
{
    gpio_set_direction(GPIO_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_PIN, GPIO_PULLUP_ONLY);

    int secnd = 0;  
    while (1)
    {
        int val = gpio_get_level(GPIO_PIN);  
        if (val == 0)
        {
            secnd++;  
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        else
        {
            if(secnd>0)
            {
            printf("Seconds : %d\n",secnd);
            secnd=0;
            }

        }

        vTaskDelay(pdMS_TO_TICKS(1000));  
    }
}