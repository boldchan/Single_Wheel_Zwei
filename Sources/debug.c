#define __DEBUG_C_
#include "includes.h"
int flagkey1=0;
int flagkey2=0;
void set_key(void)
{
	if(key1==0)
	{
		delay_us(30);
		if(key1==0)	
		{
			while(!key1);
			flagkey1=1;
			flagkey2=0;
			/*功能待添加*/
		}
	}
	
	if(key2==0)
	{
		delay_us(30);
		if(key2==0)	
		{
			while(!key2);
			flagkey2=1;
			flagkey1=0;
			/*功能待添加*/
		}
	}
	
	if(key3==0)
	{
		delay_us(30);
		if(key3==0)	
		{
			while(!key3);
			flagkey1=0;
			flagkey2=0;
			LCD_Fill(0x00);
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

