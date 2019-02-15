#ifndef __DRIVER_CAN_H
#define __DRIVER_CAN_H

#include "stm32f4xx.h"
#include "includes.h"
#include "Driver_PID.h"
#define RM3510_MOTOR1ID 0X201
#define RM3510_MOTOR2ID 0X202
#define RM3510_MOTOR3ID 0X203
#define RM3510_MOTOR4ID 0X204
#define extra 0x151
void Turntable(u8 pos);//×ªÅÌ·¢ËÍ
void CAN_To_Cloud(int16_t pitch,int16_t yaw);
void CAN_To_Chaise(int16_t BaseLeft, int16_t BaseRight,int16_t Roller);
void  CAN2_Process(CanRxMsg rm);
void  CAN1_Process(CanRxMsg rm);
#endif




