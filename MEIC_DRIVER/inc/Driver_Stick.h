#ifndef _DRIVER_STICK_H
#define _DRIVER_STICK_H

#include "Driver_CAN.h"
#include "Driver_Correct.h"

typedef struct
{
	float x;
	float y;
	float angle;
}point_t;

#define Limit_Speed 0.5            //�������������ٶ�  �������   
#define dead_time 18                //����ʱ��
#define Dead_Up_Angle    90         //�����˳����ϲ�Ŀ��Ƕ�
#define Dead_Down_Angle  -90        //�����˳����²�Ŀ��Ƕ�
#define Dead_Left_Angle   180       //�����˳������Ŀ��Ƕ�
#define Dead_Right_Angle  0         //�����˳����Ҳ�Ŀ��Ƕ�
#define Back_Distance 400

void Get_relative_position(void) ;
float GetDistance(point_t startPoint);
uint8_t Escape(void);
float GetexpectAngle1(void);
uint8_t JudgeStop(void);
float GetVel(void);
#endif 
