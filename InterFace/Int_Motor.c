#include "Int_Motor.h"

void motor_set_speed(Motor_Struct *motor)
{
    // 初始化电机，设置速度
    if(motor->speed>1000)
    {
        debug_printf("too high speed\n");
        return;
    }
    __HAL_TIM_SET_COMPARE(motor->tim, motor->channel, motor->speed);
}

void motor_start(Motor_Struct *motor)
{
    // 启动电机
    HAL_TIM_PWM_Start(motor->tim, motor->channel);
}
