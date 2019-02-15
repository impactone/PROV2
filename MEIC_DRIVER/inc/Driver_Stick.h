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

#define Limit_Speed 0.5            //编码器的轮子速度  卡死标记   
#define dead_time 18                //卡死时间
#define Dead_Up_Angle    90         //卡死退出的上侧目标角度
#define Dead_Down_Angle  -90        //卡死退出的下侧目标角度
#define Dead_Left_Angle   180       //卡死退出的左侧目标角度
#define Dead_Right_Angle  0         //卡死退出的右侧目标角度
#define Back_Distance 400

void Get_relative_position(void) ;
float GetDistance(point_t startPoint);
uint8_t Escape(void);
float GetexpectAngle1(void);
uint8_t JudgeStop(void);
float GetVel(void);
#endif 
