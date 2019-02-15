#include "BSP_PhotoelectricTube.h"

void BSP_PhotoelectricTube_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;   
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}