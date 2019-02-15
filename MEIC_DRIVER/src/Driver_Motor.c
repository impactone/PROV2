#include "Driver_Motor.h"
extern PID_Struct PID_BaseLeft_V;
extern PID_Struct PID_BaseRight_V;
extern PID_Struct PID_Roller_V;

extern int16_t leftinput;
extern int16_t rightinput;
/*底盘电机控制*/    

void BaseMotorControl (void)
{     	 		
	 leftinput  = PID_Calc(&PID_BaseLeft_V);		 
	 rightinput = PID_Calc(&PID_BaseRight_V);  
//   CAN_To_Chaise(leftinput,rightinput,0);		 			
}
//d(0,40)  合 d(0,90) 开
//d(1,180)  合   d(1,40) 开
//d(2,170)合        d(2,100) 开
//d(3,20)  合  d(3,160) 开
void SheetControl(Sheet whichone,u8 angle)
{
	  switch(whichone)
		{
			case LDF:
				TIM_SetCompare1(TIM4,angle/9+5);
			break;
			case LDB:
				TIM_SetCompare2(TIM4,angle/9+5);
			break;
			case RDF:
				TIM_SetCompare3(TIM4,angle/9+5);
			break;
			case RDB:
				TIM_SetCompare4(TIM4,angle/9+5);
			break;			
		}
} 