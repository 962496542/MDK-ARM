#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__

#include "usart.h"
#include "stdio.h"
#include "stdarg.h"

//日志输出打印开关
#define DEBUG_PRINT 1


#if DEBUG_PRINT

//宏定义，只打印文件名称，不打印路径名称
//strrchr()从后面开始查找字符，找到返回指向该位置的指针。如果没有找到则返回NULL.

#define __FILE_NAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)    


//打印调试信息，自动带上文件名和行号
#define debug_printf(format,...) printf("[%s:%d]"format,__FILE__,__LINE__,##__VA_ARGS__)
#else
    #define debug_printf(...)
#endif


#endif 
