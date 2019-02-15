#include "Driver_Control.h"
#include "Driver_Motor.h"
#include "Driver_PID.h"
#include "DefCollect.h"
#include "math.h"
#define Pos_Exchange(angle)  (((int16_t)angle/90)%2)?(0):(1)
#define Rad2Ang(Rad)   (Rad*180/3.1415926)
#define State_Set(begin_angle)    (begin_angle>=0)?(0):(1)
#define Getsign(error) (error>=0)?(1):(-1)
#define Body_Length 200
#define Lrap  0.2
#define Rrap  0.4
#define Percent
Cir Cir_Draw={-1000,0,0,0,1010,0,1,0,1};
int16_t leftinput;
int16_t rightinput;	 
int16_t rollerinput;
extern float test; 
extern float ZAngle,X,Y;
u8 Curve_FLAG = 0x40;
u8 Kinked_FLAG =0X20;
u8 Sheet_FLAG = 0x10;
u8 Stop_FLAG = 0x08;
u8 Turn_FLAG =0x04;
u8 Stright_FLAG =0x02;
u8 VLoop_FLAG =0x01;

float given_vexpect = 0;
void Circle_Count(float	angle,int8_t* n);

void tmr_5ms_callback(void);
void Closeloop_Speed(float p,float i,float d,float expect,float imax,float maxoutput);
void Cir_ParamSet(Cir *cir,float x,float y,float radius,float angle,float direcion,float speed,float park);
void WaitForEncoder(void);
/*---临时储存点----*/
struct tempory_story
{
	float x;
	float y;
	float angle;
}temp_story;
struct CurrentPonit
{
   float x;
	 float y;
} current;
struct PreviousPoint
{
	float x;
	float y;	
} previous;
void tmr_20ms_callback(void)
{   
	Color_Process(&BaseLeft);	
	Color_Process(&BaseRight);	
	Color_Process(&Select);	
	Color_Process(&Behindleft);	
	Color_Process(&Behindright);	
}

/*用于调试组件*/
extern OS_FLAG_GRP	EventFlags;		
extern PID_Struct PID_Roller_V;
extern PID_Struct PID_BaseLeft_V;
extern PID_Struct PID_BaseRight_V;
extern PID_Struct PID_Position;
extern PID_Struct PID_Roller;
extern PID_Struct PID_Angle;
extern OS_FLAG_GRP	EventFlags;		
extern OS_TMR  tmr2;
void tmr_1s_callback(void)
{

	  PID_BaseRight_V.Expect = -given_vexpect;
	  PID_BaseLeft_V.Expect = given_vexpect;		
		PID_Roller_V.Expect = given_vexpect;
}
void Closeloop_Speed(float p,float i,float d,float expect,float imax,float maxoutput)
{ 				
 	  OS_ERR err;
	  OSFlagPost(&EventFlags,  
								VLoop_FLAG,   
								OS_OPT_POST_FLAG_SET,
								&err);
	  PID_Param_Modify(&PID_BaseLeft_V,p,i,d,imax,maxoutput);	 	  	 	  	
	  PID_Param_Modify(&PID_BaseRight_V,p,i,d,imax,maxoutput);	
		PID_Param_Modify(&PID_Roller_V,p,i,d,imax,maxoutput);
	  given_vexpect=expect;
}
void USMART_Travel(float distance,float speed)
{
	  OS_ERR err;
	  OSFlagPost(&EventFlags,  
								Stright_FLAG,   
								OS_OPT_POST_FLAG_SET,
								&err);
	  PID_Position.Expect = distance;
	  given_vexpect =speed;
}
void USMART_Turn(float p,float i,float d,float expect)
{	
	 OS_ERR err;
	 OSFlagPost(&EventFlags,  
								Turn_FLAG,   
								OS_OPT_POST_FLAG_SET,
								&err);
   PID_Param_Modify(&PID_Angle,p,i,d,1000,5000);	 	 
	 PID_Angle.Expect = expect;	
}
void  USMART_Stop (void)
{
	 OS_ERR err;
	 OSFlagPost(&EventFlags,  
								Stop_FLAG,   
								OS_OPT_POST_FLAG_SET,
								&err);
}
float Dir,Dis,Ang;
void USMART_Kinked(float dir,float dis,float ang,float p,float i,float d)
{	
	 OS_ERR err;
	 OSFlagPost(&EventFlags,  
								Kinked_FLAG,   
								OS_OPT_POST_FLAG_SET,
								&err);
   Dir = dir;
	 Dis = dis; 
	 temp_story.x = X;
	 temp_story.y = Y;	 	 
	 temp_story.angle = ang;
	 Ang = ang;
	 PID_Param_Modify(&PID_Position,p,i,d,1000,5000);	 	 
}

void USMART_Sheet(float number,float angle)	
{
	 SheetControl((int8_t)number,(int8_t)angle);	

}
void USMART_Circle(float x,float y,float radius,float angle,float direcion,float speed,float park) 
{
   Cir_ParamSet(&Cir_Draw,x,y,radius,angle,direcion,speed,park);	
}

int16_t Vwheel1=0;
int16_t Vwheel2=0;
void USMART_ChangeParam(float p1,float p2)
{
	 Vwheel1 = p1;
	 Vwheel2 = p2;
}
float Direction=0,Basespeed =0,Baseangle = 0,Addangle = 0,Boundary = 0; 
void USMART_Curve(float dir,float boundary,float basespeed,float baseangle,float addangle)
{
    OS_ERR err;
	  OSFlagPost(&EventFlags,  
								Curve_FLAG,   
								OS_OPT_POST_FLAG_SET,
								&err);
	  Direction = dir;
	  Basespeed =basespeed;
	  Baseangle = baseangle;
	  Addangle =  addangle;
	  Boundary =boundary;

}
/*-----调试组件函数------*/
void Usmart_Movement(void)
{
	   OS_ERR err;
	   u8 loop_flag = 0;	
     static u8 turn_flag = 0,stright_flag = 0,kinked_flag = 0,curve_flag=0;	
	   static float Init_Y=0;
	   loop_flag=OSFlagPend (&EventFlags,
               VLoop_FLAG+Stright_FLAG+Turn_FLAG+Stop_FLAG+Kinked_FLAG+Curve_FLAG,
							 1,
	             OS_OPT_PEND_FLAG_CONSUME+OS_OPT_PEND_FLAG_SET_ANY,
	             0,
	             &err);	 	   
     /*无线调试速度闭环*/
		 if(loop_flag == VLoop_FLAG)
		 {						
		
			 PID_BaseLeft_V.Expect  = 0;
			 PID_BaseRight_V.Expect = 0;
			 PID_Roller_V.Expect = 0;
			 PID_BaseLeft_V.SumErr  = 0;
			 PID_BaseRight_V.SumErr = 0;
			 PID_Roller_V.SumErr = 0;
			 OSTmrStart(&tmr2,&err);			
		 }		 
		  /*无线调试速度直行*/
		 if(loop_flag == Stright_FLAG)
		 {	
			 stright_flag = 1;
			 turn_flag = 0;	
			 kinked_flag	= 0;
			 PID_BaseLeft_V.Expect =  given_vexpect;
			 PID_BaseRight_V.Expect = -given_vexpect;		
		 }		 
		   /*无线调试角度闭环*/
		 if(loop_flag == Turn_FLAG) 
		 {			 				 
			 turn_flag = 1;	
			 kinked_flag	= 0;
			 stright_flag = 0;					
		 }
		 /* 无线画弧线 */
		 if(loop_flag == Kinked_FLAG)
		 {
			 kinked_flag =1;
			 turn_flag = 0;			 
			 stright_flag = 0;      
		 }
		   /*无线停车*/
     if(loop_flag == Stop_FLAG) 
		 {		        
			 turn_flag = 0;
			 stright_flag = 0;	
			 PID_BaseLeft_V.Expect =  0;
			 PID_BaseRight_V.Expect = 0;			 
		 }		  	
		 if(loop_flag == Curve_FLAG) 
		 {
			 previous.y= Y;
			 previous.x= X;
			 curve_flag  =1;			 
		 }
		 if(kinked_flag)
		 {
			 Arc_Move(Dir,Dis,Ang);			 
		 }
     if(curve_flag)
		 {
			 Angleloop_Process();
			 Curve_Move(Direction,Boundary,Basespeed,Baseangle,Addangle);
		 }
		 /*直行和转弯调试的切换*/
		 if(turn_flag||stright_flag)
		 {				       
			 if(turn_flag)
       {
					given_vexpect = 0;	
			 }
			 if(stright_flag)
		   {				
				 if(Init_Y<=PID_Position.Expect)
				 {
					 if(Y>=PID_Position.Expect)
				  {				 
					 given_vexpect= 0;
					 Init_Y=Y;
				  }
		     }
				 else
				 {
					  if(Init_Y<=PID_Position.Expect)
						{				 
						 given_vexpect= 0;
						 Init_Y=Y;
						}
				 }
			 }
		   Angleloop_Process();  	 		  
		   PID_BaseLeft_V.Expect =  -PID_Calc(&PID_Angle)+given_vexpect;		  
			 PID_BaseRight_V.Expect =  -PID_Calc(&PID_Angle)-given_vexpect;
		 }	
}		 
/*------等待陀螺仪-----*/
void WaitForEncoder(void)
{
	 OS_ERR err;
   OSTimeDlyHMSM(0,0,10,0,0,&err);	 
}


/*---将陀螺仪的值无穷化,需要快速执行---*/
float Begining_Angle = 0;
float Location_Angle =0;
int8_t state=-1;	
void Circle_Count(float	angle,int8_t* n)
{	  
	  if(state==-1)
		{			
			
			state = State_Set(Location_Angle);
		}
	  if(angle<0&&!state)
	  {
			 if(fabs(angle)>90)
		   (*n)++;
       if(state)
			 {
				 state=0;
			 }
			 else
			 {
				 state=1;
			 }
		}
	  if(angle>0&&state)
	  {
	  	if(fabs(angle)>90)
		  (*n)--;					
			 if(state)
			 {
				 state=0;
			 }
			 else
			 {
				 state=1;
			 }
	  }					
}
int8_t Carry_Out =0;
int8_t Record=0;
int8_t EXCoor_Dif=0;
float  Coor_Dif =0;
extern u8 enterflag;	
/*-----车身画圆弧函数的初始值设置----*/
void Cir_ParamSet(Cir *cir,float x,float y,float radius,float angle,float direcion,float speed,float park)
{	 	
	 if(park>0)
	 {
		 //圆心坐标，半径//
		 cir->cx = x;
	   cir->cy = y;
	   cir->radius = radius;
	   //顺时针还是逆时针//
     cir->direcion = 	direcion;
	   
	   cir->angle = angle;
	   
	   cir->speed =speed;	 
     
		 cir->park = park;			 
		 Carry_Out =0;
		 state = -1;		 
	 }
	 else
	 {				 		    
		 if(!Record)
		 {			 
			 Coor_Dif =Cir_Draw.nx-Cir_Draw.cx;
			 EXCoor_Dif=1;
		 }
		 else
		 {
			 Coor_Dif =Cir_Draw.ny-Cir_Draw.cy;
			 EXCoor_Dif=0;
		 }
		 Record = !(State_Set((Getsign(Coor_Dif))*Cir_Draw.direcion));		 		 		 
		 //初始角度//
		 Begining_Angle  = PID_Angle.Expect;
		 //旋转角度//
		 Cir_Draw.speed = speed;
		 //旋转速度//
		 cir->angle = angle;  
		 //是否倒车//
		 cir->park = park;		 
	 }	 
}
void Radius_Control(Cir *cir)
{
	if(fabs(cir->cx-cir->nx)>cir->radius)
	{
		cir->radius = fabs(cir->cx-cir->nx);   	
	}		
	if(fabs(cir->cy-cir->ny)>cir->radius)
	{
		cir->radius = fabs(cir->cy-cir->ny);	
	}  
}
int8_t ready=0;
int8_t fake = 0 ;
int8_t N=0 ;
int8_t flag1 =0;

extern float test;
/*-----车身画圆弧函数-------*/
void Cir_Move(void) 
{	   
	 static int8_t i=0,finish=0;		 	 	 
	 Cir_Draw.nx = X;
	 Cir_Draw.ny = Y;	 	 		
	 Radius_Control(&Cir_Draw);	 
	 if(!Carry_Out)
	 {
		 N=0;
		 i=0;		     		 
		 Begining_Angle = Location_Angle = ZAngle;
		 /*初始角度矫正*/
		 if(Cir_Draw.nx-Cir_Draw.cx!=0&&Cir_Draw.ny-Cir_Draw.cy!=0)
		 {
			 if ((Getsign(Cir_Draw.nx-Cir_Draw.cx))==(Getsign(Cir_Draw.ny-Cir_Draw.cy)))
			 {		
					test=2 ;
					Begining_Angle = PID_Angle.Expect = fabs(Rad2Ang(asin((Cir_Draw.ny-Cir_Draw.cy)/Cir_Draw.radius)))-(State_Set(Getsign(Cir_Draw.ny-Cir_Draw.cy)*Cir_Draw.direcion)*180.f);			  
			 }	
			 else    
			 {
					Begining_Angle = PID_Angle.Expect = (State_Set(-(Getsign(Cir_Draw.ny-Cir_Draw.cy))*Cir_Draw.direcion)*180.f)-fabs(Rad2Ang(asin((Cir_Draw.ny-Cir_Draw.cy)/Cir_Draw.radius)));
					test=1;
			 }
			 Carry_Out = 1;
		 }
		 else
		 {   /*解决0点问题*/
				if(Cir_Draw.nx-Cir_Draw.cx==0&&(Cir_Draw.ny-Cir_Draw.cy)==0)
				{			
				  Begining_Angle = PID_Angle.Expect  = ZAngle;					
				}
				else if(Cir_Draw.nx-Cir_Draw.cx==0)
				{
					Begining_Angle = PID_Angle.Expect=Cir_Draw.direcion*90;
				}
				else if(Cir_Draw.ny-Cir_Draw.cy==0)
				{
          Begining_Angle = PID_Angle.Expect=State_Set(Getsign(Cir_Draw.nx-Cir_Draw.cx)*Cir_Draw.direcion)*180;    
				}					
		 }	
		 Record = Pos_Exchange(Begining_Angle);
		 if(!Record)
		 i++;		 
	 }	 		 	 
	 Circle_Count(ZAngle,&N);	 	 	 
	 PID_Angle.Measured = ZAngle + 360 * N;		 
	 if(Error_Admission((PID_Angle.Expect-PID_Angle.Measured),5)&&!ready)
	 {
	    ready=1;
	 }
	 /*开始按照预定轨迹旋转*/	
	 if(ready)
	 {				 
     if(!fake)
		 {			 			 
			 if(Begining_Angle*Cir_Draw.direcion>=-1||finish)
			 {
				 if(Record)
				 {							
					  test=3;					
					   PID_Angle.Expect = Cir_Draw.direcion*(Rad2Ang(fabs(asin((Cir_Draw.ny-Cir_Draw.cy)/Cir_Draw.radius)))+90*i);   					
				 }				 
				 else  
				 {  				 				 					 	
					  test=4;				 
					  PID_Angle.Expect = Cir_Draw.direcion*(Rad2Ang(fabs(asin((Cir_Draw.nx-Cir_Draw.cx)/Cir_Draw.radius)))+90*i);			 
				 }	
			 }		
			 else
			 {		
				 if(Cir_Draw.direcion* PID_Angle.Measured>0)
				 {
					  finish=1;
				 }
				 if(Cir_Draw.direcion * ZAngle<-90)	
				 {
						test=5;				 
						PID_Angle.Expect = Cir_Draw.direcion * (Rad2Ang(asin(fabs(Cir_Draw.ny-Cir_Draw.cy)/Cir_Draw.radius))- 180);
				 }
				 else	
				 {	
						test=6;				 				 
						PID_Angle.Expect = Cir_Draw.direcion * (Rad2Ang(asin((fabs(Cir_Draw.nx-Cir_Draw.cx)/Cir_Draw.radius)))- 90);
				 }			
			 }
			 if(Cir_Draw.direcion>0)
			 {
				 if(Cir_Draw.park*(Cir_Draw.angle+Begining_Angle)<=PID_Angle.Expect*Cir_Draw.park) 
				 {
					 PID_Angle.Expect = Cir_Draw.angle+Begining_Angle;					 					
					 Cir_Draw.speed =0;	
					 ready=0;			 					 				
				 }	 
			 }
			 else 
			 { 
				 if(Cir_Draw.park*(Begining_Angle-Cir_Draw.angle)>=PID_Angle.Expect*Cir_Draw.park)
				 {  			 			 
					 PID_Angle.Expect = Begining_Angle-Cir_Draw.angle;				
					 Cir_Draw.speed =0;					
					 ready=0; 								 
				 }				 
			 }			 			
		 }			 
		 if(Cir_Draw.park >0)
		 {
			 /*到达角度边界*/
			 if((Record!= Pos_Exchange(PID_Angle.Expect))||fake)
			 { 
				
				 if(!Pos_Exchange(PID_Angle.Expect))		
				 { 				
					 if(Getsign(Cir_Draw.ny-Cir_Draw.cy)>0)
					 {					 					
						 if(Cir_Draw.direcion*fabs(Cir_Draw.nx)<Cir_Draw.direcion*fabs(Cir_Draw.cx))						 
						 {					
							 fake = 1;	
						 }
						 else
						 {					 
							fake = 0;	
						 }
					 }				 
					 else
					 {					
						 if(Cir_Draw.direcion*fabs(Cir_Draw.nx)>Cir_Draw.direcion*fabs(Cir_Draw.cx))						  
						 {						 
							 fake = 1;
						 }
						 else
						 {
							 fake = 0;					
						 }
					 }
				 }
				 else
				 {
					 if(Getsign(Cir_Draw.nx-Cir_Draw.cx)>0)
					 {
						 if(Cir_Draw.direcion*fabs(Cir_Draw.ny)<Cir_Draw.direcion*fabs(Cir_Draw.cy))						 										 
						 fake = 1;								 
						 else
						 fake = 0;
					 }
					 else
					 {
						 if(Cir_Draw.direcion*fabs(Cir_Draw.ny)>Cir_Draw.direcion*fabs(Cir_Draw.cx))						  
						 fake = 1;
						 else
						 fake = 0;	 
					 }
				 }
				 if(!fake)
				 {
						 i++;			 
				 }
				 Record = Pos_Exchange(PID_Angle.Expect); 
			 }				 
		 }
		 else		
		 {
			 if(EXCoor_Dif)
			 {
				 Coor_Dif = Cir_Draw.nx-Cir_Draw.cx;
			 }
			 else
			 {
				 Coor_Dif = Cir_Draw.ny-Cir_Draw.cy;				 
			 }						 
			 if(Record!=!(State_Set((Getsign(Coor_Dif))*Cir_Draw.direcion)))
			 {
				 if(EXCoor_Dif)
				 {
					 Coor_Dif = Cir_Draw.ny-Cir_Draw.cy;
					 EXCoor_Dif =0;
				 }
				 else
				 {
					 Coor_Dif = Cir_Draw.nx-Cir_Draw.cx;
					 EXCoor_Dif =1;
				 }				 
				 i--;
			 }
			 Record = !(State_Set((Getsign(Coor_Dif))*Cir_Draw.direcion));
		 }
	 }
	 PID_BaseLeft_V.Expect =  -PID_Calc(&PID_Angle)+Cir_Draw.park*Cir_Draw.speed;		  
	 PID_BaseRight_V.Expect =  -PID_Calc(&PID_Angle)-Cir_Draw.park*Cir_Draw.speed;
}

/*----画弧侧移函数----*/
u8 Arc_Move(float sidesway_dir,float sidesway_dis,float sidesway_ang)
{	 	
	 float adjust = 0;
	 if(sidesway_dir)
	 {				 		
		 PID_Position.Expect   = sidesway_dis+temp_story.y;	
		 PID_Position.Measured = Y;  	
		 test = PID_Angle.Expect =(Getsign(-sidesway_dis))*(Rad2Ang(fabs(atan((PID_Position.Expect-PID_Position.Measured)/sidesway_dis)))+temp_story.angle);
	 }
	 else
	 {
		 PID_Position.Expect   = sidesway_dis+temp_story.x;	 
	   PID_Position.Measured = X;  	
		 test = PID_Angle.Expect =(Getsign(-sidesway_dis))*(Rad2Ang(fabs(atan((PID_Position.Expect-PID_Position.Measured)/sidesway_dis)))+temp_story.angle);
	 }
   if(Error_Admission(PID_Position.Expect-PID_Position.Measured,5))
	 {
		 PID_Position.I = 0;
	  // return 1;
	 }
	 adjust =(Getsign(-sidesway_dis))*PID_Calc(&PID_Position);	 	 
	 PID_BaseLeft_V.Expect =   -PID_Calc(&PID_Angle)-adjust;		     
	 PID_BaseRight_V.Expect =  -PID_Calc(&PID_Angle)+adjust;	 		 		 
	 return 0;
}

/*----误差可控准许------*/
u8 Error_Admission(float variable,float rage)
{
   if(fabs(variable)<rage)
		 return 1;
	 else
		 return 0;
}	

/*---求得车的旋转半径----*/
float  SolveforR(void)
{  
   return sqrt(pow(Cir_Draw.cx-Cir_Draw.nx,2)+pow(Cir_Draw.cy-Cir_Draw.ny,2)); 
}

/*----车身画多边形函数----*(这个就先实现六变形的功能吧，就三天时间汗）--*/
void  Polygon_Movement(void)
{
  
}



/*----六边形坐标函数----*/
void Hexagon_ParamSet(float nx,float ny,float min_side)
{
   Hexagon.cx  = nx;
	 Hexagon.cy  = ny;
	 Hexagon.radius  = min_side*2/3*1.732f;
	 Hexagon.terminal[0].x = Hexagon.cx +Hexagon.radius/2;
   Hexagon.terminal[0].y = Hexagon.cy - min_side;	 
	 Hexagon.terminal[1].x = Hexagon.terminal[0].x+Hexagon.radius/2;
	 Hexagon.terminal[1].y = Hexagon.cy;
	 Hexagon.terminal[2].x = Hexagon.terminal[0].x;
	 Hexagon.terminal[2].y = Hexagon.cy+min_side;
	 Hexagon.terminal[3].x = Hexagon.cx - Hexagon.radius/2;
	 Hexagon.terminal[3].y = Hexagon.terminal[2].y;
	 Hexagon.terminal[4].x = Hexagon.terminal[3].x - Hexagon.radius/2;
	 Hexagon.terminal[4].y = Hexagon.terminal[1].y ;
	 Hexagon.terminal[5].x = Hexagon.cx-Hexagon.radius/2;
	 Hexagon.terminal[5].y = Hexagon.terminal[0].y;
}
/*---六边边界获取----*/
u8 Confirm_Board(float nx,float ny)
{  
    if(ny<=Hexagon.terminal[1].y && ny >= Hexagon.terminal[0].y && nx>= Hexagon.cx)
		{
			 previous.x = Hexagon.terminal[0].x;
			 previous.y = Hexagon.terminal[0].y; 		
			 return 1;
		}
	  if(ny<= Hexagon.terminal[2].y && ny >= Hexagon.terminal[1].y && nx>= Hexagon.cx)
		{		
			 previous.x = Hexagon.terminal[1].x;
			 previous.y = Hexagon.terminal[1].y; 		
			 return 2;	
		}
		if(nx<= Hexagon.terminal[3].x && nx >= Hexagon.terminal[2].x && ny>= Hexagon.cy)
		{
			 previous.x = Hexagon.terminal[2].x;
			 previous.y = Hexagon.terminal[2].y; 		
			 return 3;	
		}	  
		if(ny<= Hexagon.terminal[3].y && ny >= Hexagon.terminal[4].y && nx<= Hexagon.cx)	  
		{
		   previous.x = Hexagon.terminal[3].x;
			 previous.y = Hexagon.terminal[3].y; 	
  		 return 4;	
		}		
		if(ny<= Hexagon.terminal[4].y && ny >= Hexagon.terminal[5].y && nx<= Hexagon.cx)
		{			
			 previous.x = Hexagon.terminal[4].x;
			 previous.y = Hexagon.terminal[4].y;
			 return 5;	
		}					
	  if(nx<= Hexagon.terminal[0].x && nx >= Hexagon.terminal[5].x && ny<= Hexagon.cy)	  
		{
			 previous.x = Hexagon.terminal[5].x;
			 previous.y = Hexagon.terminal[5].y;
			 return 6;	
		}	  
		return 0;
}	

/*获取两点的距离*/
float  GetDis(void)
{
	float Distance = 0.0f;
  Distance = sqrt((current.x - previous.x)*(current.x - previous.x) + (current.y - previous.y)*(current.y - previous.y));
	return Distance;
}
float Adjust=0;
/*角度环函数的处理*/
void  Angleloop_Process(void)
{
	 PID_Angle.Measured = ZAngle;
	 if((PID_Angle.Expect -PID_Angle.Measured)>180||(PID_Angle.Expect -PID_Angle.Measured)<-180)
	 PID_Angle.Measured = Getsign(PID_Angle.Expect)*360+ZAngle;                        		   
}

/*---角度环直线----*/
void Line_Move(float distance,float basespeed,float angle,float realange,float percent)
{  
   PID_Angle.Expect  = angle;	 
	 PID_Angle.Measured = realange;	
	 Adjust = PID_Calc(&PID_Angle);
	 PID_BaseLeft_V.Expect =  -Adjust+basespeed;		     
	 PID_BaseRight_V.Expect =  -Adjust-basespeed;		
} 
/*---角度环弧线---*/
u8 Curve_Move(float dir,float boundary,float basespeed,float baseangle,float addangle)
{
	  float percent;	   
	  if(dir)
		{			
			percent = fabs((Y-previous.y)/(boundary-previous.y));			
			percent = percent>=1 ?1:percent;
		  PID_Angle.Expect = percent*addangle+baseangle;
		}
		else
		{
		  percent = fabs((X-previous.x)/(boundary-previous.x));
			percent = percent>=1 ?1:percent;
			PID_Angle.Expect = fabs((X-previous.x)/(boundary-previous.x))*addangle+baseangle;
		} 		
		if(percent>=1)
		{
			return 1;
		}
	  Adjust = PID_Calc(&PID_Angle);		
    PID_BaseLeft_V.Expect =   -Adjust+basespeed;		     
	  PID_BaseRight_V.Expect =  -Adjust-basespeed;		
    return 0;
}
hexagon Hexagon;
/*---六边形走形状---*/
void Hexagon_Move(void)
{
	static u8 step =0 ;	
  /*获得角度实际值 用于角度环*/	
  Angleloop_Process();	
  
	switch(step)//Confirm_Board(X,Y))
	{
		case 0:
			 Hexagon_ParamSet(0,1941,1000);
		   step=1; 
		break;
		case 1:
			 Arc_Move(0,Hexagon.terminal[0].x,30);
			 previous.x = Hexagon.terminal[0].x;
			 previous.y = Hexagon.terminal[0].y; 	
       //step=1;
		break;
		case 2:
			 if(Curve_Move(1,1000,3000,0,-30))
		    step=2;		   
		break;
		case 3:
			 Curve_Move(1,Hexagon.terminal[2].y,1000,30,-60);
		break;
		case 4:
			 Curve_Move(0,Hexagon.terminal[3].x,1000,-30,0);
		break;
		case 5:
	     Curve_Move(0,Hexagon.terminal[3].x,1000,-30,0);
		break;
		case 6:
			 Curve_Move(0,Hexagon.terminal[3].x,1000,-30,0);
		break;
//			temp_story.x = X;
//		  temp_story.y = Y;		
//		  temp_story.angle = ZAngle;								
	} 
    //Arc_Move()
}
void Correct_Scheme(void)
{
	
}





