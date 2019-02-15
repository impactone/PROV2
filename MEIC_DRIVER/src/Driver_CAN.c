#include "Driver_CAN.h"
extern OS_Q  CanMsgQue;
extern PID_Struct PID_BaseLeft_V;
extern PID_Struct PID_BaseRight_V;
extern PID_Struct PID_Roller_V;
#define h8b(x) (x>>8)
#define l8b(x) ((x)&0xff)
#define MAX_VOL 70//转盘转速
#define V_Roll -1600
void Turntable(u8 pos)//转盘发送
{
	OS_ERR   err;
	CanTxMsg TxMsg;
	TxMsg.StdId = 0x101;     //
	TxMsg.IDE = CAN_Id_Standard;
	TxMsg.RTR = CAN_RTR_Data;
	TxMsg.DLC = 0x08;
	TxMsg.Data[0] = pos;
	TxMsg.Data[1] = MAX_VOL;
	TxMsg.Data[2] = 0;
	TxMsg.Data[3] = 0;
	TxMsg.Data[4] = 0;
	TxMsg.Data[5] = 0;
	TxMsg.Data[6] = 0;
	TxMsg.Data[7] = 0;	
	
	OSQPost(&CanMsgQue,CAN2,sizeof(*CAN2),OS_OPT_POST_FIFO,&err);
	OSQPost(&CanMsgQue,&TxMsg,sizeof(TxMsg),OS_OPT_POST_FIFO,&err);	

}
void CAN_To_Chaise(int16_t BaseLeft, int16_t BaseRight,int16_t Roller)
{
		OS_ERR   err;	  
		static CanTxMsg TxMsg;
		TxMsg.StdId = 0x200;
		TxMsg.IDE = CAN_Id_Standard;
		TxMsg.RTR = CAN_RTR_Data;
		TxMsg.DLC = 0x08;
		TxMsg.Data[0] = h8b(BaseLeft);
		TxMsg.Data[1] = l8b(BaseLeft);
		TxMsg.Data[2] = h8b(BaseRight);
		TxMsg.Data[3] = l8b(BaseRight);
		TxMsg.Data[4] = h8b(Roller);
		TxMsg.Data[5] = l8b(Roller);
		TxMsg.Data[6] =  0;
		TxMsg.Data[7] =  0;
	 	OSQPost(&CanMsgQue,CAN1,sizeof(*CAN1),OS_OPT_POST_FIFO,&err);  
	  OSQPost(&CanMsgQue,&TxMsg,sizeof(TxMsg),OS_OPT_POST_FIFO,&err);
}
void  CAN2_Process(CanRxMsg RxMsg)
{	
	
}
void  CAN1_Process(CanRxMsg RxMsg)
{
  switch (RxMsg.StdId)
	{
		case RM3510_MOTOR1ID:		
        PID_BaseLeft_V.Measured  = (int16_t)(RxMsg.Data[2]<<8 | RxMsg.Data[3]);			
			break; 		
		case RM3510_MOTOR2ID:
			  PID_BaseRight_V.Measured = (int16_t)(RxMsg.Data[2]<<8 | RxMsg.Data[3]);			
			break;				 
    case RM3510_MOTOR3ID:
			  PID_Roller_V.Measured = (int16_t)(RxMsg.Data[2]<<8 | RxMsg.Data[3]);			
		default:			
			break;
	}
}
