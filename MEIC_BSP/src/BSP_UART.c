#include "BSP_UART.h"


union Rec
{
		char RecChar[4];
		float RecFloat;
};
union Rec RecMessage;


u8 USART_RX_BUF[USART_REC_LEN]; 
u16 USART_RX_STA=0;
/**
  * @brief  UART��ʼ��
  * @param  void
  * @retval void
  */

uint8_t Encoder_RX_BUF[ENCODER_LENGTH];
void BSP_UART_InitConfig(void)
{
    USART_InitTypeDef   USART_InitStructure;
    GPIO_InitTypeDef	  GPIO_InitStructure;
	  NVIC_InitTypeDef	  NVIC_InitStructure;
    DMA_InitTypeDef     DMA_InitStructure;
    
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);     
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	    
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	

	 //�˿�����
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	  
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);	  
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);	  
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);	  
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);	 
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);	  
	  GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);	 
    
	  //UART3����
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		GPIO_Init(GPIOB,&GPIO_InitStructure);	 
	  //UART2 ����
		GPIO_InitStructure.GPIO_Pin= GPIO_Pin_3|GPIO_Pin_2;			
		GPIO_Init(GPIOA,&GPIO_InitStructure);	 	
    //UART1 ����
		GPIO_InitStructure.GPIO_Pin= GPIO_Pin_9|GPIO_Pin_10;			
		GPIO_Init(GPIOA,&GPIO_InitStructure);	 		
			
		//UART4����
		GPIO_InitStructure.GPIO_Pin= GPIO_Pin_10|GPIO_Pin_11;		
		GPIO_Init(GPIOC,&GPIO_InitStructure);	
		
		//UART1 Э��
		USART_InitStructure.USART_BaudRate              =   115200;
    USART_InitStructure.USART_HardwareFlowControl   =   USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                  =    USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStructure.USART_Parity                =   USART_Parity_No;
    USART_InitStructure.USART_StopBits              =   USART_StopBits_1;
    USART_InitStructure.USART_WordLength            =   USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);  
		//USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
		//UART2 Э��
		USART_InitStructure.USART_BaudRate              =   9600;
    USART_InitStructure.USART_HardwareFlowControl   =   USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                  =   USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStructure.USART_Parity                =   USART_Parity_No;
    USART_InitStructure.USART_StopBits              =   USART_StopBits_1;
    USART_InitStructure.USART_WordLength            =   USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStructure);  		
    //UART3 Э��
    USART_InitStructure.USART_BaudRate              =   9600;
    USART_InitStructure.USART_HardwareFlowControl   =   USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                  =   USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity                =   USART_Parity_No;
    USART_InitStructure.USART_StopBits              =   USART_StopBits_1;
    USART_InitStructure.USART_WordLength            =   USART_WordLength_8b;
    USART_Init(USART3, &USART_InitStructure);     		
    //UART4 Э��
    USART_InitStructure.USART_BaudRate              =   115200;
    USART_InitStructure.USART_HardwareFlowControl   =   USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                  =   USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity                =   USART_Parity_No;
    USART_InitStructure.USART_StopBits              =   USART_StopBits_1;
    USART_InitStructure.USART_WordLength            =   USART_WordLength_8b;
    USART_Init(UART4, &USART_InitStructure); 
		USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);//ʹ��		
		
		//����3�����ж�
		NVIC_InitStructure.NVIC_IRQChannel						         =	USART3_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelCmd					         =	ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	 =	1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority			     =	1;
	  NVIC_Init(&NVIC_InitStructure);	  		
		NVIC_InitStructure.NVIC_IRQChannel                      = USART2_IRQn;
		NVIC_Init(&NVIC_InitStructure);									
	  NVIC_InitStructure.NVIC_IRQChannel                      = USART1_IRQn;
		NVIC_Init(&NVIC_InitStructure);
		
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���������ж�
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//���������ж�
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�
			 
	 //����4�����ж�,һ��ʱ��û�н��յ����ж�,���������ݽ���
		NVIC_InitStructure.NVIC_IRQChannel						         =	UART4_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelCmd					         =	ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	 =	0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority			     =	1;
	  NVIC_Init(&NVIC_InitStructure);
	  USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
		//��DMA ��֤��ȫ�ԣ����˴��ڿ����ж�
		DMA_DeInit(DMA1_Stream2);		
		while (DMA_GetCmdStatus(DMA1_Stream2) != DISABLE);
    DMA_InitStructure.DMA_Channel                   = DMA_Channel_4;  
    DMA_InitStructure.DMA_PeripheralBaseAddr        = (uint32_t)(&UART4->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr           = (uint32_t)Encoder_RX_BUF;
    DMA_InitStructure.DMA_DIR                       = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize                = ENCODER_LENGTH;
    DMA_InitStructure.DMA_PeripheralInc             = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc                 = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize        = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize            = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode                      = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority                  = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode                  = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold             = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst               = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst           = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream2, &DMA_InitStructure);	
		DMA_Cmd(DMA1_Stream2, ENABLE);			

		//UART4ʹ��
	  USART_Cmd(UART4, ENABLE);			  
    USART_ClearFlag(UART4, USART_FLAG_IDLE);
		//UART2ʹ��
    USART_Cmd(USART2, ENABLE);			  
    USART_ClearFlag(USART2, USART_FLAG_TC);
		
		//UART3ʹ��
    USART_Cmd(USART3, ENABLE);			  
    USART_ClearFlag(USART3, USART_FLAG_TC);
		//UART1ʹ��
	  USART_Cmd(USART1, ENABLE);
		USART_ClearFlag(USART1, USART_FLAG_TC);
}

float ZAngle =0,XAngle =0,YAngle=0 ,X= 0,Y= 0,W= 0;
void Encoder_Decode(void)
{
	    RecMessage.RecChar[0] = Encoder_RX_BUF[2];
			RecMessage.RecChar[1] = Encoder_RX_BUF[3];
			RecMessage.RecChar[2] = Encoder_RX_BUF[4];
			RecMessage.RecChar[3] = Encoder_RX_BUF[5];
			ZAngle = RecMessage.RecFloat;
			
			RecMessage.RecChar[0] = Encoder_RX_BUF[6];
			RecMessage.RecChar[1] = Encoder_RX_BUF[7];
			RecMessage.RecChar[2] = Encoder_RX_BUF[8];
			RecMessage.RecChar[3] = Encoder_RX_BUF[9];
			XAngle = RecMessage.RecFloat;
			
			RecMessage.RecChar[0] = Encoder_RX_BUF[10];
			RecMessage.RecChar[1] = Encoder_RX_BUF[11];
			RecMessage.RecChar[2] = Encoder_RX_BUF[12];
			RecMessage.RecChar[3] = Encoder_RX_BUF[13];
			YAngle = RecMessage.RecFloat;
  
			RecMessage.RecChar[0] = Encoder_RX_BUF[14];
			RecMessage.RecChar[1] = Encoder_RX_BUF[15];
			RecMessage.RecChar[2] = Encoder_RX_BUF[16];
			RecMessage.RecChar[3] = Encoder_RX_BUF[17];
			X = -RecMessage.RecFloat;

			RecMessage.RecChar[0] = Encoder_RX_BUF[18];
			RecMessage.RecChar[1] = Encoder_RX_BUF[19];
			RecMessage.RecChar[2] = Encoder_RX_BUF[20];
			RecMessage.RecChar[3] = Encoder_RX_BUF[21];
			Y = -RecMessage.RecFloat;

			RecMessage.RecChar[0] = Encoder_RX_BUF[22];
			RecMessage.RecChar[1] = Encoder_RX_BUF[23];
			RecMessage.RecChar[2] = Encoder_RX_BUF[24];
			RecMessage.RecChar[3] = Encoder_RX_BUF[25];
			W = RecMessage.RecFloat;		
      
				
}
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

/////////////////////////////////////////////////////////////////////////////////////
//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
//#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
//#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
//#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
//#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

dt_flag_t f;					//��Ҫ�������ݵı�־
u8 data_to_send[50];	//�������ݻ���

/////////////////////////////////////////////////////////////////////////////////////
//Data_Exchange��������������ݷ������󣬱�����ʵ��ÿ5ms����һ�δ�������������λ�������ڴ˺�����ʵ��
//�˺���Ӧ���û�ÿ1ms����һ��
/////////////////////////////////////////////////////////////////////////////////////
//Send_Data������Э�������з������ݹ���ʹ�õ��ķ��ͺ���
//��ֲʱ���û�Ӧ��������Ӧ�õ����������ʹ�õ�ͨ�ŷ�ʽ��ʵ�ִ˺���
void ANO_DT_Send_Data(u8 *dataToSend , u8 length)
{
	u8 i=0;
	while(i<length)
	{
	USART1->DR = (dataToSend[i]&(uint16_t)0x01FF);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	i++;
	}
}
	void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z,s32 bar)
{
	u8 _cnt=0;
	vs16 _temp;
	u8 i=0;
		u8 sum = 0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	_temp = a_x;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	

	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
	u8 _cnt=0;
	vs16 _temp;
	vs32 _temp2 = alt;
		u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(angle_rol*1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*1000);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	data_to_send[_cnt++] = fly_model;
	
	data_to_send[_cnt++] = armed;
	
	data_to_send[3] = _cnt-4;
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_PID(u8 group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	u8 _cnt=0;
	vs16 _temp;
	u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10+group-1;
	data_to_send[_cnt++]=0;
	
	_temp = p1_p * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_d  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_p  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_d * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_p  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_d * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
	sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
  ANO_DT_Send_Data(data_to_send, _cnt);
}
static void ANO_DT_Send_Check(u8 head, u8 check_sum)
{
	u8 i;
	u8 sum = 0;
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;
	

	for(i=0;i<6;i++)
	sum += data_to_send[i];
	data_to_send[6]=sum;

	ANO_DT_Send_Data(data_to_send, 7);
}

/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Prepare������Э��Ԥ����������Э��ĸ�ʽ�����յ������ݽ���һ�θ�ʽ�Խ�������ʽ��ȷ�Ļ��ٽ������ݽ���
//��ֲʱ���˺���Ӧ���û���������ʹ�õ�ͨ�ŷ�ʽ���е��ã����紮��ÿ�յ�һ�ֽ����ݣ�����ô˺���һ��
//�˺������������ϸ�ʽ������֡�󣬻����е������ݽ�������
void ANO_DT_Data_Receive_Prepare(u8 data)
{
	static u8 RxBuffer[50];
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state = 0;

	if(state==0&&data==0xAA)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0xAF)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = data;
		_data_cnt = 0;
	}
	else if(state==4&&_data_len>0)
	{
		_data_len--;
		RxBuffer[4+_data_cnt++]=data;
		if(_data_len==0)
			state = 5;
	}
	else if(state==5)
	{
		state = 0;
		RxBuffer[4+_data_cnt]=data;
		ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
	}
	else
		state = 0;
}
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Anl������Э�����ݽ������������������Ƿ���Э���ʽ��һ������֡���ú��������ȶ�Э�����ݽ���У��
//У��ͨ��������ݽ��н�����ʵ����Ӧ����
//�˺������Բ����û����е��ã��ɺ���Data_Receive_Prepare�Զ�����
void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num)
{
	u8 sum = 0;
	u8 i=0;
	for(i=0;i<(num-1);i++)
	sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		//�ж�sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//�ж�֡ͷ
	if(*(data_buf+2)==0X10)								//PID1
	ANO_DT_Send_Check(*(data_buf+2),sum);
}
void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8)
{
	u8 _cnt=0;
	u8 i=0;
	u8 sum = 0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=BYTE1(m_1);
	data_to_send[_cnt++]=0;	
	data_to_send[_cnt++]=BYTE0(m_1);
	data_to_send[_cnt++]=BYTE1(m_2);
	data_to_send[_cnt++]=BYTE0(m_2);
	data_to_send[_cnt++]=BYTE1(m_3);
	data_to_send[_cnt++]=BYTE0(m_3);
	data_to_send[_cnt++]=BYTE1(m_4);
	data_to_send[_cnt++]=BYTE0(m_4);
	data_to_send[_cnt++]=BYTE1(m_5);
	data_to_send[_cnt++]=BYTE0(m_5);
	data_to_send[_cnt++]=BYTE1(m_6);
	data_to_send[_cnt++]=BYTE0(m_6);
	data_to_send[_cnt++]=BYTE1(m_7);
	data_to_send[_cnt++]=BYTE0(m_7);
	data_to_send[_cnt++]=BYTE1(m_8);
	data_to_send[_cnt++]=BYTE0(m_8);
	data_to_send[3] = _cnt-4;
	for(i=0;i<_cnt;i++)
	sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_RCData(u16 temp)
{
	u8 _cnt=0;
	u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=temp;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=0;
  data_to_send[3] = _cnt-4;
	for(i=0;i<_cnt;i++)
	sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
	}




