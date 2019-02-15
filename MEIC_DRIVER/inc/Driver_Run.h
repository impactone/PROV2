#ifndef _DRIVER_RUN_H
#define _DRIVER_RUN_H

#include "Driver_CAN.h"
#define RANGE_OF_XY(a,b,c,d)				(X<b&&X>a&&Y<d&&Y>c)    //    X��a��b   Y�� c��d
#define Brand_Y_Min   1500          //��������Y��Сֵ
#define Brand_Y_Max   1600          //��������Y���ֵ
#define Clean_Brand_Y_Min  1500     //����������Y��Сֵ
#define Clean_Brand_Y_Max  1600     //����������Y���ֵ
#define Brand_X  10                 //�������X��ֵ
#define Left_ExpectAngle   180      //�������Ŀ��Ƕ�
#define Right_ExpectAngle  0        //�����Ҳ�Ŀ��Ƕ�    
#define UP_ExpectAngle    90        //�����ϲ�Ŀ��Ƕ�
#define Down_ExpectAngle  -90       //�����²�Ŀ��Ƕ�

typedef struct
{
	int Bound_UP_X;
	int Bound_UP_Y;
	int Bound_Down_X;
	int Bound_Down_Y;
}WalkRoute_t;

extern WalkRoute_t  Route;
extern uint8_t CycleNumber;
extern int16_t  V_RUN;

void ChangeRoute(int num);
uint8_t GetCycle(void);
int16_t GetCycleErr(int16_t circle);
void FreeWalk(void);
#endif 
