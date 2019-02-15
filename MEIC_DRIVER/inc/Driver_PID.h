#ifndef Driver_PID_H
#define Driver_PID_H
#include "stm32f4xx.h"
typedef struct
{
	float P;
	float I;
	float D;
	float Imax;
	
	float NowErr;
	float LstErr;
	float SumErr;
	
  float Expect;
  float Measured;
	
	float OutPut;
	float MaxOutput;
} PID_Struct;

void PID_Param_Init(PID_Struct *pid,float p,float i,float d,float imax,float maxoutput);
void PID_Param_Modify(PID_Struct *pid,float p,float i,float d,float imax,float maxoutput);
void PIDStruct_Init(PID_Struct *pid);
void ALL_PID_Init(void);
float  PID_Calc(PID_Struct *pid);
float  PID_Calc_Roate(PID_Struct *pid);
#endif

