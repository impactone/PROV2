#include "BSP_TIME.h"
#include "DefCollect.h"
//f= 84M/arr*psc*
#define psc(Hz,Arr)  (84*1000000/(Hz*(Arr+1))-1)
void  BSP_TIME_InitConfig(void)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD ,ENABLE);

  /*¶æ»ú³õÊ¼»¯*/
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);	 	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);	 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);	 
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	 	  
	
	TIM_TimeBaseStructure.TIM_Period = 199;    
	TIM_TimeBaseStructure.TIM_Prescaler = psc(50,199);
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);		
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse =0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);	
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);			 
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);	
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);	
  
  TIM_ARRPreloadConfig(TIM4,ENABLE);		
	
	TIM_Cmd(TIM4,ENABLE);	
  
//	Angle1 = close1;
//	Angle2 = open2;
//	Angle3 = close3;
//	Angle4 = open4;	

}
