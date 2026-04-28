#include "Com_debug.h"

//重定向fputc函数，用于调试信息输出到文件

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 1000);
    return ch;
}
