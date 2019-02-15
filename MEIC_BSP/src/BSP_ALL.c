#include "BSP_ALL.h"

void BSP_ALL_Init(void)
{
	BSP_NVIC_InitConfig();
	BSP_TIME_InitConfig();
	BSP_UART_InitConfig();		
	BSP_CAN_InitConfig(); 
  BSP_Color_InitConfig();
  BSP_EXTI_InitConfig();
	BSP_Cylinder_Init();	
	BSP_PhotoelectricTube_Init();
	BSP_Laser_Init();
	BSP_Swich_Init();	
  usmart_dev.init(84);
}