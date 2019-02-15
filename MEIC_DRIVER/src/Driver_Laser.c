#include "Driver_Laser.h"

u16 Get_Adc(u8 ch)   
{
	ADC_RegularChannelConfig(ADC1, ch, 0, ADC_SampleTime_480Cycles );	//ADC3,ADC通道,480个周期,提高采样时间可以提高精确度			     
	ADC_SoftwareStartConv(ADC1);			
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	return ADC_GetConversionValue(ADC1);	
}

float Get_Dis(void)
{
		u32 temp_val = 0;
	  if (Ranged)
		{			
	    temp_val = Get_Adc(ADC_Channel_0);
		  //return ave_Filter(temp_val)*0.22524f + 100.0f;
			return (temp_val*0.22524f + 100.0f)/10.0f;
			//return temp_val;
		}
		else			
			return 65535.0f;		
}



