
#include "main.h"
#include "adc.h"
#include "temperature.h"


uint32_t get_averague(uint32_t * buff, uint32_t n)
{
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t temp = 0;
	uint32_t sum = 0;
	
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			if(buff[i] > buff[j])
			{
				temp = buff[i];
				buff[i] = buff[j];
				buff[j] = temp;
			}
		}
	}
	
	for(i=ADC_REMOVE_CNT; i<n-ADC_REMOVE_CNT; i++)
	{
		sum+=buff[i];
	}
	
	sum = sum/(ADC_MAX_CNT-(ADC_REMOVE_CNT*2));
	
	//sum = buff[ADC_MAX_CNT/2];
	
	return sum;
}

uint32_t adc_val = 0;
float get_temp(void)
{
	

	static float temperate = 0;
	
	if(gAdcReg.stop_flag == 1)
	{
		gAdcReg.stop_flag = 0;
		adc_val = get_averague(gAdcReg.val, ADC_MAX_CNT);  
		temperate = (float)adc_val*(3.3/4096);  
		  
		//temperate = ((1.43-temperate)/0.0043)+25;  
		temperate = ((temperate-0.76)/0.0025)+25;  
		HAL_ADC_Start_IT(&hadc1);
	}
	
	return temperate;
	
}




