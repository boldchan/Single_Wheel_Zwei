#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag=0;

void main(void)
{
 #if 0
	float Eax,Eay,Eaz;//E:earth
	float vy=0,sy=0;//y方向(前进方向)上的速度和距离
	float deltat=0.003;
	float xdev,ydev,zdev;
	float pitch,roll,yaw;
	init_all_and_POST();
	//GY953_deviation_adjust_accx(&xdev,&ydev,&zdev);
	GY953_READ_Angle(&yaw,&pitch,&roll);
	LCD_PrintoutFloat(60,1,pitch);

	for(;;)
	{
		D5=0;
		Sframe2Eframe(&Eax,&Eay,&Eaz);
//		Sax-=xdev;
//		Say-=ydev;
//		Saz-=zdev;

		send_acc(Eay,0xBB);
		vy+=Eay*deltat;
		sy+=vy*deltat;
		//LCD_PrintoutFloat(60,1,Eay);
		//LCD_PrintoutFloat(60,3,vy);
		//LCD_PrintoutFloat(60,5,sy);
		D5=1;
		delay_ms(1);
	}
#endif

#if 0
	//int16_t ax,ay,az,gx,gy,gz;//g:gyro,a:acc
	//float axf,ayf,azf,gxf,gyf,gzf;
	//uint8_t q0_H,q0_L,q1_H,q1_L,q2_H,q2_L,q3_H,q3_L;
	//int16_t mYaw,mPitch,mRoll;
	float Yaw,Pitch,Roll;
	uint8_t num[4]={0};
	//float Angle[6];
	init_all_and_POST();
	for(;;)
	{
		//Read_GYalldata(GY953_Data);
//		GY953_READ_ACC_GYRO(&ax,&ay,&az,&gx,&gy,&gz);
//		axf=ax;ayf=ay;azf=az;gxf=gx;gyf=gy;gzf=gz;
//		filterUpdate(gxf,gyf,gzf,axf,ayf,azf);
//		
//		SEq_1*=10000;
//		SEq_2*=10000;
//		SEq_3*=10000;
//		SEq_4*=10000;
//		q0_L=(int16_t)SEq_1;
//		q0_H=(int16_t)SEq_1>>8;
//		q1_L=(int16_t)SEq_2;
//		q1_H=(int16_t)SEq_2>>8;
//		q2_L=(int16_t)SEq_3;
//		q2_H=(int16_t)SEq_3>>8;
//		q3_L=(int16_t)SEq_4;
//		q3_H=(int16_t)SEq_4>>8;
//		GY953_Data[26]=q0_H;
//		GY953_Data[27]=q0_L;
//		GY953_Data[28]=q1_H;
//		GY953_Data[29]=q1_L;
//		GY953_Data[30]=q2_H;
//		GY953_Data[31]=q2_L;
//		GY953_Data[32]=q3_H;
//		GY953_Data[33]=q3_L;

//		send_data2PC(3,ACC_TYPE,GY953_Data);
//		send_data2PC(3,FOUR_TYPE,GY953_Data);
		if(num[2]==3)
		{
			GY953_READ_Angle(&Yaw,&Pitch,&Roll);
			LCD_PrintoutFloat(60,1,Yaw);
			LCD_PrintoutFloat(60,3,Pitch);
			LCD_PrintoutFloat(60,5,Roll);
		}
		if(g_remote_frame_state)
		{
			switch(remote_frame_data[1])
			{
			case 0x57:
				GY953_Write(0x02,0x15);
				break;
			case 0x58:
				GY953_Write(0x02,0x19);
                while(num[2]!=3)
				{
					Read_Precision(num);
				}
				break;
			case 0x75:
				Read_Precision(num);
				generate_remote_frame_2(PREC_TYPE,4,num);
				break;
			default:
				break;
			}
			g_remote_frame_state=0;
		}
		
//		mYaw=atan2(2*(SEq_1*SEq_4+SEq_2*SEq_3),1-2*(SEq_3*SEq_3+SEq_4*SEq_4));
//		mPitch=asin(2*(SEq_1*SEq_3-SEq_2*SEq_4));
//		mRoll=atan2(2*(SEq_1*SEq_2+SEq_3*SEq_4),1-2*(SEq_2*SEq_2+SEq_3*SEq_3));
//		
//		mYaw*=100;
//		mPitch*=100;
//		mRoll*=100;
//		
//		Angle[1]=mRoll;
//		Angle[0]=mRoll>>8;
//		Angle[3]=mPitch;
//		Angle[2]=mPitch>>8;
//		Angle[5]=mYaw;
//		Angle[4]=mYaw>>8;
//		generate_remote_frame_2( ANGLE_TYPE, 6, (const BYTE *)(&Angle[0]));
		delay_ms(5);
	}
#endif
	
#if 0//fusion algorithm
//	int16_t rYAW;//由寄存器读取的YAW
	uint8_t num[4]={0};
	float q0,q1,q2,q3;
	int16_t mYaw,mPitch,mRoll;
	float Angle[6];
	float ax,ay,az,gx,gy,gz;
	float T32,T31,T33,T12,T22;
	q0=1;
	q1=0;
	q2=0;
	q3=0;//四元数初始化
	init_all_and_POST();
	//set_speed_target(0);
	for(;;)
	{
		GY953_READ_ACC_GYRO(&ax,&ay,&az,&gx,&gy,&gz);
		MadgwickAHRSupdateIMU(gx,gy,gz,ax,ay,az,&q0,&q1,&q2,&q3);
		T32=2*(q2*q3-q0*q1);
		T31=2*(q1*q3+q2*q2);
		T33=q0*q0-q1*q1-q2*q2+q3*q3;
		T12=2*(q1*q3-q0*q2);
		T22=q0*q0-q1*q1+q2*q2-q3*q3;
		mPitch=100*asin(T32);
		mYaw=100*atan(-T31/T33);
		mRoll=100*atan(T12/T22);
		Angle[0]=mRoll;
		Angle[1]=mRoll>>8;
		Angle[2]=mPitch;
		Angle[3]=mPitch>>8;
		Angle[4]=mYaw;
		Angle[5]=mYaw>>8;
		generate_remote_frame_2( ANGLE_TYPE, 6, (const BYTE *)(&Angle[0]));
		delay_ms(2);
	}
#endif
		
#if 1	//GY953 校准
		int16_t rYAW;//由寄存器读取的YAW
		uint8_t num[4]={0};
		init_all_and_POST();
		for(;;)
		{

		if(g_remote_frame_state)
		{
			switch(remote_frame_data[1])
			{
			case 0x57:
				GY953_Write(0x02,0x15);
				break;
			case 0x58:
				GY953_Write(0x02,0x19);
                while(num[2]!=3)
				{
					Read_Precision(num);
				}
				break;
			case 0x75:
				Read_Precision(num);
				generate_remote_frame_2(PREC_TYPE,4,num);
				break;
			default:
				break;
			}
			g_remote_frame_state=0;
		}
		if(num[2]==3)                                                                 
		{
			Read_GYalldata(GY953_Data);
			send_data2PC(3,ANGLE_TYPE,GY953_Data);
			//send_data2PC(3,ACC_TYPE,GY953_Data);
			//send_data2PC(3,FOUR_TYPE,GY953_Data);
			rYAW=GY953_Data[24];
			rYAW=(rYAW<<8)|GY953_Data[25];
			g_fAngleYaw=rYAW/100;
			LCD_PrintoutFloat(60,1,g_fAngleYaw);
		}
		//set_key();//按键设置
		//YawControl(); //写在pit中断中
		delay_ms(5);
		}
		
#endif
		
#if 0
		init_all_and_POST();
		set_speed_target(0);
		for(;;)
		{
			set_key();
			YawControl();
#if 0
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
#endif

}

