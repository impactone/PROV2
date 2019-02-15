#include "Driver_Colorlook.h"

#define S12 PEout(5)
#define S13 PEout(6)

#define S22 PCout(4)	
#define S23 PCout(5)	

#define S32 PEout(8)	
#define S33 PEout(9)	

#define S42 PCout(6)
#define S43 PCout(7)

#define S52 PCout(8)
#define S53 PCout(9)
color_sensor  BaseLeft={0,0,0,0,0,0,0,0,0,0,0,baseleft};
color_sensor  BaseRight={0,0,0,0,0,0,0,0,0,0,0,baseright}; 
color_sensor  Select={0,0,0,0,0,0,0,0,0,0,0,select};
color_sensor  Behindleft={0,0,0,0,0,0,0,0,0,0,0,behindleft};
color_sensor  Behindright={0,0,0,0,0,0,0,0,0,0,0,behindright};
//开启红色滤波器
void RedFilter(enum Sensor_Place place)
{	
	 switch(place)
	 { 
		 case baseleft:
		 S12 = 0;
	   S13 = 0;
		 break;
		 case baseright:
		 S22 = 0;
	   S23 = 0;
		 break;
		 case select:
		 S32 = 0;
	   S33 = 0;
		 break;
		 case behindleft:
		 S42 = 0;
	   S43 = 0;
		 break;
		 case behindright:
     S52 = 0;
	   S53 = 0;
		 break;
	 }
}

//开启绿色滤波器
void GreenFilter(enum Sensor_Place place)
{
	 switch(place)
	 { 
		 case baseleft:
		 S12 = 1;
	   S13 = 1;
		 break;
		 case baseright:
		 S22 = 1;
	   S23 = 1;
		 break;
		 case select:
		 S32 = 1;
	   S33 = 1;
		 break;
		 case behindleft:
		 S42 = 1;
	   S43 = 1;		 	  
		 break;
		 case behindright:
     S52 = 1;
	   S53 = 1;
		 break;
	 }
}

//开启蓝色滤波器
void BlueFilter(enum Sensor_Place place)
{
	 switch(place)
	 { 
		 case baseleft:
		 S12 = 0;
	   S13 = 1;
		 break;
		 case baseright:
		 S22 = 0;
	   S23 = 1;
		 break;
		 case select:
		 S32 = 0;
	   S33 = 1;
		 break;
		 case behindleft:
		 S42 = 0;
	   S43 = 1;
		 break;
		 case behindright:
     S52 = 0;
	   S53 = 1;
		 break;
	 }
}
/*每个颜色传感器的处理*/
int max(float x,float y,float z);
void Color_Process(color_sensor *sensor)
{		 	 	
	 if(!sensor->ColorExamOver)
	 {  		   
		  if(sensor->Filter == 0) 
			{				
			  RedFilter(sensor->place);
				if(!sensor->OpenColorExit)
			  {
					sensor->OpenColorExit	=1;		
					sensor->Counter=0;
				}
				sensor->Filter = 1;			
			}
			else if (sensor->Filter == 1)
			{								
				sensor->red=sensor->Counter;
				sensor->Counter = 0;
				sensor->Filter = 2;
				GreenFilter(sensor->place);
			}
			else if(sensor->Filter == 2)
			{			
				sensor->green =sensor->Counter;
				sensor->Counter = 0;
				sensor->Filter = 3;
				BlueFilter(sensor->place);
			}	
      else if(sensor->Filter == 3)
			{						
			  sensor->blue =sensor->Counter;
				sensor->Counter = 0;
				sensor->Filter = 0;								        					
				sensor->ColorExamOver = 1;
				sensor->OpenColorExit = 0;				
			}		
			if(sensor->place == select)
			{  
			  if((sensor->red + sensor->green + sensor->blue) > 100)
				  sensor->color = 5;	
			  else if((sensor->red + sensor->green + sensor->blue) > 12)
				  sensor->color = 6;
			}			
			
				//sensor->color = max(sensor->red*sensor->red_rap,sensor->green*sensor->green_rap,sensor->blue*sensor->blue_rap);			
		}			  
}

int max(float x,float y,float z)
{
	float m ,n;
	if(x>y) m = x;
	else m = y;
	if(m>z) n = m;
	else n = z;
	return(n);
}
