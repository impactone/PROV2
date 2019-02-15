#ifndef _DRIVER_CONTROL_H
#define _DRIVER_CONTROL_H
#include "stm32f4xx_it.h"
#include "Driver_Colorlook.h"
#include "io.h"
typedef struct Draw_Circle
{
   float cx;
   float cy;
	 float nx;
	 float ny;	 
	 float radius;
   float angle;
   float direcion;	
	 float speed;
	 float park;
}Cir;
struct Terminal
{
	float x;
	float y;
};
typedef struct draw_hexagon
{
	float cx;
	float cy;
  float radius;  
  struct Terminal terminal[6];
}hexagon;
extern Cir Cir_Draw;
extern hexagon Hexagon;
void electric_task(void);
void steering_task(void);
void select_task(void);
void  USMART_Travel(float distance,float speed);
void USMART_Turn(float p,float i,float d,float expect);
void  USMART_Stop(void);
void USMART_Kinked(float dir,float dis,float ang,float p,float i,float d);
void USMART_Sheet(float number,float angle)	; 
void USMART_Circle(float x,float y,float radius,float angle,float direcion,float speed,float park);
void USMART_ChangeParam(float p1,float p2);
void USMART_Curve(float dir,float boundary,float basespeed,float baseangle,float addangle);
void Closeloop_Speed(float p,float i,float d,float expect,float imax,float maxoutput);
u8 Arc_Move(float sidesway_dir,float sidesway_dis,float sidesway_ang);
void Hexagon_Move(void);
void Cir_Move(void);
u8 Curve_Move(float dir,float boundary,float basespeed,float baseangle,float addangle);
void Usmart_Movement(void);
u8 Error_Admission(float rage,float variable);
void  Angleloop_Process(void);
void WaitForEncoder(void);
#endif



