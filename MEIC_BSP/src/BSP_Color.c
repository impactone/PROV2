#include "BSP_Color.h"


void BSP_Color_InitConfig(void)
{
	
	 GPIO_InitTypeDef  GPIO_InitStructure;
  			
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	 
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	 
	 	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;				
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������  
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����	 
	 GPIO_Init(GPIOC, &GPIO_InitStructure);   
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9;				
	 GPIO_Init(GPIOE, &GPIO_InitStructure);   
	
}