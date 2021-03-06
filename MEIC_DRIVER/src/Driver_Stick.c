#include "Driver_Stick.h"

/*          获取当前距离相对初始位置        */
float GetDistance(point_t startPoint)
{
	float distance = 0.0f;
	distance = sqrt((X- startPoint.x)*(X- startPoint.x) + (Y- startPoint.y)*(Y- startPoint.y));
	return distance;
}

  /*         判断速度是否为零     */ 
 float VEL;     
float GetVel(void)
{
	static float lastX = 0.0f, lastY = 0.0f;
	static  float velX = 0.0f, velY = 0.0f, vel = 0.0f;                            
	velX = X- lastX;
	velY = Y- lastY;
	vel = sqrtf(velX*velX + velY*velY);                             //将距离转化为速度    mm/s
	lastX = X;
	lastY =Y;
    VEL=vel;
	return vel;
}


/*          判断卡死状态      */
int ISStop;
uint8_t JudgeStop(void)
{
	static int cnt = 0, isStop = 0;

	if (fabs(GetVel())<Limit_Speed)                                 //速度小于0.05
	{
		cnt++;
	}
	else
	{
		cnt = 0;
	}
                                                                    //速度等于0的时间大于等于dead_time时认为机器人停住
	if (cnt >= dead_time)
		isStop = 1;
	else
		isStop = 0;
    ISStop=isStop;
	return isStop;
}

/*          获取卡死时矫正运行时的角度     */
float GetexpectAngle1(void)                           
{
   	float expectAngle = 0.0f;
     
    //场地下侧区域期望角度为-90                     
	 if ( X>-2400 && X<50 && Y>0 && Y<1400)  
    {
      expectAngle = Dead_Down_Angle;               
    }
    
     //场地右侧区域期望角度为0
	if (X>50 && X<2400 && Y>0 && Y<3000)             
	{
      expectAngle = Dead_Right_Angle;
    } 
    
    //场地顶部区域期望角度为90
	if (X>-50 && X<2400 && Y>3000 && Y<4500)
	{
      expectAngle = Dead_Up_Angle;
    }  
    
     //场地左侧区域期望角度为180
    if (X>-2400 && X<-50 &&  Y>1400 && Y<4500)          
	{   
      expectAngle = Dead_Left_Angle;
    }
	return expectAngle;
}

/*  获取车身当前与中线相对位置   */
int Axis_Sign;                        
void Get_relative_position(void)                         
{                        
	 if ( X>-2400 && X<50 && Y>0 && Y<1500)  
    {
        if(Y>712 )  
        {
            Axis_Sign=1;                                  //车在赛道左侧
        }
        else      
        {
            Axis_Sign=-1;                                 //车在赛道右侧
        }
    }
    
	if (X>50 && X<2400 && Y>0 && Y<3000)                  //场地右侧区域
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
    
	if (X>-50 && X<2400 && Y>3000 && Y<4500)               //上侧区域 
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
    
    if (X>-2400 && X<-50 &&  Y>1500 && Y<4500)             //左侧区域     
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

/*          卡死逃逸程序         */
uint8_t STEP;
uint8_t Escape(void)
{
    static short step = 1, cnt = 0,Compare_STA=0x01;
	char complete_flag = 0;
	static float  distance = 0.0f, expectangle1 = 0.0f,mark_expectangle;
	static point_t startPoint = { 0,0 };
    
	expectangle1 = GetexpectAngle1();                            //根据坐标确定目标角度  
    Get_relative_position();    
	distance = GetDistance(startPoint);                             //获取两点的距离
    
	switch (step)
	{
	case 1:                                                         //就两个状态  2和3
		
        CAN_To_Chaise(-1150,1180);                            //单纯的回退
		cnt++;
		if (Compare_STA==0x01)
		{
            startPoint.x = X;                                        //初始时将当前位置存入startpoint
            startPoint.y= Y; 
            Compare_STA=0x02;
        }
                                                                     //一段时间倒车的距离很小，加速向后倒车
		if (cnt >500 && distance < Back_Distance)                    //时间到了100ms，倒车距离不够 Back_Distance 进入第三步
		{
			step = 5;                                                //倒车距离过短，进入第三步，同时清空标志�
            cnt = 0;
            Compare_STA=0x01;                                        //跳出该步，标志位随之归位
		}
        
		if (distance > Back_Distance && cnt > 1)                     // 如果倒车距离够长，进入下一状态    一定要清空计数
		{
			step = 2;
			cnt = 0;
            Compare_STA=0x01;
		}
		break;
    case 2:                                                           //这个只执行一遍
       if( Axis_Sign==1)                                              //车在赛道的左侧，向右扭动调整
       {
           step = 3; 
           mark_expectangle=expectangle1;                            //已后退成功，记录此位置的角度
       }
       if( Axis_Sign==-1)
       {
           step = 4; 
           mark_expectangle=expectangle1;                            //已后退成功，记录此位置的角度
       }           
    break ;
    case 3:                                                           //车在赛道左侧，矫正过程
        cnt++;
        if(Action_Correct(Move_Right,20,mark_expectangle)==1)                          //向右扭动成功
        {
            cnt = 0;
			step = 1;                                                  //方便下次进入，
			complete_flag = 1;  
        }
        else if(cnt>800)
        {
            step=5;
            cnt=0;
        }
        break;
    case 4:                                                           //车在赛道右侧，进行矫正
        cnt++;
        if(Action_Correct(Move_Left,20,mark_expectangle)==1)                          //向左扭动成功
        {
            cnt = 0;
			step = 1;                                                  //方便下次进入，
			complete_flag = 1;  
        }
        else if(cnt>800)                                               //长时间无法矫正成功
        {
            step=5;
            cnt=0;
        }
        break;
    case 5:
        CAN_To_Chaise(-1500,1500);                              //卡死状态，巨大速度倒退
		cnt++;
		if (Compare_STA==0x01)
        {
           startPoint.x = X;                                           //初始时将当前位置存入startpoint
            startPoint.y= Y;  
            Compare_STA=0x02;
        }                                                          
		if (distance > 350 && cnt > 1)                            //直到倒车倒出一段距离后进入下一状态    
		{
			step = 2;
			cnt = 0;
            Compare_STA=0x01;                                    //已退出卡死状态，标志位归位
		}
        if(cnt>500)                                               //向后倒车的时间过长，已被卡死，满速向前冲
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
           startPoint.x = X;                                           //初始时将当前位置存入startpoint
           startPoint.y= Y;  
        } 
        if (distance > 350 && cnt > 1)                                //已经冲出30cm   退出卡死
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
