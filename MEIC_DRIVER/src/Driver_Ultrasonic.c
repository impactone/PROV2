#include "Driver_Ultrasonic.h"

uint8_t LeftData[2] = {0,0};
uint8_t RightData[2] = {0,0};
DisType Distance;
void USART2_Process(uint8_t res)
{
  static uint8_t index = 0;
	LeftData[index++] = res;
	if(index >= 2)
	{
		Distance.Left[0] = LeftData[0]<<8|LeftData[1];		 
		index = 0;
	}	
}

void USART3_Process(uint8_t res)
{
	static uint8_t index = 0;
	RightData[index++] = res;	
	if(index >= 2)
	{		
		Distance.Right[0] = RightData[0]<<8|RightData[1];
	  index = 0;
	}
}

void UltraSonic_Trigger(void)
{	
	static uint8_t  number = 1;
	switch(number)
	{	
	  case 1:
			USART_SendData(USART2,0X55);		 
		  //number = 2;
		break;
//	  case 2:
//			USART_SendData(USART3,0X55);
//		  number = 1;
//		break;
	}	
}