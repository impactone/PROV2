#ifndef _DRIVER_RAMP_H
#define _DRIVER_RAMP_H
#include "stm32f4xx.h"
typedef struct 
{
	uint32_t CNT;
	uint32_t TICK;
	float Output;
	
} Ramp;

void Ramp_Init(Ramp *ramp,int16_t tick);
float Ramp_Calc(Ramp *ramp,int16_t max_cnt);
void Ramp_Reint(Ramp *ramp);
void ALL_Ramp_Init(void);
#endif 

