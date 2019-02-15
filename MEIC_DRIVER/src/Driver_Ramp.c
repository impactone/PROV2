#include "Driver_Ramp.h"
Ramp xxxRamp;
void ALL_Ramp_Init(void)
{
    
}
void Ramp_Init(Ramp *ramp,int16_t tick)
{
	ramp -> CNT=0;
	ramp -> TICK=tick;
	ramp -> Output=0.0f;	
}
float Ramp_Calc(Ramp *ramp,int16_t max_cnt)
{
	ramp -> CNT +=  ramp -> TICK;
	ramp -> CNT = ramp -> CNT >= max_cnt ? max_cnt :ramp -> CNT;	
	ramp -> Output = 1.f * ramp -> CNT / max_cnt;	
	return ramp -> Output;	
}
void Ramp_Reint(Ramp *ramp)
{
	ramp -> CNT=0;
	ramp -> Output=0.0f;		
}
