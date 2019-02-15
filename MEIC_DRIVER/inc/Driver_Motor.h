#ifndef _DRIVER_MOTOR_H
#define _DRIVER_MOTOR_H
#include "Driver_CAN.h"
typedef enum {LDF,LDB,RDF,RDB} Sheet;
void BaseMotorControl(void);
void SheetControl(Sheet whichone,u8 angle);
#endif

