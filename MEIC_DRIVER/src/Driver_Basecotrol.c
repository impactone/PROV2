#include "Driver_Basecotrol.h"

int flag1;
/*       车体控制总函数      */
void Base_Control(void)
{
    static uint8_t statusTask = 1, status = 0,Initstatus1=0x01;     //行驶步骤标志  卡死标志  陀螺仪初始化标志  初始不停标志   
     if ((JudgeStop() == 1) && (statusTask <= 2) && Initstatus1==0x02  )            //满足条件   视为卡死   因为前两部是不停车的  走行阶段
        {
                                                                                         //二次卡死标志位
            status = 1; 
        }        
          if (status == 0)
		{        
          switch (statusTask)
			{
			case 1:
                ChasisControl(2000,-2000,0);
                if(Y>First_Y)   
                Initstatus1=0x02 ;
		         if (Y>Second_Y)                         //已经走完第一边
				{
					statusTask = 2;
				}
				break;
			case 2:                    
               ChangeRoute( GetCycle());
				FreeWalk();                       //根据条件，驱动电机
//				 //返回圈数差值判断是否走完了一圈
//            if (GetCycleErr(GetCycle()) == 1)                                      //已走圈数相差一圈
//			{
//					statusTask = 3;				}
                        if (GetCycle()==3)                                      //已走圈数相差一圈
			{
					statusTask = 3;				}
				break;
            case 3:
                if(Correct_Control()==1)
                {
               statusTask = 4;            
                }
				break;
               case 4:
              CAN_To_Chaise(0,0);               
				break;

			}
        }
        else if (status == 1)
		{
			if (Escape() == 1)                   //已经成功脱离
			{
				status = 0;                             //清空标志位
			}
		}
                        flag1=status;
      }       
