#ifndef _DRIVER_CORRECT_H
#define _DRIVER_CORRECT_H
#include "includes.h"
#include "Driver_CAN.h"
#include "Driver_Stick.h"
#include "Driver_Colorlook.h"

extern float ZAngle;
extern uint8_t ACtion_STA,ACtion_STA1;

#define ANGLE_FROM_TO(a,b)                  (ZAngle>a && ZAngle<b)  //   角度在a到b之间
typedef enum                        //声明一个枚举类型 Action       
{
Move_Left,
Move_Right,
}Action;


uint8_t Correct_Control(void);
float GetAngleErr(float AngleInit);
void Correct_Action(Action approach );
uint8_t Action_Correct(Action approach ,uint8_t Angle_Increace ,float Aim_Angle);
void PID_CoordinateControl(float Xa,float expectAngle,int mode);
#endif 
