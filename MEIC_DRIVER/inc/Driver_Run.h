#ifndef _DRIVER_RUN_H
#define _DRIVER_RUN_H

#include "Driver_CAN.h"
#define RANGE_OF_XY(a,b,c,d)				(X<b&&X>a&&Y<d&&Y>c)    //    X是a到b   Y是 c到d
#define Brand_Y_Min   1500          //标记区域的Y最小值
#define Brand_Y_Max   1600          //标记区域的Y最大值
#define Clean_Brand_Y_Min  1500     //清除标记区域Y最小值
#define Clean_Brand_Y_Max  1600     //清除标记区域Y最大值
#define Brand_X  10                 //标记区域X的值
#define Left_ExpectAngle   180      //走形左侧目标角度
#define Right_ExpectAngle  0        //走形右侧目标角度    
#define UP_ExpectAngle    90        //走形上侧目标角度
#define Down_ExpectAngle  -90       //走形下侧目标角度

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
