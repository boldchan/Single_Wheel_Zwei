#define __STEPMOTOR_C_
#include "includes.h"

int stepspeed=1000;
BYTE g_Control;
int timer=0;
int speed_count=0;

void Pit_1ms(void) 
{
	D6=1;
	//static int timer=0;
	//static int ABCD=0;
	//static int count=0;
	g_Control=1;
	speed_count++;
	if(speed_count==5)
	{
		speed_count=0;
		get_speed_now();
	}
	PIT.CH[2].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1
	D6=0;
}




