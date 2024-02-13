#include <stdio.h>
#include "driver/touch_pad.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

uint16_t  touch_value;
void app_main(void)
{
    gpio_set_direction(GPIO_NUM_2,GPIO_MODE_OUTPUT);
    touch_pad_init();
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_config(TOUCH_PAD_NUM4,0);

    while (1)
    {
        touch_pad_read(TOUCH_PAD_NUM4,&touch_value);
        //printf("%u \n",touch_value);
        if(touch_value<300)
        {
            gpio_set_level(GPIO_NUM_2,1);
        }
        else
        {
            gpio_set_level(GPIO_NUM_2,0);

        }
        vTaskDelay(pdMS_TO_TICKS(30));
    }
    
    

}
