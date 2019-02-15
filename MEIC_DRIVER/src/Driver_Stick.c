#include "Driver_Stick.h"

/*          ��ȡ��ǰ������Գ�ʼλ��        */
float GetDistance(point_t startPoint)
{
	float distance = 0.0f;
	distance = sqrt((X- startPoint.x)*(X- startPoint.x) + (Y- startPoint.y)*(Y- startPoint.y));
	return distance;
}

  /*         �ж��ٶ��Ƿ�Ϊ��     */ 
 float VEL;     
float GetVel(void)
{
	static float lastX = 0.0f, lastY = 0.0f;
	static  float velX = 0.0f, velY = 0.0f, vel = 0.0f;                            
	velX = X- lastX;
	velY = Y- lastY;
	vel = sqrtf(velX*velX + velY*velY);                             //������ת��Ϊ�ٶ�    mm/s
	lastX = X;
	lastY =Y;
    VEL=vel;
	return vel;
}


/*          �жϿ���״̬      */
int ISStop;
uint8_t JudgeStop(void)
{
	static int cnt = 0, isStop = 0;

	if (fabs(GetVel())<Limit_Speed)                                 //�ٶ�С��0.05
	{
		cnt++;
	}
	else
	{
		cnt = 0;
	}
                                                                    //�ٶȵ���0��ʱ����ڵ���dead_timeʱ��Ϊ������ͣס
	if (cnt >= dead_time)
		isStop = 1;
	else
		isStop = 0;
    ISStop=isStop;
	return isStop;
}

/*          ��ȡ����ʱ��������ʱ�ĽǶ�     */
float GetexpectAngle1(void)                           
{
   	float expectAngle = 0.0f;
     
    //�����²����������Ƕ�Ϊ-90                     
	 if ( X>-2400 && X<50 && Y>0 && Y<1400)  
    {
      expectAngle = Dead_Down_Angle;               
    }
    
     //�����Ҳ����������Ƕ�Ϊ0
	if (X>50 && X<2400 && Y>0 && Y<3000)             
	{
      expectAngle = Dead_Right_Angle;
    } 
    
    //���ض������������Ƕ�Ϊ90
	if (X>-50 && X<2400 && Y>3000 && Y<4500)
	{
      expectAngle = Dead_Up_Angle;
    }  
    
     //����������������Ƕ�Ϊ180
    if (X>-2400 && X<-50 &&  Y>1400 && Y<4500)          
	{   
      expectAngle = Dead_Left_Angle;
    }
	return expectAngle;
}

/*  ��ȡ����ǰ���������λ��   */
int Axis_Sign;                        
void Get_relative_position(void)                         
{                        
	 if ( X>-2400 && X<50 && Y>0 && Y<1500)  
    {
        if(Y>712 )  
        {
            Axis_Sign=1;                                  //�����������
        }
        else      
        {
            Axis_Sign=-1;                                 //���������Ҳ�
        }
    }
    
	if (X>50 && X<2400 && Y>0 && Y<3000)                  //�����Ҳ�����
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
    
	if (X>-50 && X<2400 && Y>3000 && Y<4500)               //�ϲ����� 
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
    
    if (X>-2400 && X<-50 &&  Y>1500 && Y<4500)             //�������     
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

/*          �������ݳ���         */
uint8_t STEP;
uint8_t Escape(void)
{
    static short step = 1, cnt = 0,Compare_STA=0x01;
	char complete_flag = 0;
	static float  distance = 0.0f, expectangle1 = 0.0f,mark_expectangle;
	static point_t startPoint = { 0,0 };
    
	expectangle1 = GetexpectAngle1();                            //��������ȷ��Ŀ��Ƕ�  
    Get_relative_position();    
	distance = GetDistance(startPoint);                             //��ȡ����ľ���
    
	switch (step)
	{
	case 1:                                                         //������״̬  2��3
		
        CAN_To_Chaise(-1150,1180);                            //�����Ļ���
		cnt++;
		if (Compare_STA==0x01)
		{
            startPoint.x = X;                                        //��ʼʱ����ǰλ�ô���startpoint
            startPoint.y= Y; 
            Compare_STA=0x02;
        }
                                                                     //һ��ʱ�䵹���ľ����С��������󵹳�
		if (cnt >500 && distance < Back_Distance)                    //ʱ�䵽��100ms���������벻�� Back_Distance ���������
		{
			step = 5;                                                //����������̣������������ͬʱ��ձ�־�
            cnt = 0;
            Compare_STA=0x01;                                        //�����ò�����־λ��֮��λ
		}
        
		if (distance > Back_Distance && cnt > 1)                     // ����������빻����������һ״̬    һ��Ҫ��ռ���
		{
			step = 2;
			cnt = 0;
            Compare_STA=0x01;
		}
		break;
    case 2:                                                           //���ִֻ��һ��
       if( Axis_Sign==1)                                              //������������࣬����Ť������
       {
           step = 3; 
           mark_expectangle=expectangle1;                            //�Ѻ��˳ɹ�����¼��λ�õĽǶ�
       }
       if( Axis_Sign==-1)
       {
           step = 4; 
           mark_expectangle=expectangle1;                            //�Ѻ��˳ɹ�����¼��λ�õĽǶ�
       }           
    break ;
    case 3:                                                           //����������࣬��������
        cnt++;
        if(Action_Correct(Move_Right,20,mark_expectangle)==1)                          //����Ť���ɹ�
        {
            cnt = 0;
			step = 1;                                                  //�����´ν��룬
			complete_flag = 1;  
        }
        else if(cnt>800)
        {
            step=5;
            cnt=0;
        }
        break;
    case 4:                                                           //���������Ҳ࣬���н���
        cnt++;
        if(Action_Correct(Move_Left,20,mark_expectangle)==1)                          //����Ť���ɹ�
        {
            cnt = 0;
			step = 1;                                                  //�����´ν��룬
			complete_flag = 1;  
        }
        else if(cnt>800)                                               //��ʱ���޷������ɹ�
        {
            step=5;
            cnt=0;
        }
        break;
    case 5:
        CAN_To_Chaise(-1500,1500);                              //����״̬���޴��ٶȵ���
		cnt++;
		if (Compare_STA==0x01)
        {
           startPoint.x = X;                                           //��ʼʱ����ǰλ�ô���startpoint
            startPoint.y= Y;  
            Compare_STA=0x02;
        }                                                          
		if (distance > 350 && cnt > 1)                            //ֱ����������һ�ξ���������һ״̬    
		{
			step = 2;
			cnt = 0;
            Compare_STA=0x01;                                    //���˳�����״̬����־λ��λ
		}
        if(cnt>500)                                               //��󵹳���ʱ��������ѱ�������������ǰ��
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
           startPoint.x = X;                                           //��ʼʱ����ǰλ�ô���startpoint
           startPoint.y= Y;  
        } 
        if (distance > 350 && cnt > 1)                                //�Ѿ����30cm   �˳�����
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
