#ifndef __INT_LED_H
#define __INT_LED_H


#include "main.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
}LED_Struct;


//初始化LED

void led_turn_on(LED_Struct *led);
void led_turn_off(LED_Struct *led);

void led_toggle(LED_Struct *led);

#endif
