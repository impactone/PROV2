#include "DefCollect.h"
#include "includes.h"
/*触发开关达到期望时间 单位(ms)*/
u8 TransientTouch_Switch(u8 SwitchType,u16 Expecttime)
{
	OS_ERR err; 
	u8 count = Expecttime/5;	
	if(!SwitchType)
	{
		 OSTimeDly(10,OS_OPT_TIME_DLY,&err);
		 if(!SwitchType)
		 {
			 while(!SwitchType&&count)
			 {
				 OSTimeDly(5,OS_OPT_TIME_DLY,&err);
				 count--;
			 }
			 if(!count) return 1;			 
			 else  return 0;				
		 }
		 return 0;
	}
	return 0;
}





