#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag=0;

void main(void)
  {
	int16_t rYAW;//由寄存器读取的YAW
	init_all_and_POST();
	//set_speed_target(0);
	for(;;)
	{
		set_key();//按键设置

#if 1
//		Read_GYalldata(GY953_Data);
//		rYAW=GY953_Data[24];
//		rYAW=(rYAW<<8)|GY953_Data[25];
//		g_fAngleYaw=rYAW/100;
//		
//		send_data2PC(3,ANGLE_TYPE,GY953_Data);
//		LCD_PrintoutFloat(60,7,g_fAngleYaw);
//		send_data2PC(3,GYR_TYPE,GY953_Data);
//		YawControl(); //写在pit中断中
		delay_ms(5);
		
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+2);
			
			D7=~D7;
		}	
		if(g_Control)
		{
			g_Control=0;
			count++;
			D6=~D6;	
			speed_period++;
			angle_read(AngleResult);
			angle_calculate();
#if 1
			set_speed_pwm();
			AngleControl();
			BalanceControl();
			if(flagkey2==1)
			{
				LCD_PrintoutInt(0, 0, AngleResult[2]);
				LCD_PrintoutInt(64, 0, AngleResult[3]);
				LCD_PrintoutInt(0, 4, AngleCalculate[2]);
				LCD_PrintoutInt(64, 4, AngleCalculate[3]);
			}
			if(flagkey1==1)
			{
				LCD_PrintoutInt(0, 0, AngleResult[0]);
				LCD_PrintoutInt(64, 0, AngleResult[1]);
				LCD_PrintoutInt(0, 4, AngleCalculate[0]);
				LCD_PrintoutInt(64, 4, AngleCalculate[1]);
			}
			if(flagkey3==1)
			{
				getmax();
				LCD_PrintoutInt(0, 0, maxep);
				LCD_PrintoutInt(64, 0, maxen);
				LCD_PrintoutInt(0,4,maxecp);
				LCD_PrintoutInt(64,4,maxecn);
			}
			
//			LCD_PrintoutInt(0, 0, data_ROLL_angle_pid.p);
//			LCD_PrintoutInt(64, 0, data_ROLL_angle_pid.d);
//			LCD_PrintoutInt(0, 0, data_speed_pid.p);
//			LCD_PrintoutInt(64, 0, data_speed_pid.d);
			

			/*	前后控制	*/
			if(AngleCalculate[0]<20&&AngleCalculate[0]>-20)
			{ 
				PITCH_motor_control();

			} 
			else
			{
				set_PITCH_motor_pwm(0);
			}
#endif
//			LCD_PrintoutInt(0, 4, AngleResult[2]);
//			LCD_PrintoutInt(64, 4, AngleResult[3]);
			if(AngleCalculate[2]<20&&AngleCalculate[2]>-20)
			{ 
				//ROLL_motor_control();
				PropellerA_Control();
				PropellerB_Control();
//				Bangbang_balance_control();
			} 
			else
			{
				set_PropellerA_motor_pwm(-2000);
				set_PropellerB_motor_pwm(2000);
			}
//			if(count==3)
//			{
//				/*	平衡控制	*/ 
//				if(AngleCalculate[2]<20&&AngleCalculate[2]>-20)
//				{ 
//					Balance_Control_HELM();//【大保健看这里：想用上位机调要用WIFI的接收 见For C CUP】
//				} 
//				else
//				{
//					set_steer_helm_basement(3000);
//				}
//			}

			if(count==4)
			{
				SpeedCountFlag++;
				get_speed_now();
				if(SpeedCountFlag>=20) 
				{
					set_speed_PID();
					contorl_speed_encoder_pid();
					speed_period=0;
//					LCD_PrintoutInt(0, 4, data_speed_pid.p);
//					LCD_PrintoutInt(64, 4, data_speed_pid.d);
//					LCD_PrintoutInt(0, 4, data_speed_settings.speed_target);
					LCD_PrintoutInt(65, 4, data_encoder1.speed_real);
					SpeedCountFlag=0;
				}
			}

			
			if(count==5)
			{
//				send_data2PC(ENC03,GYR_TYPE,dall);
				count=0;
			}

		}

	#endif
		}

}

