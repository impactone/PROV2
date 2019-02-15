#include "Driver_Stick.h"

/*          »ñÈ¡µ±Ç°¾àÀëÏà¶Ô³õÊ¼Î»ÖÃ        */
float GetDistance(point_t startPoint)
{
	float distance = 0.0f;
	distance = sqrt((X- startPoint.x)*(X- startPoint.x) + (Y- startPoint.y)*(Y- startPoint.y));
	return distance;
}

  /*         ÅĞ¶ÏËÙ¶ÈÊÇ·ñÎªÁã     */ 
 float VEL;     
float GetVel(void)
{
	static float lastX = 0.0f, lastY = 0.0f;
	static  float velX = 0.0f, velY = 0.0f, vel = 0.0f;                            
	velX = X- lastX;
	velY = Y- lastY;
	vel = sqrtf(velX*velX + velY*velY);                             //½«¾àÀë×ª»¯ÎªËÙ¶È    mm/s
	lastX = X;
	lastY =Y;
    VEL=vel;
	return vel;
}


/*          ÅĞ¶Ï¿¨ËÀ×´Ì¬      */
int ISStop;
uint8_t JudgeStop(void)
{
	static int cnt = 0, isStop = 0;

	if (fabs(GetVel())<Limit_Speed)                                 //ËÙ¶ÈĞ¡ÓÚ0.05
	{
		cnt++;
	}
	else
	{
		cnt = 0;
	}
                                                                    //ËÙ¶ÈµÈÓÚ0µÄÊ±¼ä´óÓÚµÈÓÚdead_timeÊ±ÈÏÎª»úÆ÷ÈËÍ£×¡
	if (cnt >= dead_time)
		isStop = 1;
	else
		isStop = 0;
    ISStop=isStop;
	return isStop;
}

/*          »ñÈ¡¿¨ËÀÊ±½ÃÕıÔËĞĞÊ±µÄ½Ç¶È     */
float GetexpectAngle1(void)                           
{
   	float expectAngle = 0.0f;
     
    //³¡µØÏÂ²àÇøÓòÆÚÍû½Ç¶ÈÎª-90                     
	 if ( X>-2400 && X<50 && Y>0 && Y<1400)  
    {
      expectAngle = Dead_Down_Angle;               
    }
    
     //³¡µØÓÒ²àÇøÓòÆÚÍû½Ç¶ÈÎª0
	if (X>50 && X<2400 && Y>0 && Y<3000)             
	{
      expectAngle = Dead_Right_Angle;
    } 
    
    //³¡µØ¶¥²¿ÇøÓòÆÚÍû½Ç¶ÈÎª90
	if (X>-50 && X<2400 && Y>3000 && Y<4500)
	{
      expectAngle = Dead_Up_Angle;
    }  
    
     //³¡µØ×ó²àÇøÓòÆÚÍû½Ç¶ÈÎª180
    if (X>-2400 && X<-50 &&  Y>1400 && Y<4500)          
	{   
      expectAngle = Dead_Left_Angle;
    }
	return expectAngle;
}

/*  »ñÈ¡³µÉíµ±Ç°ÓëÖĞÏßÏà¶ÔÎ»ÖÃ   */
int Axis_Sign;                        
void Get_relative_position(void)                         
{                        
	 if ( X>-2400 && X<50 && Y>0 && Y<1500)  
    {
        if(Y>712 )  
        {
            Axis_Sign=1;                                  //³µÔÚÈüµÀ×ó²à
        }
        else      
        {
            Axis_Sign=-1;                                 //³µÔÚÈüµÀÓÒ²à
        }
    }
    
	if (X>50 && X<2400 && Y>0 && Y<3000)                  //³¡µØÓÒ²àÇøÓò
	{
        if(X>1600) 
        {
            Axis_Sign=-1;
        }
        else     
        {
            Axis_Sign=1;
        }
    }     
    
	if (X>-50 && X<2400 && Y>3000 && Y<4500)               //ÉÏ²àÇøÓò 
	{
        if(Y>3200) 
        {
            Axis_Sign=-1;
        }
        else    
        {
            Axis_Sign=1;
        }
    } 
    
    if (X>-2400 && X<-50 &&  Y>1500 && Y<4500)             //×ó²àÇøÓò     
	{   
         if(X>-1450) 
         {
             Axis_Sign=1;
         }
         else   
         {
             Axis_Sign=-1;
         }
    }
}

/*          ¿¨ËÀÌÓÒİ³ÌĞò         */
uint8_t STEP;
uint8_t Escape(void)
{
    static short step = 1, cnt = 0,Compare_STA=0x01;
	char complete_flag = 0;
	static float  distance = 0.0f, expectangle1 = 0.0f,mark_expectangle;
	static point_t startPoint = { 0,0 };
    
	expectangle1 = GetexpectAngle1();                            //¸ù¾İ×ø±êÈ·¶¨Ä¿±ê½Ç¶È  
    Get_relative_position();    
	distance = GetDistance(startPoint);                             //»ñÈ¡Á½µãµÄ¾àÀë
    
	switch (step)
	{
	case 1:                                                         //¾ÍÁ½¸ö×´Ì¬  2ºÍ3
		
        CAN_To_Chaise(-1150,1180);                            //µ¥´¿µÄ»ØÍË
		cnt++;
		if (Compare_STA==0x01)
		{
            startPoint.x = X;                                        //³õÊ¼Ê±½«µ±Ç°Î»ÖÃ´æÈëstartpoint
            startPoint.y= Y; 
            Compare_STA=0x02;
        }
                                                                     //Ò»¶ÎÊ±¼äµ¹³µµÄ¾àÀëºÜĞ¡£¬¼ÓËÙÏòºóµ¹³µ
		if (cnt >500 && distance < Back_Distance)                    //Ê±¼äµ½ÁË100ms£¬µ¹³µ¾àÀë²»¹» Back_Distance ½øÈëµÚÈı²½
		{
			step = 5;                                                //µ¹³µ¾àÀë¹ı¶Ì£¬½øÈëµÚÈı²½£¬Í¬Ê±Çå¿Õ±êÖ¾Î
            cnt = 0;
            Compare_STA=0x01;                                        //Ìø³ö¸Ã²½£¬±êÖ¾Î»ËæÖ®¹éÎ»
		}
        
		if (distance > Back_Distance && cnt > 1)                     // Èç¹ûµ¹³µ¾àÀë¹»³¤£¬½øÈëÏÂÒ»×´Ì¬    Ò»¶¨ÒªÇå¿Õ¼ÆÊı
		{
			step = 2;
			cnt = 0;
            Compare_STA=0x01;
		}
		break;
    case 2:                                                           //Õâ¸öÖ»Ö´ĞĞÒ»±é
       if( Axis_Sign==1)                                              //³µÔÚÈüµÀµÄ×ó²à£¬ÏòÓÒÅ¤¶¯µ÷Õû
       {
           step = 3; 
           mark_expectangle=expectangle1;                            //ÒÑºóÍË³É¹¦£¬¼ÇÂ¼´ËÎ»ÖÃµÄ½Ç¶È
       }
       if( Axis_Sign==-1)
       {
           step = 4; 
           mark_expectangle=expectangle1;                            //ÒÑºóÍË³É¹¦£¬¼ÇÂ¼´ËÎ»ÖÃµÄ½Ç¶È
       }           
    break ;
    case 3:                                                           //³µÔÚÈüµÀ×ó²à£¬½ÃÕı¹ı³Ì
        cnt++;
        if(Action_Correct(Move_Right,20,mark_expectangle)==1)                          //ÏòÓÒÅ¤¶¯³É¹¦
        {
            cnt = 0;
			step = 1;                                                  //·½±ãÏÂ´Î½øÈë£¬
			complete_flag = 1;  
        }
        else if(cnt>800)
        {
            step=5;
            cnt=0;
        }
        break;
    case 4:                                                           //³µÔÚÈüµÀÓÒ²à£¬½øĞĞ½ÃÕı
        cnt++;
        if(Action_Correct(Move_Left,20,mark_expectangle)==1)                          //Ïò×óÅ¤¶¯³É¹¦
        {
            cnt = 0;
			step = 1;                                                  //·½±ãÏÂ´Î½øÈë£¬
			complete_flag = 1;  
        }
        else if(cnt>800)                                               //³¤Ê±¼äÎŞ·¨½ÃÕı³É¹¦
        {
            step=5;
            cnt=0;
        }
        break;
    case 5:
        CAN_To_Chaise(-1500,1500);                              //¿¨ËÀ×´Ì¬£¬¾Ş´óËÙ¶Èµ¹ÍË
		cnt++;
		if (Compare_STA==0x01)
        {
           startPoint.x = X;                                           //³õÊ¼Ê±½«µ±Ç°Î»ÖÃ´æÈëstartpoint
            startPoint.y= Y;  
            Compare_STA=0x02;
        }                                                          
		if (distance > 350 && cnt > 1)                            //Ö±µ½µ¹³µµ¹³öÒ»¶Î¾àÀëºó½øÈëÏÂÒ»×´Ì¬    
		{
			step = 2;
			cnt = 0;
            Compare_STA=0x01;                                    //ÒÑÍË³ö¿¨ËÀ×´Ì¬£¬±êÖ¾Î»¹éÎ»
		}
        if(cnt>500)                                               //Ïòºóµ¹³µµÄÊ±¼ä¹ı³¤£¬ÒÑ±»¿¨ËÀ£¬ÂúËÙÏòÇ°³å
        {
            step=6;
            cnt=0;
            Compare_STA=0x01;
        }
		break;
    case 6:
         ChasisControl(3000,-3000,expectangle1); 
         cnt++;
         if (cnt == 1)
        {
           startPoint.x = X;                                           //³õÊ¼Ê±½«µ±Ç°Î»ÖÃ´æÈëstartpoint
           startPoint.y= Y;  
        } 
        if (distance > 350 && cnt > 1)                                //ÒÑ¾­³å³ö30cm   ÍË³ö¿¨ËÀ
		{
			step = 1;
			cnt = 0;
            complete_flag=1;
		}
        break;
    }        
     STEP=step;
    return complete_flag;
}  
