#include "App_FreeRtos_Task.h"

//STM32F103C8T6 -> SRAM:20KB ->12K给FreeRTOS使用



void power_task(void *args);
//最小堆栈大小128=128*4=512字节
#define POWER_TASK_STACKSIZE 128

//任务优先级，数值越小优先级越小 -> 最大4 -> 不推荐使用0 -> 空闲任务占用
#define POWER_TASK_PRIORITY 4
TaskHandle_t power_task_handle;


void App_FreeRTOS_Init(void)
{
    //创建电源管理任务
    xTaskCreate(power_task, "power_task", POWER_TASK_STACKSIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);
    //启动调度器
    vTaskStartScheduler();
}

void power_task(void *args)
{
    //获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
       
       //每十秒启动一次电源
       vTaskDelayUntil(&xLastWakeTime, 10000);
       IP5305T_Init();
    }
}
