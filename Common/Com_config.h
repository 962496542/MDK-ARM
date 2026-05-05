#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

#include "main.h"

//连接状态
typedef enum
{
    REMOTE_CONNECTED=0,
    REMOTE_DISCONNECTED,
}Remote_State;

//飞行状态
typedef enum
{
    IDLE=0,//空闲
    NORMAL,//正常
    FIX_HEIGHT,//定高
    FAIL,//故障
}Flight_State;

//解锁状态
typedef enum
{
    FREE=0,
    MAX,
    MIN,
    LEAVE_MAX,
    UNLOCK,
}Thr_State;

//遥控器数据结构定义
typedef struct
{
    int16_t thr;
    int16_t rol;
    int16_t pit;
    int16_t yaw;
    uint8_t shutdown;    //1:关机 0:不关机
    uint8_t fix_height;  //1:切换是否定高模式 0:不切换
}Remote_Data;



#endif 
