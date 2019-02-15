#include "Driver_Run.h"

/*   ���ı߽�����   */
WalkRoute_t  Route;
void ChangeRoute(int num)
{
	/*�����²�Ϊ����1���Ҳ�Ϊ����2���ϲ�Ϊ����3�����Ϊ����4
	Bound_UP_X-����12����ֱ��   ���Ͻǵ�X
	Bound_UP_Y-����23����ֱ��   ���Ͻǵ�Y
	Bound_Down_X-����34����ֱ��   ���½ǵ�X
	Bound_Down_Y-����14����ֱ��   ���½ǵ�Y
    num  ����Ȧ��
	*/  
     switch(num)
     {      
         case 0:                             
        Route.Bound_UP_X =300;
	    Route.Bound_UP_Y = 3000;
	    Route.Bound_Down_X = -300;
	    Route.Bound_Down_Y =1300;    
        break;
         case 1:                             
        Route.Bound_UP_X =500;
	    Route.Bound_UP_Y = 3300;
	    Route.Bound_Down_X = -500;
	    Route.Bound_Down_Y =1300;
         break;
         case 2:                          
        Route.Bound_UP_X =250;
	    Route.Bound_UP_Y = 2900;
	    Route.Bound_Down_X = -250;
	    Route.Bound_Down_Y =1400;
         break;
        case 3:                              
        Route.Bound_UP_X =375;
	    Route.Bound_UP_Y = 3100;
	    Route.Bound_Down_X = -375;
	    Route.Bound_Down_Y =1200;
         break; 
        case 4:                             
        Route.Bound_UP_X =180;
	    Route.Bound_UP_Y = 2750;
	    Route.Bound_Down_X = -150;
	    Route.Bound_Down_Y =1600;
         break; 
       case 5:                              
        Route.Bound_UP_X =400;
	    Route.Bound_UP_Y = 2900;
	    Route.Bound_Down_X = -400;
	    Route.Bound_Down_Y =1450;
         break; 
       case 6:                              
        Route.Bound_UP_X =250;
	    Route.Bound_UP_Y = 2900;
	    Route.Bound_Down_X = -250;
	    Route.Bound_Down_Y =1400;
        break; 
    }
}


/*    ��������Ȧ��   ��  ���������һȦ  */
uint8_t CycleNumber;
uint8_t GetCycle(void)
{
	static uint8_t start = 0, cycleNumber = 0;
                                                                    //��yСBrand_Y_Max����Brand_Y_Min��С��1600���������ж�Ȧ������ 
	if (X< -Brand_X &&Y>Brand_Y_Min &&Y<Brand_Y_Max)                 //��㶨��һ���������     ��������������   
	{
                                                                     //��֤�����ظ���¼Ȧ��
		if (start == 0)
		{  
			cycleNumber++;  
			start = 1;
		}
	}                                                                   //���ж������⽫��־λ����
	if (X>Brand_X && Y>Clean_Brand_Y_Min && Y<Clean_Brand_Y_Max)	 start = 0;
    CycleNumber=cycleNumber;
	return cycleNumber;
}

/*          �����Ƿ��Ѿ�����һȦ   */
int16_t GetCycleErr(int16_t circle)
{
	static int16_t circleLast = 0;
	int16_t velCircle;
	velCircle = circle - circleLast;
	circleLast = circle;
	return velCircle;
}

/*              �������ߺ���         */
int16_t  V_RUN=4000;
void FreeWalk(void)
{                                                                  
	static float expectAngle = 0.0f;	                 //��̬�ֲ�����  
	if ((X<Route.Bound_UP_X&&Y < Route.Bound_Down_Y) || RANGE_OF_XY( -100,Route.Bound_UP_X, Route.Bound_Down_Y,1650))    //������λ�ڳ����²�ʱ��Ŀ��Ƕ�-90��ˮƽ����
        expectAngle=Down_ExpectAngle;                                                                    
	if ((X> Route.Bound_UP_X&&Y< Route.Bound_UP_Y) || RANGE_OF_XY(100, Route.Bound_UP_X,  1650,Route.Bound_UP_Y))    //������λ�ڳ����Ҳ�ʱ��Ŀ��Ƕ�Ϊ0����ֱ����
		expectAngle=Right_ExpectAngle;                                                                   
	if ((X > Route.Bound_Down_X&&Y>Route.Bound_UP_Y) || RANGE_OF_XY(Route.Bound_Down_X,100,   2750,Route.Bound_UP_Y))    //������λ�ڳ��ض���ʱ��Ŀ��Ƕ�Ϊ90��ˮƽ����
		expectAngle=UP_ExpectAngle;                                                                 
	if ((X<Route.Bound_Down_X&&Y>Route.Bound_Down_Y) || RANGE_OF_XY(Route.Bound_Down_X,-100,Route.Bound_Down_Y,2750))        //������λ�ڳ������ʱ��Ŀ��Ƕ�Ϊ180����ֱ����
		expectAngle=Left_ExpectAngle;
    ChasisControl(V_RUN,-V_RUN,expectAngle);
}
