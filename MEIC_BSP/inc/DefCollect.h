#ifndef _DEFCOLLECT_H
#define _DEFCOLLECT_H	 
#include "stm32f4xx.h"
#include "io.h"

#define Switch_Front1   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)
#define Switch_Front2   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)
#define Switch_Front3   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)
#define Switch_Front4   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)

#define Switch_Back1    GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)
#define Switch_Back2    GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)

#define  Cylindef_Left   PAout(5)
#define  Cylindef_Right  PAout(6)

#define Angle1 TIM4->CCR1
#define Angle2 TIM4->CCR2
#define Angle3 TIM4->CCR3
#define Angle4 TIM4->CCR4

#define close1 85
#define open1  150
#define close2 255
#define open2  100
#define close3 220
#define open3  150
#define close4 75
#define open4  222

#define electric GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
#endif



