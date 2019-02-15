#ifndef __DRIVER_LASER_H
#define __DRIVER_LASER_H

#include "stm32f4xx.h"

#define Ranged GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)

u16 Get_Adc(u8 ch);
float Get_Dis(void);


#endif
