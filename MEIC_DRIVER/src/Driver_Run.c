#include "Driver_Run.h"

/*   更改边界坐标   */
WalkRoute_t  Route;
void ChangeRoute(int num)
{
	/*场地下侧为区域1，右侧为区域2，上侧为区域3，左侧为区域4
	Bound_UP_X-区域12交界直线   右上角的X
	Bound_UP_Y-区域23交界直线   右上角的Y
	Bound_Down_X-区域34交界直线   左下角的X
	Bound_Down_Y-区域14交界直线   左下角的Y
    num  代表圈数
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


/*    计算已走圈数   及  标记已走完一圈  */
uint8_t CycleNumber;
uint8_t GetCycle(void)
{
	static uint8_t start = 0, cycleNumber = 0;
                                                                    //在y小Brand_Y_Max大于Brand_Y_Min且小于1600的区域内判断圈数增加 
	if (X< -Brand_X &&Y>Brand_Y_Min &&Y<Brand_Y_Max)                 //随便定义一个标记区域     尽量靠近交界线   
	{
                                                                     //保证不会重复记录圈数
		if (start == 0)
		{  
			cycleNumber++;  
			start = 1;
		}
	}                                                                   //在判断区域外将标志位清零
	if (X>Brand_X && Y>Clean_Brand_Y_Min && Y<Clean_Brand_Y_Max)	 start = 0;
    CycleNumber=cycleNumber;
	return cycleNumber;
}

/*          计算是否已经走完一圈   */
int16_t GetCycleErr(int16_t circle)
{
	static int16_t circleLast = 0;
	int16_t velCircle;
	velCircle = circle - circleLast;
	circleLast = circle;
	return velCircle;
}

/*              正常行走函数         */
int16_t  V_RUN=4000;
void FreeWalk(void)
{                                                                  
	static float expectAngle = 0.0f;	                 //静态局部变量  
	if ((X<Route.Bound_UP_X&&Y < Route.Bound_Down_Y) || RANGE_OF_XY( -100,Route.Bound_UP_X, Route.Bound_Down_Y,1650))    //机器人位于场地下侧时，目标角度-90，水平像右
        expectAngle=Down_ExpectAngle;                                                                    
	if ((X> Route.Bound_UP_X&&Y< Route.Bound_UP_Y) || RANGE_OF_XY(100, Route.Bound_UP_X,  1650,Route.Bound_UP_Y))    //机器人位于场地右侧时，目标角度为0，竖直向上
		expectAngle=Right_ExpectAngle;                                                                   
	if ((X > Route.Bound_Down_X&&Y>Route.Bound_UP_Y) || RANGE_OF_XY(Route.Bound_Down_X,100,   2750,Route.Bound_UP_Y))    //机器人位于场地顶部时，目标角度为90，水平像右
		expectAngle=UP_ExpectAngle;                                                                 
	if ((X<Route.Bound_Down_X&&Y>Route.Bound_Down_Y) || RANGE_OF_XY(Route.Bound_Down_X,-100,Route.Bound_Down_Y,2750))        //机器人位于场地左侧时，目标角度为180，竖直向下
		expectAngle=Left_ExpectAngle;
    ChasisControl(V_RUN,-V_RUN,expectAngle);
}
