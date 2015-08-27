#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag=0;
void key_work(void);

void main(void)	  {
	float temp1=0,temp2=0,c1=0;
	int16_t rYAW;//由寄存器读取的YAW
	init_all_and_POST();
	set_speed_target(0);
	for(;;)
	{
		set_key();//按键设置
#if 1
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+2);
			
			D7=~D7;
		}	
		if(g_Control)
		{
			D7=1;
			count++;
			speed_period++;
			/*读取各陀螺仪值*/
			angle_read(AngleResult); 	//读取模拟陀螺仪的AD返回值
			angle_calculate();			 //转换模拟陀螺仪返回值
			Gy953_angle_read();			 //GY953 数字陀螺仪数据读取 陀螺仪数据更新周期5ms
			key_work();					//按键屏幕显示
			/*车身姿态判断*/
			if((AngleCalculate[0]>20||AngleCalculate[0]<-20)||(AngleCalculate[2]>20||AngleCalculate[2]<-20))
			{
				set_PITCH_motor_pwm(0);
				set_PropellerA_motor_pwm(2000);
				set_PropellerB_motor_pwm(-2000);
				g_Control=0;
				continue;
			}
			set_speed_pwm();			//前后速度pwm递增
			AngleControl();				//前后角度控制pwm计算
			PITCH_motor_control();		//前后平衡、速度控制 每毫秒控制  *开屏幕时不可开启驱动板
			if(flagkey4==1||flagkey1==1)				//按键4和按键1不进行左右平衡
			{
				set_PropellerA_motor_pwm(2000);
				set_PropellerB_motor_pwm(-2000);				
			}
			if(count==3&&flagkey3==1||flagkey2==1) 	//5毫秒控制一次  按键2（有显示）、按键3（无显示）有效
			{
				/*	左右控制	*/
				BalanceControl();				//左右平衡控制
				Propeller_YawControl();			//左右航向控制
				PropellerA_Control();			//黑白电机控制
				PropellerB_Control();			//红蓝电机控制
			}
			
			if(count==4)//5毫秒计算一次速度控制
			{
				SpeedCountFlag++;
				if(SpeedCountFlag>=20) 				//达到一百毫秒
				{
					if(flagkey3)
						set_speed_PID();				//根据速度设置PID 标定时关闭
					contorl_speed_encoder_pid(); 	//速度PWM计算
					speed_period=0;					//速度增量值清零
					SpeedCountFlag=0;				//100毫秒计数清零
				}
			}

			
			if(count==5)
			{
//				send_data2PC(ENC03,GYR_TYPE,dall);
				count=0;
			}
			g_Control=0;
			D7=0;
		}

	#endif
		}

}
void key_work(void)
{
	D5=1;
	if(flagkey1==1)
	{
		LCD_PrintoutInt(0, 0, AngleResult[0]);
		LCD_PrintoutInt(64, 0, AngleResult[1]);
		LCD_PrintoutInt(0, 4, AngleCalculate[0]);
		LCD_PrintoutInt(64, 4, AngleCalculate[1]);
	}
	if(flagkey2==1)
	{
		/*	左右控制	*/
		LCD_PrintoutInt(0, 0, AngleResult[2]);
		LCD_PrintoutInt(64, 0, AngleResult[3]);
		LCD_PrintoutInt(0, 4, AngleCalculate[2]);
		LCD_PrintoutInt(64, 4, AngleCalculate[3]);
	}
	if(flagkey3==1)
	{
//				getmax();
//				LCD_PrintoutInt(0, 0, maxep);
//				LCD_PrintoutInt(64, 0, maxen);
//				LCD_PrintoutInt(0,4,maxecp);
//				LCD_PrintoutInt(64,4,maxecn);
	}
	if(flagkey4==1)
	{
		LCD_PrintoutInt(0, 0, GYRead[4]*10);
		LCD_PrintoutInt(64, 0, GYRead[5]*10);
//				EMIOS_0.CH[20].CBDR.R = yaw_pwm;
//				LCD_PrintoutInt(64, 6, yaw_pwm);			
//				Gy953_angle_read(AngleCalculate);
//				LCD_PrintoutInt(0,0,(AngleCalculate[0]*10.0));
//				LCD_PrintoutInt(64,0,AngleCalculate[1]);
//				LCD_PrintoutInt(0,2,(AngleCalculate[2]*10.0));
//				LCD_PrintoutInt(64,2,AngleCalculate[3]);
//				LCD_PrintoutInt(0,4,AngleCalculate[4]);
//				LCD_PrintoutInt(64,4,AngleCalculate[5]);
//				LCD_PrintoutInt(0,0,data_speed_pid.p);
//				LCD_PrintoutInt(0,2,data_speed_pid.i);
//				LCD_PrintoutInt(0,4,data_speed_pid.d);
//				LCD_PrintoutInt(64,0,data_angle_pid.p);
//				LCD_PrintoutInt(64,2,data_angle_pid.i);
//				LCD_PrintoutInt(64,4,data_angle_pid.d*10);
	}
	D5=0;
	
}
