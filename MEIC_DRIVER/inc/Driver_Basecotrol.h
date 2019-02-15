#ifndef _DRIVER_BASECONTROL_H
#define _DRIVER_BASECONTROL_H

#include "includes.h"
#include "Driver_Stick.h"
#include "Driver_Delay.h"
#include "Driver_Run.h"
#include "Driver_Correct.h"

#define First_Y 100                 //初始行走距离  开始卡死判断
#define Second_Y 600                //初始行走距离2  开始自由行走

void Base_Control(void);

#endif 
