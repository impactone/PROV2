#include "Driver_Correct.h"

/*   获取当前角度与目标角度的差值    */
float GetAngleErr(float AngleInit)
{
	float AnglerErr;
	AnglerErr = ZAngle-AngleInit;
	return AnglerErr;
}

/*  矫正动作  */
uint8_t ACtion_STA=0x01,ACtion_STA1=0x01;
void Correct_Action(Action approach )
{
    switch(approach)
    {
        case  Move_Left:                               //左轮抱死，右轮向前              
        CAN_To_Chaise(0,-1800);
        
        break;   
        case  Move_Right:
        CAN_To_Chaise(1800,0);                  //右轮抱死，左轮向前 
        break;               
    }
}

/*              矫正函数           */
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
                         angleinit=ZAngle-Angle_Increace;                        //这个是目标角度 
                         STAX=0x02;
                     }                         
                     Correct_Action(Move_Right);                              //右转 
                     if(GetAngleErr(angleinit) <2 && GetAngleErr(angleinit) >-2 )
                     {
                     c_nt=0;
                     ACtion_STA=0x02; 
                     }
                    break;                    
                 case 0x02:
                   Correct_Action(Move_Left);                                            //左转
                    if(ANGLE_FROM_TO(-3+Aim_Angle,3+Aim_Angle))                                               //回到0度
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
                         angleinit=ZAngle+Angle_Increace;                        //这个是目标角度 
                         STAX=0x02;
                     }                         
                     Correct_Action(Move_Left);                              //右转 
                     if(GetAngleErr(angleinit) <2 && GetAngleErr(angleinit) >-2 )
                     {
                     ACtion_STA1=0x02; 
                     }
                    break;                    
                 case 0x02:
                   Correct_Action(Move_Right);                                            //左转
                   if(ANGLE_FROM_TO(-3+Aim_Angle,3+Aim_Angle))                             //回到0度
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

/*         单方向位置环     */
int16_t VLeft,VRight;
void PID_CoordinateControl(float Xa,float expectAngle,int mode)                                             //X 或Y方向的位置环
{
    int16_t V,W;
    PID_Coordinate.Expect=Xa;
    if(mode==0)                                                                                             //模式0  X方向  模式1  Y方向                                                                                      //X方向位置闭环
    PID_Coordinate.Measured=X;
    else
     PID_Coordinate.Measured=Y;                                                                             //Y方向位置闭环
    PID_Angle.Expect = expectAngle;
	PID_Angle.Measured = ZAngle;
    V=PID_Calc(&PID_Coordinate);                                                                            //坐标的输出
    W=PID_Calc(&PID_Angle);
   	PID_BaseLeft_V.Expect = V-W;
	PID_BaseRight_V.Expect = -V-W;
	VLeft = PID_Calc(&PID_BaseLeft_V);
	VRight = PID_Calc(&PID_BaseRight_V);
	CAN_To_Chaise(VLeft,VRight);
}

/*           前往校准区域           */
//float XX_Expect,YY_Expect;
//uint8_t  Correct_Control(void)
//{
//    static uint8_t statuscorrect = 1,cnt = 0,dead_status=0x00,ACtion_STAtus=0x01,Sta=0x01,STAT=0x01; 
//    uint8_t correctflag=0;
//	static float distance = 0.0f;
//	static point_t startPoint = { 0,0 };
//	//计算行驶距离
//    distance = GetDistance(startPoint);                                             //两个形参的距离
//     switch (statuscorrect)
//	 {
//         case 1:                                                                   //车往前走180度
//         if(!(dead_status &0x02))                                                  //未向前卡死
//         {
//             if(Y>1020 && JudgeStop()==0 )                                          // 车离框的Y的距离较远  且未卡死
//             {
//                 ChasisControl(3000,-3000,180); 
//             }
//             if(Y>1020 && JudgeStop()==1)                                           //车离框Y的距离较远  且已卡死 转换状态
//                 dead_status|=0x02;
//             if(Y<1020)
//                statuscorrect=2; 
//         }
//         else                                                                      //意外卡死
//         {
//             if(ACtion_STAtus==0x01)
//             {
//             CAN_To_Chaise(-1500,1500);  
//             cnt++;
//	         if (cnt == 1)
//              {
//                startPoint.x = X;                                                 //初始时将当前位置存入startpoint
//                startPoint.y= Y;  
//              }                                                    
//              if (distance > 300 && cnt > 1)                                   //直到倒车倒出一段距离后进入下一状态    
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
//                startPoint.x = X;                                                 //初始时将当前位置存入startpoint
//                startPoint.y= Y;  
//                }                                                    
//                if (distance > 300 && cnt > 1)                                   //直到倒车倒出一段距离后进入下一状态    
//                 {
//                   cnt=0;
//                  ACtion_STAtus=0x01;
//                  dead_status&=0xfd;
//                 }
//             }
//         }
//         break;
//         case 2:                                                                 //右行至色块区域
//            if( Sta==0x01)                               //未检测到红色
//            {
//                if(STAT==0x01)
//                 ChasisControl(2000,-2000,-120); 
//                else
//                {
//                 ChasisControl(1200,-1200,-90); 
//                }                    
//                if(  JudgeStop()==1 )                                            //卡死  之后-90度前行
//                {
//                    STAT=0x02; 
//                }       
//            }
//            else
//            {
//                
//                 Sta=0x02;
//                 ChasisControl(500,-500,-90);                                   //降速前行
////                 if(Behindright.color==1)                                           //右后检测到红色              
////                {
////                   XX_Expect=X-100;                                                         //记录当前的X坐标，作为目标坐�
////                   YY_Expect=Y;
////                    Sta=0x01;
////                    statuscorrect=3; 
////                    STAT=0x01;
////                    cnt=0;
////                }    
//            }
//          break;
//         case 3:
//             PID_CoordinateControl(XX_Expect,-90,0);                                           //闭个位置环  X方向
//             if(  (X-XX_Expect)<1  &&  (X-XX_Expect)>-1 && (ZAngle+90)<2  && (ZAngle+90)>-2 )   //已经到了该位置
//               statuscorrect=4; 
//             break;
//         case 4:
//             PID_CoordinateControl(YY_Expect,0,1);                                           //闭个位置环  Y方向
//             if(  (Y-YY_Expect)<1  &&  (Y-YY_Expect)>-1 && (ZAngle)<2  && (ZAngle)>-2 )   //已经到了该位置
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
           PID_CoordinateControl(-95,-90,0);                                           //闭个位置环  X方向
             if(  (X+95)<1  &&  (X+95)>-1 && (ZAngle+90)<2  && (ZAngle+90)>-2 )   //已经到了该位置
               statuscorrect=2;  
             break;
         case 2:
            PID_CoordinateControl(0,0,1);                                           //闭个位置环  Y方向
             if(  (Y-0)<1  &&  (Y-0)>-1 && (ZAngle)<2  && (ZAngle)>-2 )   //已经到了该位置
             {
                 statuscorrect=1;
                 correctflag=1;
             }  
          break; 
         } 
  return correctflag;     
}