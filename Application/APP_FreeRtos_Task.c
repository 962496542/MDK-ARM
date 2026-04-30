#include "App_FreeRtos_Task.h"

//STM32F103C8T6 -> SRAM:20KB ->12K给FreeRTOS使用
//内存管理 -> C语言结构体存在 堆 中，不会自动垃圾回收，需要手动释放 -> 可使用同一结构体，循环使用
Motor_Struct left_top_motor={.tim=&htim3,.channel=TIM_CHANNEL_1,.speed=200};
Motor_Struct left_bottom_motor={.tim=&htim4,.channel=TIM_CHANNEL_4,.speed=200};
Motor_Struct right_top_motor={.tim=&htim2,.channel=TIM_CHANNEL_2,.speed=200};
Motor_Struct right_bottom_motor={.tim=&htim1,.channel=TIM_CHANNEL_3,.speed=200};



/**电源管理任务
 * 
 * 
 */
void power_task(void *args);
//最小堆栈大小128=128*4=512字节
#define POWER_TASK_STACKSIZE 128
//任务优先级，数值越小优先级越小 -> 最大4 -> 不推荐使用0 -> 空闲任务占用
#define POWER_TASK_PRIORITY 4
TaskHandle_t power_task_handle;
//任务周期
#define POWER_TASK_PERIOD 10000


/**飞行任务
 * 
 */
void flight_task(void *args);
#define FLIGHT_TASK_STACKSIZE 128
#define FLIGHT_TASK_PRIORITY 3
TaskHandle_t flight_task_handle;
//任务周期
#define FLIGHT_TASK_PERIOD 6



void App_FreeRTOS_Init(void)
{
    //创建电源管理任务
    xTaskCreate(power_task, "power_task", POWER_TASK_STACKSIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);
    //创建飞行任务
    xTaskCreate(flight_task, "flight_task", FLIGHT_TASK_STACKSIZE, NULL, FLIGHT_TASK_PRIORITY, &flight_task_handle);

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

void flight_task(void *args)
{
    //获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        motor_init_start(&left_top_motor);

        vTaskDelayUntil(&xLastWakeTime, FLIGHT_TASK_PERIOD);

    }
}

