#include "Int_led.h"


//开灯
void led_turn_on(LED_Struct *led)
{
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}


//关灯
void led_turn_off(LED_Struct *led)
{
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}


//
void led_toggle(LED_Struct *led)
{
    HAL_GPIO_TogglePin(led->port, led->pin);
}
