#ifndef __DRIVER_ULTRASONIC_H
#define __DRIVER_ULTRASONIC_H

#include "stm32f4xx.h"
typedef struct
{
	float Left[2];
	float Right[2];
	
}DisType;
 
extern DisType Distance;

void USART2_Process(uint8_t res);
void USART3_Process(uint8_t res);
void UltraSonic_Trigger(void);

#endif 

