#include "Driver_Basecotrol.h"

int flag1;
/*       ��������ܺ���      */
void Base_Control(void)
{
    static uint8_t statusTask = 1, status = 0,Initstatus1=0x01;     //��ʻ�����־  ������־  �����ǳ�ʼ����־  ��ʼ��ͣ��־   
     if ((JudgeStop() == 1) && (statusTask <= 2) && Initstatus1==0x02  )            //��������   ��Ϊ����   ��Ϊǰ�����ǲ�ͣ����  ���н׶�
        {
                                                                                         //���ο�����־λ
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
		         if (Y>Second_Y)                         //�Ѿ������һ��
				{
					statusTask = 2;
				}
				break;
			case 2:                    
               ChangeRoute( GetCycle());
				FreeWalk();                       //�����������������
//				 //����Ȧ����ֵ�ж��Ƿ�������һȦ
//            if (GetCycleErr(GetCycle()) == 1)                                      //����Ȧ�����һȦ
//			{
//					statusTask = 3;				}
                        if (GetCycle()==3)                                      //����Ȧ�����һȦ
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
			if (Escape() == 1)                   //�Ѿ��ɹ�����
			{
				status = 0;                             //��ձ�־λ
			}
		}
                        flag1=status;
      }       
