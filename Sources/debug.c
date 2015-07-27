#define __DEBUG_C_
#include "includes.h"

void set_key(void)
{
	if(key1==0)
	{
		delay_us(30);
		if(key1==0)	
		{
			while(!key1);
			angle_data.PITCH_angle_zero++;
			/*功能待添加*/
		}
	}
	
	if(key2==0)
	{
		delay_us(30);
		if(key2==0)	
		{
			while(!key2);
			angle_data.PITCH_anglespeed_zero++;

			/*功能待添加*/
		}
	}
	
	if(key3==0)
	{
		delay_us(30);
		if(key3==0)	
		{
			while(!key3);
			angle_data.PITCH_angle_zero++;
			/*功能待添加*/
		}
	}
	
	if(key4==0)
	{
		delay_us(30);
		if(key4==0)	
		{
			while(!key4);
			angle_data.PITCH_anglespeed_zero++;
			/*功能待添加*/
		}
	}
}

