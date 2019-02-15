#include "BSP_CAN.h"

void BSP_CAN_InitConfig(void)
{
	 // CAN设备，中断初始化	
    CAN_InitTypeDef        CAN_InitStruct;
    CAN_FilterInitTypeDef  CAN_FilterInitStruct;
    GPIO_InitTypeDef       GPIO_InitStruct;
    NVIC_InitTypeDef       NVIC_InitStruct;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);      
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); 
    
    GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_12| GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AF;//复用功能
    GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;//推挽输出
    GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_100MHz;//100MHz
    GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
		
    NVIC_InitStruct.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    NVIC_InitStruct.NVIC_IRQChannel = CAN1_TX_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
		
		NVIC_InitStruct.NVIC_IRQChannel                       = CAN2_RX0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority     = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority            = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd                    = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    NVIC_InitStruct.NVIC_IRQChannel                       = CAN2_TX_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority     = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority            = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd                    = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

		CAN_DeInit(CAN1);		
		CAN_DeInit(CAN2);
    CAN_StructInit(&CAN_InitStruct);
		//CAN 通信协议初始化
		CAN_InitStruct.CAN_TTCM        = DISABLE;
    CAN_InitStruct.CAN_ABOM        = DISABLE;    
    CAN_InitStruct.CAN_AWUM        = DISABLE;    
    CAN_InitStruct.CAN_NART        = DISABLE;    
    CAN_InitStruct.CAN_RFLM        = DISABLE;    
    CAN_InitStruct.CAN_TXFP        = ENABLE;     
    CAN_InitStruct.CAN_Mode        = CAN_Mode_Normal; 
    CAN_InitStruct.CAN_SJW         = CAN_SJW_1tq;
    CAN_InitStruct.CAN_BS1         = CAN_BS1_9tq;
    CAN_InitStruct.CAN_BS2         = CAN_BS2_4tq; 
    CAN_InitStruct.CAN_Prescaler   = 3;   
		CAN_Init(CAN1, &CAN_InitStruct);
    CAN_Init(CAN2, &CAN_InitStruct);
		//CAN通信滤波初始化
		CAN_FilterInitStruct.CAN_FilterNumber             = 0;
    CAN_FilterInitStruct.CAN_FilterMode               = CAN_FilterMode_IdMask;
    CAN_FilterInitStruct.CAN_FilterScale              = CAN_FilterScale_32bit;
    CAN_FilterInitStruct.CAN_FilterIdHigh             = 0x0000;
    CAN_FilterInitStruct.CAN_FilterIdLow              = 0x0000;
    CAN_FilterInitStruct.CAN_FilterMaskIdHigh         = 0x0000;
    CAN_FilterInitStruct.CAN_FilterMaskIdLow          = 0x0000;
    CAN_FilterInitStruct.CAN_FilterFIFOAssignment     = 0;
    CAN_FilterInitStruct.CAN_FilterActivation         = ENABLE;
		CAN_FilterInit(&CAN_FilterInitStruct);
		CAN_FilterInitStruct.CAN_FilterNumber             = 14;
    CAN_FilterInit(&CAN_FilterInitStruct);
		//CAN1
		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
		//CAN2接收中断
    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);  //不写CAN2接收中断服务函数，会死在中断里
    CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE); //CAN2发送中断
		
}
