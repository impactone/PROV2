#ifndef Driver_DELAY_H
#define Driver_DELAY_H
#include "stm32f4xx.h"
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_init(void);
#endif