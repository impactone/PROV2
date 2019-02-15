#include "Driver_PID.h"

PID_Struct PID_Roller_V;
PID_Struct PID_BaseLeft_V;
PID_Struct PID_BaseRight_V;
PID_Struct PID_Angle;
PID_Struct PID_Position;
void ALL_PID_Init(void)
{	
	PIDStruct_Init(&PID_BaseLeft_V);
	PIDStruct_Init(&PID_BaseRight_V);
	PIDStruct_Init(&PID_Roller_V);
	PIDStruct_Init(&PID_Angle);
  PIDStruct_Init(&PID_Position);
	
  PID_Param_Init(&PID_BaseLeft_V,1.5,0.1,0,1160,8000);
	PID_Param_Init(&PID_BaseRight_V,1.5,0.1,0,1160,8000);  
	PID_Param_Init(&PID_Angle,120,0,0,1000,5000); 
	PID_Param_Init(&PID_Position,0,0,0,1000,5000);	
  PID_Param_Init(&PID_Roller_V,0,0,0,1000,5000);	
//	PID_BaseLeft_V.Expect =6000;
	//PID_BaseRight_V.Expect =-6000;
	PID_Angle.Expect = 0;
}
void PIDStruct_Init(PID_Struct *pid)
{
	pid->P          = 0; 
	pid->I          = 0;
	pid->D          = 0;
	pid->Imax       = 0;
	pid->NowErr     = 0;
	pid->LstErr     = 0;
	pid->SumErr     = 0;
	
	pid->Expect     = 0;
	pid->Measured   = 0;
	
	pid->OutPut     = 0;
	pid->MaxOutput  = 0;
}

void PID_Param_Init(PID_Struct *pid,float p,float i,float d,float imax,float maxoutput)
{
	pid -> P = p;
	pid -> I = i;
	pid -> D = d;
	pid -> Imax = imax;
	pid -> MaxOutput = maxoutput;	
}
void PID_Param_Modify(PID_Struct *pid,float p,float i,float d,float imax,float maxoutput)
{
	PID_Param_Init(pid,p,i,d,imax,maxoutput);
}
float  PID_Calc(PID_Struct *pid)
{	
  float Pout=0,Iout=0,Dout=0;
	pid -> NowErr = pid -> Expect - pid -> Measured;	
	pid -> SumErr += (pid ->I==0) ? 0 : pid -> NowErr;	
	//PID 各项输出
	Pout = pid -> P * pid -> NowErr;
	Iout = pid -> I * pid -> SumErr;
	Dout = pid -> D * (pid -> NowErr- pid -> LstErr);
	/* 积分，输出限幅*/
	if(Iout >= pid -> Imax)	
	{
		pid -> SumErr -= pid -> NowErr;
    Iout = pid -> Imax;
	}
	else if(Iout <=- pid -> Imax)	
	{
    pid -> SumErr -= pid -> NowErr; 	  
		Iout = -pid -> Imax;
	}
	pid->OutPut = Pout + Iout + Dout;	
	if (pid->OutPut >= pid->MaxOutput) 
	{
		pid->OutPut = pid->MaxOutput;
	}
	else if (pid->OutPut <= -pid->MaxOutput)
	{
		pid->OutPut = -pid->MaxOutput;
	}	
	/*误差更新*/
	 pid->LstErr = pid->NowErr;
	
	 return pid -> OutPut;
}










