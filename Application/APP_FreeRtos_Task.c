#include "App_FreeRtos_Task.h"

//STM32F103C8T6 -> SRAM:20KB ->12K给FreeRTOS使用
//内存管理 -> C语言结构体存在 堆 中，不会自动垃圾回收，需要手动释放 -> 可使用同一结构体，循环使用

//电机结构体
Motor_Struct left_top_motor={.tim=&htim3,.channel=TIM_CHANNEL_1,.speed=100};
Motor_Struct left_bottom_motor={.tim=&htim4,.channel=TIM_CHANNEL_4,.speed=100};
Motor_Struct right_top_motor={.tim=&htim2,.channel=TIM_CHANNEL_2,.speed=100};
Motor_Struct right_bottom_motor={.tim=&htim1,.channel=TIM_CHANNEL_3,.speed=100};


//LED结构体
LED_Struct left_top_led={.port=LED1_GPIO_Port,.pin=LED1_Pin};
LED_Struct right_top_led={.port=LED2_GPIO_Port,.pin=LED2_Pin};
LED_Struct right_bottom_led={.port=LED3_GPIO_Port,.pin=LED3_Pin};
LED_Struct left_bottom_led={.port=LED4_GPIO_Port,.pin=LED4_Pin};


//遥控状态
Remote_State remote_state=REMOTE_CONNECTED;

//飞行状态
Flight_State flight_state=IDLE;

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


//LED控制任务
void led_task(void *args);
#define LED_TASK_STACKSIZE 128
#define LED_TASK_PRIORITY 1
TaskHandle_t led_task_handle;
//任务周期
#define LED_TASK_PERIOD 100

//SI24R1 通讯任务
void si24r1_task(void *args);
#define SI24R1_TASK_STACKSIZE 128
#define SI24R1_TASK_PRIORITY 2
TaskHandle_t si24r1_task_handle;
//任务周期
#define SI24R1_TASK_PERIOD 6



void App_FreeRTOS_Init(void)
{
    //创建电源管理任务
    xTaskCreate(power_task, "power_task", POWER_TASK_STACKSIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);
    //创建飞行任务
    xTaskCreate(flight_task, "flight_task", FLIGHT_TASK_STACKSIZE, NULL, FLIGHT_TASK_PRIORITY, &flight_task_handle);
    //创建LED控制任务
    xTaskCreate(led_task, "led_task", LED_TASK_STACKSIZE, NULL, LED_TASK_PRIORITY, &led_task_handle);
    //创建SI24R1通讯任务
    xTaskCreate(si24r1_task, "si24r1_task", SI24R1_TASK_STACKSIZE, NULL, SI24R1_TASK_PRIORITY, &si24r1_task_handle);
    
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
        
        // motor_start(&left_top_motor);
        // motor_start(&left_bottom_motor);
        // motor_start(&right_top_motor);
        // motor_start(&right_bottom_motor);

        //motor_set_speed(&left_top_motor);
        //motor_set_speed(&left_bottom_motor);
        //motor_set_speed(&right_top_motor);
        //motor_set_speed(&right_bottom_motor);


        vTaskDelayUntil(&xLastWakeTime, FLIGHT_TASK_PERIOD);

    }
}


void led_task(void *args)
{
    //获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t count=0;
    while(1)
    {
        //闪烁计数
        count++;

        //连接状态：左上、右上亮
        if(remote_state==REMOTE_CONNECTED)
        {
            led_turn_on(&left_top_led);
            led_turn_on(&right_top_led);
        }
        //断开状态：左上、右上灭
        else if(remote_state==REMOTE_DISCONNECTED)
        {
            led_turn_off(&left_top_led);
            led_turn_off(&right_top_led);
        }


        if(flight_state==IDLE)//空闲状态：左下、右下慢闪烁
        {
            //灯慢闪烁 -> 500ms闪烁一次
            if(count%5==0)
            {
                //循环5次 -> 100ms闪烁一次
                led_toggle(&left_bottom_led);
                led_toggle(&right_bottom_led);
            }

        }
        else if(flight_state==NORMAL)//正常飞行状态：左下、右下闪烁

        {
            //灯快闪烁 -> 100ms闪烁一次
            if(count%2==0)
            {
                led_toggle(&left_bottom_led);
                led_toggle(&right_bottom_led);
            }
        }
        else if(flight_state==FIX_HIGH)//固定高度飞行状态：左下、右下亮
        {
            //左下、右下亮
            led_turn_on(&left_bottom_led);
            led_turn_on(&right_bottom_led);
        }
        else if(flight_state==FAIL)//故障状态：左下、右下灭
        {
            //左下、右下灭
            led_turn_off(&left_bottom_led);
            led_turn_off(&right_bottom_led);
        }
        if(count==10)
        {
            count=0;
        }

        vTaskDelayUntil(&xLastWakeTime, LED_TASK_PERIOD);
    }
}


uint8_t si24r1_rx_data[TX_PLOAD_WIDTH]={0};
void si24r1_task(void *args)
{
    //获取当前基准时间
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        //接收数据到缓冲区
        uint8_t res = Int_SI24R1_RxPacket(si24r1_rx_data);
        if(res==0)
        {
            debug_printf("接收数据成功:%s\r\n", si24r1_rx_data);
        }
        //6ms接受一次
        vTaskDelayUntil(&xLastWakeTime, SI24R1_TASK_PERIOD);
    }
}
