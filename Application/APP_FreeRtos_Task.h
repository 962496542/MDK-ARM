#ifndef __APP_FREE_RTOS_H__
#define __APP_FREE_RTOS_H__


#include "FreeRTOS.h"
#include "task.h"
#include "Com_debug.h"
#include "Int_IP5305T.h"
#include "Int_Motor.h"
#include "Int_led.h"
#include "Com_config.h"
#include "Int_SI24R1.h"
#include "APP_Receive_Data.h"


//初始化FreeRTOS任务
void App_FreeRTOS_Init(void);


#endif 

