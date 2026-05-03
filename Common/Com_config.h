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
    FIX_HIGH,//定高
    FAIL,//故障
}Flight_State;


#endif 
