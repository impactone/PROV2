#include "Driver_Correct.h"

/*   »ñÈ¡µ±Ç°½Ç¶ÈÓëÄ¿±ê½Ç¶ÈµÄ²îÖµ    */
float GetAngleErr(float AngleInit)
{
	float AnglerErr;
	AnglerErr = ZAngle-AngleInit;
	return AnglerErr;
}

/*  ½ÃÕý¶¯×÷  */
uint8_t ACtion_STA=0x01,ACtion_STA1=0x01;
void Correct_Action(Action approach )
{
    switch(approach)
    {
        case  Move_Left:                               //×óÂÖ±§ËÀ£¬ÓÒÂÖÏòÇ°              
        CAN_To_Chaise(0,-1800);
        
        break;   
        case  Move_Right:
        CAN_To_Chaise(1800,0);                  //ÓÒÂÖ±§ËÀ£¬×óÂÖÏòÇ° 
        break;               
    }
}

/*              ½ÃÕýº¯Êý           */
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
                         angleinit=ZAngle-Angle_Increace;                        //Õâ¸öÊÇÄ¿±ê½Ç¶È 
                         STAX=0x02;
                     }                         
                     Correct_Action(Move_Right);                              //ÓÒ×ª 
                     if(GetAngleErr(angleinit) <2 && GetAngleErr(angleinit) >-2 )
                     {
                     c_nt=0;
                     ACtion_STA=0x02; 
                     }
                    break;                    
                 case 0x02:
                   Correct_Action(Move_Left);                                            //×ó×ª
                    if(ANGLE_FROM_TO(-3+Aim_Angle,3+Aim_Angle))                                               //»Øµ½0¶È
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
                         angleinit=ZAngle+Angle_Increace;                        //Õâ¸öÊÇÄ¿±ê½Ç¶È 
                         STAX=0x02;
                     }                         
                     Correct_Action(Move_Left);                              //ÓÒ×ª 
                     if(GetAngleErr(angleinit) <2 && GetAngleErr(angleinit) >-2 )
                     {
                     ACtion_STA1=0x02; 
                     }
                    break;                    
                 case 0x02:
                   Correct_Action(Move_Right);                                            //×ó×ª
                   if(ANGLE_FROM_TO(-3+Aim_Angle,3+Aim_Angle))                             //»Øµ½0¶È
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

/*         µ¥·½ÏòÎ»ÖÃ»·     */
int16_t VLeft,VRight;
void PID_CoordinateControl(float Xa,float expectAngle,int mode)                                             //X »òY·½ÏòµÄÎ»ÖÃ»·
{
    int16_t V,W;
    PID_Coordinate.Expect=Xa;
    if(mode==0)                                                                                             //Ä£Ê½0  X·½Ïò  Ä£Ê½1  Y·½Ïò                                                                                      //X·½ÏòÎ»ÖÃ±Õ»·
    PID_Coordinate.Measured=X;
    else
     PID_Coordinate.Measured=Y;                                                                             //Y·½ÏòÎ»ÖÃ±Õ»·
    PID_Angle.Expect = expectAngle;
	PID_Angle.Measured = ZAngle;
    V=PID_Calc(&PID_Coordinate);                                                                            //×ø±êµÄÊä³ö
    W=PID_Calc(&PID_Angle);
   	PID_BaseLeft_V.Expect = V-W;
	PID_BaseRight_V.Expect = -V-W;
	VLeft = PID_Calc(&PID_BaseLeft_V);
	VRight = PID_Calc(&PID_BaseRight_V);
	CAN_To_Chaise(VLeft,VRight);
}

/*           Ç°ÍùÐ£×¼ÇøÓò           */
//float XX_Expect,YY_Expect;
//uint8_t  Correct_Control(void)
//{
//    static uint8_t statuscorrect = 1,cnt = 0,dead_status=0x00,ACtion_STAtus=0x01,Sta=0x01,STAT=0x01; 
//    uint8_t correctflag=0;
//	static float distance = 0.0f;
//	static point_t startPoint = { 0,0 };
//	//¼ÆËãÐÐÊ»¾àÀë
//    distance = GetDistance(startPoint);                                             //Á½¸öÐÎ²ÎµÄ¾àÀë
//     switch (statuscorrect)
//	 {
//         case 1:                                                                   //³µÍùÇ°×ß180¶È
//         if(!(dead_status &0x02))                                                  //Î´ÏòÇ°¿¨ËÀ
//         {
//             if(Y>1020 && JudgeStop()==0 )                                          // ³µÀë¿òµÄYµÄ¾àÀë½ÏÔ¶  ÇÒÎ´¿¨ËÀ
//             {
//                 ChasisControl(3000,-3000,180); 
//             }
//             if(Y>1020 && JudgeStop()==1)                                           //³µÀë¿òYµÄ¾àÀë½ÏÔ¶  ÇÒÒÑ¿¨ËÀ ×ª»»×´Ì¬
//                 dead_status|=0x02;
//             if(Y<1020)
//                statuscorrect=2; 
//         }
//         else                                                                      //ÒâÍâ¿¨ËÀ
//         {
//             if(ACtion_STAtus==0x01)
//             {
//             CAN_To_Chaise(-1500,1500);  
//             cnt++;
//	         if (cnt == 1)
//              {
//                startPoint.x = X;                                                 //³õÊ¼Ê±½«µ±Ç°Î»ÖÃ´æÈëstartpoint
//                startPoint.y= Y;  
//              }                                                    
//              if (distance > 300 && cnt > 1)                                   //Ö±µ½µ¹³µµ¹³öÒ»¶Î¾àÀëºó½øÈëÏÂÒ»×´Ì¬    
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
//                startPoint.x = X;                                                 //³õÊ¼Ê±½«µ±Ç°Î»ÖÃ´æÈëstartpoint
//                startPoint.y= Y;  
//                }                                                    
//                if (distance > 300 && cnt > 1)                                   //Ö±µ½µ¹³µµ¹³öÒ»¶Î¾àÀëºó½øÈëÏÂÒ»×´Ì¬    
//                 {
//                   cnt=0;
//                  ACtion_STAtus=0x01;
//                  dead_status&=0xfd;
//                 }
//             }
//         }
//         break;
//         case 2:                                                                 //ÓÒÐÐÖÁÉ«¿éÇøÓò
//            if( Sta==0x01)                               //Î´¼ì²âµ½ºìÉ«
//            {
//                if(STAT==0x01)
//                 ChasisControl(2000,-2000,-120); 
//                else
//                {
//                 ChasisControl(1200,-1200,-90); 
//                }                    
//                if(  JudgeStop()==1 )                                            //¿¨ËÀ  Ö®ºó-90¶ÈÇ°ÐÐ
//                {
//                    STAT=0x02; 
//                }       
//            }
//            else
//            {
//                
//                 Sta=0x02;
//                 ChasisControl(500,-500,-90);                                   //½µËÙÇ°ÐÐ
////                 if(Behindright.color==1)                                           //ÓÒºó¼ì²âµ½ºìÉ«              
////                {
////                   XX_Expect=X-100;                                                         //¼ÇÂ¼µ±Ç°µÄX×ø±ê£¬×÷ÎªÄ¿±ê×ø±
////                   YY_Expect=Y;
////                    Sta=0x01;
////                    statuscorrect=3; 
////                    STAT=0x01;
////                    cnt=0;
////                }    
//            }
//          break;
//         case 3:
//             PID_CoordinateControl(XX_Expect,-90,0);                                           //±Õ¸öÎ»ÖÃ»·  X·½Ïò
//             if(  (X-XX_Expect)<1  &&  (X-XX_Expect)>-1 && (ZAngle+90)<2  && (ZAngle+90)>-2 )   //ÒÑ¾­µ½ÁË¸ÃÎ»ÖÃ
//               statuscorrect=4; 
//             break;
//         case 4:
//             PID_CoordinateControl(YY_Expect,0,1);                                           //±Õ¸öÎ»ÖÃ»·  Y·½Ïò
//             if(  (Y-YY_Expect)<1  &&  (Y-YY_Expect)>-1 && (ZAngle)<2  && (ZAngle)>-2 )   //ÒÑ¾­µ½ÁË¸ÃÎ»ÖÃ
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
           PID_CoordinateControl(-95,-90,0);                                           //±Õ¸öÎ»ÖÃ»·  X·½Ïò
             if(  (X+95)<1  &&  (X+95)>-1 && (ZAngle+90)<2  && (ZAngle+90)>-2 )   //ÒÑ¾­µ½ÁË¸ÃÎ»ÖÃ
               statuscorrect=2;  
             break;
         case 2:
            PID_CoordinateControl(0,0,1);                                           //±Õ¸öÎ»ÖÃ»·  Y·½Ïò
             if(  (Y-0)<1  &&  (Y-0)>-1 && (ZAngle)<2  && (ZAngle)>-2 )   //ÒÑ¾­µ½ÁË¸ÃÎ»ÖÃ
             {
                 statuscorrect=1;
                 correctflag=1;
             }  
          break; 
         } 
  return correctflag;     
}