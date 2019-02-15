#include "Driver_Correct.h"

/*   ��ȡ��ǰ�Ƕ���Ŀ��ǶȵĲ�ֵ    */
float GetAngleErr(float AngleInit)
{
	float AnglerErr;
	AnglerErr = ZAngle-AngleInit;
	return AnglerErr;
}

/*  ��������  */
uint8_t ACtion_STA=0x01,ACtion_STA1=0x01;
void Correct_Action(Action approach )
{
    switch(approach)
    {
        case  Move_Left:                               //���ֱ�����������ǰ              
        CAN_To_Chaise(0,-1800);
        
        break;   
        case  Move_Right:
        CAN_To_Chaise(1800,0);                  //���ֱ�����������ǰ 
        break;               
    }
}

/*              ��������           */
float AngleIni, angleinit;
uint8_t Action_Correct(Action approach ,uint8_t Angle_Increace ,float Aim_Angle)
{
          static uint8_t c_nt=0,STAX=0x01;  
          uint8_t Correct_Flag=0; 
          if(approach==Move_Right)
          {
           switch (ACtion_STA)
            {
                     case 0x01:  
                     if(STAX==0x01) 
                     {
                         angleinit=ZAngle-Angle_Increace;                        //�����Ŀ��Ƕ� 
                         STAX=0x02;
                     }                         
                     Correct_Action(Move_Right);                              //��ת 
                     if(GetAngleErr(angleinit) <2 && GetAngleErr(angleinit) >-2 )
                     {
                     c_nt=0;
                     ACtion_STA=0x02; 
                     }
                    break;                    
                 case 0x02:
                   Correct_Action(Move_Left);                                            //��ת
                    if(ANGLE_FROM_TO(-3+Aim_Angle,3+Aim_Angle))                                               //�ص�0��
                    {
                    ACtion_STA=0x03;
                    }   
                     break;
                 case 0x03:
                 ChasisControl(0,0,0);
                 ACtion_STA=0x01;
                  Correct_Flag=1;
                 STAX=0x01;
                 break;
                } 
            }
          if(approach==Move_Left)
          {
             switch (ACtion_STA1)
            {
                     case 0x01:                    
                      if(STAX==0x01) 
                     {
                         angleinit=ZAngle+Angle_Increace;                        //�����Ŀ��Ƕ� 
                         STAX=0x02;
                     }                         
                     Correct_Action(Move_Left);                              //��ת 
                     if(GetAngleErr(angleinit) <2 && GetAngleErr(angleinit) >-2 )
                     {
                     ACtion_STA1=0x02; 
                     }
                    break;                    
                 case 0x02:
                   Correct_Action(Move_Right);                                            //��ת
                   if(ANGLE_FROM_TO(-3+Aim_Angle,3+Aim_Angle))                             //�ص�0��
                    {
                    ACtion_STA1=0x03;
                    }   
                     break;
                 case 0x03:
                 ChasisControl(0,0,0);
                 ACtion_STA1=0x01;
                  Correct_Flag=1;
                 STAX=0x01;
                 break;
                } 
           }
          AngleIni=angleinit;
          return   Correct_Flag;           
}  

/*         ������λ�û�     */
int16_t VLeft,VRight;
void PID_CoordinateControl(float Xa,float expectAngle,int mode)                                             //X ��Y�����λ�û�
{
    int16_t V,W;
    PID_Coordinate.Expect=Xa;
    if(mode==0)                                                                                             //ģʽ0  X����  ģʽ1  Y����                                                                                      //X����λ�ñջ�
    PID_Coordinate.Measured=X;
    else
     PID_Coordinate.Measured=Y;                                                                             //Y����λ�ñջ�
    PID_Angle.Expect = expectAngle;
	PID_Angle.Measured = ZAngle;
    V=PID_Calc(&PID_Coordinate);                                                                            //��������
    W=PID_Calc(&PID_Angle);
   	PID_BaseLeft_V.Expect = V-W;
	PID_BaseRight_V.Expect = -V-W;
	VLeft = PID_Calc(&PID_BaseLeft_V);
	VRight = PID_Calc(&PID_BaseRight_V);
	CAN_To_Chaise(VLeft,VRight);
}

/*           ǰ��У׼����           */
//float XX_Expect,YY_Expect;
//uint8_t  Correct_Control(void)
//{
//    static uint8_t statuscorrect = 1,cnt = 0,dead_status=0x00,ACtion_STAtus=0x01,Sta=0x01,STAT=0x01; 
//    uint8_t correctflag=0;
//	static float distance = 0.0f;
//	static point_t startPoint = { 0,0 };
//	//������ʻ����
//    distance = GetDistance(startPoint);                                             //�����βεľ���
//     switch (statuscorrect)
//	 {
//         case 1:                                                                   //����ǰ��180��
//         if(!(dead_status &0x02))                                                  //δ��ǰ����
//         {
//             if(Y>1020 && JudgeStop()==0 )                                          // ������Y�ľ����Զ  ��δ����
//             {
//                 ChasisControl(3000,-3000,180); 
//             }
//             if(Y>1020 && JudgeStop()==1)                                           //�����Y�ľ����Զ  ���ѿ��� ת��״̬
//                 dead_status|=0x02;
//             if(Y<1020)
//                statuscorrect=2; 
//         }
//         else                                                                      //���⿨��
//         {
//             if(ACtion_STAtus==0x01)
//             {
//             CAN_To_Chaise(-1500,1500);  
//             cnt++;
//	         if (cnt == 1)
//              {
//                startPoint.x = X;                                                 //��ʼʱ����ǰλ�ô���startpoint
//                startPoint.y= Y;  
//              }                                                    
//              if (distance > 300 && cnt > 1)                                   //ֱ����������һ�ξ���������һ״̬    
//              {
//                   cnt=0;
//                  ACtion_STAtus=0x02;
//               }
//              }
//             else
//             {
//                 ChasisControl(3000,-3000,-110);  
//                 cnt++;
//	             if (cnt == 1)
//                {
//                startPoint.x = X;                                                 //��ʼʱ����ǰλ�ô���startpoint
//                startPoint.y= Y;  
//                }                                                    
//                if (distance > 300 && cnt > 1)                                   //ֱ����������һ�ξ���������һ״̬    
//                 {
//                   cnt=0;
//                  ACtion_STAtus=0x01;
//                  dead_status&=0xfd;
//                 }
//             }
//         }
//         break;
//         case 2:                                                                 //������ɫ������
//            if( Sta==0x01)                               //δ��⵽��ɫ
//            {
//                if(STAT==0x01)
//                 ChasisControl(2000,-2000,-120); 
//                else
//                {
//                 ChasisControl(1200,-1200,-90); 
//                }                    
//                if(  JudgeStop()==1 )                                            //����  ֮��-90��ǰ��
//                {
//                    STAT=0x02; 
//                }       
//            }
//            else
//            {
//                
//                 Sta=0x02;
//                 ChasisControl(500,-500,-90);                                   //����ǰ��
////                 if(Behindright.color==1)                                           //�Һ��⵽��ɫ              
////                {
////                   XX_Expect=X-100;                                                         //��¼��ǰ��X���꣬��ΪĿ�����
////                   YY_Expect=Y;
////                    Sta=0x01;
////                    statuscorrect=3; 
////                    STAT=0x01;
////                    cnt=0;
////                }    
//            }
//          break;
//         case 3:
//             PID_CoordinateControl(XX_Expect,-90,0);                                           //�ո�λ�û�  X����
//             if(  (X-XX_Expect)<1  &&  (X-XX_Expect)>-1 && (ZAngle+90)<2  && (ZAngle+90)>-2 )   //�Ѿ����˸�λ��
//               statuscorrect=4; 
//             break;
//         case 4:
//             PID_CoordinateControl(YY_Expect,0,1);                                           //�ո�λ�û�  Y����
//             if(  (Y-YY_Expect)<1  &&  (Y-YY_Expect)>-1 && (ZAngle)<2  && (ZAngle)>-2 )   //�Ѿ����˸�λ��
//             {
//                 statuscorrect=1;
//                 correctflag=1;
//             }
//          break;    
//} 
//     return correctflag; 
//}

uint8_t  Correct_Control(void)
{
        static uint8_t statuscorrect = 1,cnt = 0,dead_status=0x00,ACtion_STAtus=0x01,Sta=0x01,STAT=0x01; 
      uint8_t correctflag=0;
         switch (statuscorrect)
	 {
             case 1:
           PID_CoordinateControl(-95,-90,0);                                           //�ո�λ�û�  X����
             if(  (X+95)<1  &&  (X+95)>-1 && (ZAngle+90)<2  && (ZAngle+90)>-2 )   //�Ѿ����˸�λ��
               statuscorrect=2;  
             break;
         case 2:
            PID_CoordinateControl(0,0,1);                                           //�ո�λ�û�  Y����
             if(  (Y-0)<1  &&  (Y-0)>-1 && (ZAngle)<2  && (ZAngle)>-2 )   //�Ѿ����˸�λ��
             {
                 statuscorrect=1;
                 correctflag=1;
             }  
          break; 
         } 
  return correctflag;     
}