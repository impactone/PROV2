#ifndef Driver_Colorlook_H
#define Driver_Colorlook_H
#include "io.h"

typedef enum Sensor_Place
{	
  	baseleft,
	  baseright,
	  select,
    behindleft,
    behindright	
} Place;  
typedef struct Color_Sensor   
{
  float red_rap;
	float green_rap;  
	float blue_rap;
	u16 red;
	u16 green;
	u16 blue;
	u8  color;   // 1 2 3 4  5 ºì ÂÌ À¶ °× ºÚ
	u16 Counter;
	u8 Filter;	
	u8 ColorExamOver;
	u8 OpenColorExit;		
	Place place;	
} color_sensor;

extern color_sensor  BaseLeft;
extern color_sensor  BaseRight; 
extern color_sensor  Select;
extern color_sensor  Behindleft;
extern color_sensor  Behindright;
void Color_Process(color_sensor *sensor);
#endif





