#ifndef __INT_MOTOR_H
#define __INT_MOTOR_H

#include "tim.h"
#include "Com_debug.h"

typedef struct 
{
	TIM_HandleTypeDef *tim;
    uint16_t channel;
    uint16_t speed;
}Motor_Struct;


/**
 * @brief 传入比较值，最大速度为1000，最小速度为0，默认200
 *
 * @param speed 速度值
 */
void motor_init_set_speed(Motor_Struct *motor);

/**
 * @brief 启动电机，传入结构体
 * 
 * @param
 */
void motor_init_start(Motor_Struct *motor);



#endif
