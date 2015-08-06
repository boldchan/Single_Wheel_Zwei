#define __CONTROL_C_
#include "includes.h"


int g_f_pit = 0;
int g_f_enable_mag_steer_control = 0;
int g_f_enable_speed_control = 0;	/* 启用闭环速度控制标志位 */
int g_f_enable_pwm_control = 0;	/* 启用开环速度控制标志位 */
int speed = 0;
int update_steer_helm_basement_to_steer_helm(void);

int counter=0;

//速度控制全局变量
static float d_speed_pwm=0;
static float speed_pwm=0;
extern unsigned char g_nSpeedControlPeriod;
//平衡控制速度全局变量
static float d_speed_pwm_balance=0;
static float speed_pwm_balance=0;
extern unsigned char g_nSpeedControlPeriod_balance;
//角度控制全局变量
float fDelta;
float g_fCarAngle;
float g_fGyroscopeAngleSpeed;
float g_fGyroscopeTurnSpeed;
float CarAngleInitial=0;
float CarAnglespeedInitial=0;
extern float  AngleCalculate[4];
//左右平衡控制全局变量
float fDelta_balance;
float g_fCarAngle_balance;
float g_fGyroscopeAngleSpeed_balance;
float g_fGyroscopeTurnSpeed_balance;
float CarAngleInitial_balance=0;
float CarAnglespeedInitial_balance=0;

//转向控制全局变量
//float fDelta_balance;
float g_fAngleYaw;
float g_fGyroscopeAngleSpeedYaw;
float EndYawAngle=90;
float EndYawAnglespeed=0;


// float AngleControlOutMax=0.2, AngleControlOutMin=-0.2;

float  PITCH_angle_pwm;
float  ROLL_angle_pwm;


static int new_speed_pwm=0;
static int old_speed_pwm=0;
BYTE speed_period=0;


DWORD tmp_a, tmp_b;


/*-----------------------------------------------------------------------*/
/* 舵机初始化 	                                                                      */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* PIT中断处理函数        10ms                                                         */
/*-----------------------------------------------------------------------*/
void PitISR(void)
{
	get_speed_now();//光编读值
	/* 开始执行速度控制算法 */
	PIT.CH[1].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1
}

/*-----------------------------------------------------------------------*/
/* 获得光编当前速度                                                                    */
/*-----------------------------------------------------------------------*/
void get_speed_now()
{
	/*	光编1读值	*/
	data_encoder1.is_forward = SIU.GPDI[48].B.PDI;//PD0
	data_encoder1.cnt_old = data_encoder1.cnt_new;
	data_encoder1.cnt_new = (WORD)EMIOS_0.CH[0].CCNTR.R;//PA0
	if (data_encoder1.cnt_new >= data_encoder1.cnt_old)
	{
		data_encoder1.speed_now = data_encoder1.cnt_new - data_encoder1.cnt_old;
	}
	else
	{
		data_encoder1.speed_now = 0xffff - (data_encoder1.cnt_old - data_encoder1.cnt_new);
	}
	if(data_encoder1.is_forward==0) 
		data_encoder1.speed_real = 0 - data_encoder1.speed_now;
	else 
		data_encoder1.speed_real = data_encoder1.speed_now;
	
	/*	光编2读值	*/
	data_encoder2.is_forward = SIU.GPDI[49].B.PDI;//PD1
	data_encoder2.cnt_old = data_encoder2.cnt_new;
	data_encoder2.cnt_new = (WORD)EMIOS_0.CH[1].CCNTR.R;//PA1
	if (data_encoder2.cnt_new >= data_encoder2.cnt_old)
	{
		data_encoder2.speed_now = data_encoder2.cnt_new - data_encoder2.cnt_old;
	}
	else
	{
		data_encoder2.speed_now = 0xffff - (data_encoder2.cnt_old - data_encoder2.cnt_new);
	}
	if(data_encoder2.is_forward==0) 
		data_encoder2.speed_real = 0 - data_encoder2.speed_now;
	else 
		data_encoder2.speed_real = data_encoder2.speed_now;
}
/*-----------------------------------------------------------------------*/
/* 设置俯仰电机PWM                                                                    */
/*-----------------------------------------------------------------------*/

void set_PITCH_motor_pwm(int16_t motor_pwm)	//speed_pwm正为向前，负为向后
{
	if (motor_pwm>0)	//forward
	{
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}
		EMIOS_0.CH[21].CBDR.R = motor_pwm;
		EMIOS_0.CH[22].CBDR.R = 1;
		
	}
	else if (motor_pwm<0)	//backward
	{
		motor_pwm = 0-motor_pwm;
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}

		EMIOS_0.CH[21].CBDR.R = 1;
		EMIOS_0.CH[22].CBDR.R = motor_pwm;	
	}
	else
	{
		EMIOS_0.CH[21].CBDR.R = 1;
		EMIOS_0.CH[22].CBDR.R = 1;	
	}
}
void PITCH_motor_control(void)
{
	int16_t motor_pwm;

	motor_pwm=PITCH_angle_pwm-speed_pwm;

//	motor_pwm=speed_pwm;
	set_PITCH_motor_pwm(motor_pwm);
}

//void motor_control_balance(void)
//{
//	int16_t motor_pwm_balance;
//	motor_pwm_balance=angle_pwm_balance;
//	set_motor_pwm(angle_pwm_balance);
//}
#if 0
/*-----------------------------------------------------------------------*/
/* 设置平衡电机PWM                                                                    */
/*-----------------------------------------------------------------------*/
void set_ROLL_motor_pwm(int16_t motor_pwm)	//speed_pwm正为向前，负为向后
{
	if (motor_pwm>0)	//forward
	{
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}
		EMIOS_0.CH[19].CBDR.R = motor_pwm;
		EMIOS_0.CH[20].CBDR.R = 1;
		
	}
	else if (motor_pwm<0)	//backward
	{
		motor_pwm = 0-motor_pwm;
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}

		EMIOS_0.CH[19].CBDR.R = 1;
		EMIOS_0.CH[20].CBDR.R = motor_pwm;	
	}
	else
	{
		EMIOS_0.CH[19].CBDR.R = 1;
		EMIOS_0.CH[20].CBDR.R = 1;	
	}
}
#endif
/*----设置螺旋桨电机A MOTOR3 黑白线电机-------*/
void set_PropellerA_motor_pwm(int16_t motor_pwm)	
{
	//使用PE3 PE4
	motor_pwm=900+motor_pwm;
	if (motor_pwm>0)	
	{
		if (motor_pwm>1800)//最高电压7.2V
		{
			motor_pwm = 1800;
		}
//		else if(motor_pwm<250)//死区电压1V
//		{
//			motor_pwm=250;
//		}
		EMIOS_0.CH[19].CBDR.R = 1;//PE3
		EMIOS_0.CH[20].CBDR.R = motor_pwm;//PE4
		
	}
	else 	
	{
		EMIOS_0.CH[19].CBDR.R = 1;
		EMIOS_0.CH[20].CBDR.R = 1;	
	}
}

/*---设置螺旋桨电机B MOTOR2 红蓝线电机---*/
void set_PropellerB_motor_pwm(int16_t motor_pwm)	
{
	//暂时使用PE1,PE2
	motor_pwm=900-motor_pwm;
	if (motor_pwm>0)	
	{
		if (motor_pwm>1800)//最高电压7.2V
		{
			motor_pwm = 1800;
		}
//		else if(motor_pwm<250)//死区电压1V
//		{
//			motor_pwm=250;
//		}
		EMIOS_0.CH[18].CBDR.R = motor_pwm;//PE2
		EMIOS_0.CH[17].CBDR.R = 1;//PE1
		
	}
	else 	
	{
		EMIOS_0.CH[17].CBDR.R = 1;
		EMIOS_0.CH[18].CBDR.R = 1;	
	}
}
void PropellerA_Control(void)
{
	int16_t motor_pwm;
	motor_pwm=ROLL_angle_pwm;
	set_PropellerA_motor_pwm(motor_pwm);
}
void PropellerB_Control(void)
{
	int16_t motor_pwm;
	motor_pwm=ROLL_angle_pwm;
	set_PropellerB_motor_pwm(motor_pwm);
}


/*-----------------------------------------------------------------------*/
/* 设置转向电机PWM                                                                    */
/*-----------------------------------------------------------------------*/
void set_YAW_motor_pwm(int16_t motor_pwm)	//speed_pwm正为向前，负为向后
{
	if (motor_pwm>0)	//forward
	{
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}
		EMIOS_0.CH[17].CBDR.R = motor_pwm;
		EMIOS_0.CH[18].CBDR.R = 1;
		
	}
	else if (motor_pwm<0)	//backward
	{
		motor_pwm = 0-motor_pwm;
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}

		EMIOS_0.CH[17].CBDR.R = 1;
		EMIOS_0.CH[18].CBDR.R = motor_pwm;	
	}
	else
	{
		EMIOS_0.CH[17].CBDR.R = 1;
		EMIOS_0.CH[18].CBDR.R = 1;	
	}
}
#if 0
void ROLL_motor_control(void)
{
	int16_t motor_pwm;
	motor_pwm=ROLL_angle_pwm;
	set_ROLL_motor_pwm(motor_pwm);
}
#endif


/*-----------------------------------------------------------------------*/
/* 前后角度控制                                                             */
/*-----------------------------------------------------------------------*/
void AngleControl(void)

{  float delta_angle;
   float delta_anglespeed;
   float temp_angle, temp_anglespeed;
   float currentanglespeed, lastanglespeed=0;
   float last_angle=0;
   //angle_calculate();
   g_fCarAngle= AngleCalculate[0];
   g_fGyroscopeAngleSpeed= -AngleCalculate[1];
 // g_fGyroscopeTurnSpeed= AngleCalculateResult[2];
 
   temp_angle=CarAngleInitial - g_fCarAngle;
   temp_anglespeed= CarAnglespeedInitial - g_fGyroscopeAngleSpeed;
  
//   if(temp_angle<-15)
//	   data_angle_pid.p=150;	//150//100开环
//   else if(temp_angle>=-15&temp_angle<=0)
//	   data_angle_pid.p=260;	//230//200开环
//   else if(temp_angle>0&temp_angle<=15)
//	   data_angle_pid.p=260;	//230//200开环
//   else
//	   data_angle_pid.p=150;	//150//100开环
//                                                    
//  
//   if(temp_anglespeed>=50||temp_anglespeed<=-50)
//	   data_angle_pid.d=2;//0.3
//   else
//	   data_angle_pid.d=0.5;//0.1 
   delta_angle = data_angle_pid.p*(CarAngleInitial - g_fCarAngle);
   delta_angle+=data_angle_pid.d*(CarAnglespeedInitial - g_fGyroscopeAngleSpeed);

  //delta_angle = data_angle_pid.p*(CarAngleInitial - g_fCarAngle) /5000 +data_angle_pid.d*(CarAnglespeedInitial - g_fGyroscopeAngleSpeed) /15000; // 1000 与10000是否根据实际需要调整 
  //angle_pwm=delta_angle;
    PITCH_angle_pwm=delta_angle;
  
}

/*-----------------------------------------------------------------------*/
/* 左右平衡控制     by-LZY                                                        */
/*-----------------------------------------------------------------------*/
void BalanceControl(void)
{
	float delta_angle_balance;
	float delta_anglespeed_balance;
	float temp_angle_balance, temp_anglespeed_balance;
	static float currentanglespeed_balance, lastanglespeed_balance=0;
	float last_angle_balance=0;
	float temp_p,temp_d;
	//angle_calculate();
	g_fCarAngle_balance= AngleCalculate[2];
	g_fGyroscopeAngleSpeed_balance=-AngleCalculate[3];
	 
	temp_angle_balance=CarAngleInitial_balance - g_fCarAngle_balance;
	temp_anglespeed_balance= CarAnglespeedInitial_balance - g_fGyroscopeAngleSpeed_balance;
	if(g_fCarAngle_balance>7||g_fCarAngle_balance<-7)
	{
		temp_d=0;
		temp_p=400;
	}
	else
	{
		temp_p=data_ROLL_angle_pid.p;
		temp_d=data_ROLL_angle_pid.d;
	}	
	//	currentanglespeed_balance=g_fCarAngle_balance;
	//	delta_anglespeed_balance=currentanglespeed_balance-lastanglespeed_balance;
	//	lastanglespeed_balance=currentanglespeed_balance;
		  
	delta_angle_balance = temp_p*(CarAngleInitial_balance - g_fCarAngle_balance);
	delta_angle_balance+=temp_d*(CarAnglespeedInitial_balance - g_fGyroscopeAngleSpeed_balance);
	//	delta_angle_balance+=data_speed_pid.d*0.4*delta_anglespeed_balance;	  
	//angle_pwm_balance=dta_angle;
	ROLL_angle_pwm=delta_angle_balance;
	LCD_PrintoutInt(0, 2, ROLL_angle_pwm);
}

/*-----------------------------------------------------------------------*/
/* 转向控制      by TGC                                                        */
/*-----------------------------------------------------------------------*/

void YawControl(void)
{
	//int i;
	//float temp_angle, temp_anglespeed;
	float delta_yaw;
	
	g_fAngleYaw=AngleCalculate[2];
	g_fGyroscopeAngleSpeedYaw=AngleCalculate[3];

	delta_yaw=data_YAW_angle_pid.p*(EndYawAngle-g_fAngleYaw);
	delta_yaw+=data_YAW_angle_pid.d*(EndYawAnglespeed-g_fGyroscopeAngleSpeedYaw);
	
	set_YAW_motor_pwm(delta_yaw);
	
	/*set_YAW_motor_pwm(500);
	delay_ms(3000);
	set_YAW_motor_pwm(400);
	delay_ms(1000);
	set_YAW_motor_pwm(300);
	delay_ms(1000);
	set_YAW_motor_pwm(200);
	delay_ms(1000);
	set_YAW_motor_pwm(100);
	delay_ms(1000);
	set_YAW_motor_pwm(0);
	delay_ms(1000);
	
	set_YAW_motor_pwm(1000);
		delay_ms(1000);
	for(i=19;i>=0;i--)
	{
		int j=i*50;
		set_YAW_motor_pwm(j);
		delay_ms(500);
		
	}
	
	//set_YAW_motor_pwm(0);
		delay_ms(6000);
	*/
	
	
}






/*-----------------------------------------------------------------------*/
/* 设置方向舵机位置                                                                */
/* 对于白色信号线的舵机：                                                       */
/* 面对舵机轴，占空比增大，舵机逆时针旋转，对我们的车是左舵    */
/* 对于橙色信号线的舵机：                                                       */
/* 相反                                                                                  */
/* 直接方向舵机寄存器                                                             */
/* 有限幅                                                                               */
/*-----------------------------------------------------------------------*/
void set_steer_helm_basement(WORD helmData)
{
	if(helmData <= 1000)
	{
		helmData = 1000;
	}
	else if(helmData >= 6000)
	{
		helmData = 6000;
	}
	EMIOS_0.CH[9].CBDR.R = helmData;
}


/*-----------------------------------------------------------------------*/
/* 获得速度偏差                                                                      */
/* 有问题找叶川                                                                      */
/*-----------------------------------------------------------------------*/
static SWORD get_e0()
{
	SWORD tmp_speed_now;
	SWORD e0;
	if (data_encoder1.is_forward)
	{
		tmp_speed_now =(SWORD) data_encoder1.speed_now;
	}
	else
	{
		tmp_speed_now = 0 - (SWORD) data_encoder1.speed_now;
	}
	e0=data_speed_settings.speed_target-tmp_speed_now;
	return e0;
	
}

/*-----------------------------------------------------------------------*/
/* PID速度控制                                                                       */
/* 有问题找叶川                                                                      */                                                          
/*-----------------------------------------------------------------------*/
void contorl_speed_encoder_pid(void)
{
	int error=0;
	int kp,ki,kd;
	static SWORD error_last=0;
	static SWORD sum_error=0;
	error_last = error;
	error = data_speed_settings.speed_target - data_encoder1.speed_real;
	

	old_speed_pwm = new_speed_pwm;
	kp=(SWORD)(data_speed_pid.p*(error));       //P控制
	new_speed_pwm=kp;
	kd=(SWORD)(data_speed_pid.d*(error-error_last));  //I控制
	new_speed_pwm+=kd;
	sum_error+=error;
	if(sum_error>500) sum_error=500;
	if(sum_error<-500) sum_error=-500;
	ki=(SWORD)(data_speed_pid.i*(sum_error));	
	new_speed_pwm+=ki;
	
	if(new_speed_pwm>1000)
		new_speed_pwm=1000;
	if(new_speed_pwm<-1000)
		new_speed_pwm=-1000;   //限制pwm变化量
	
//	LCD_PrintoutInt(0, 6, error);
//	LCD_PrintoutInt(0, 0, kp);
//	LCD_PrintoutInt(0, 2, kd);
//	LCD_PrintoutInt(0, 4, ki);	
//	LCD_PrintoutInt(64, 4, sum_error);
//	LCD_PrintoutInt(65, 6, new_speed_pwm);
}
void set_speed_pwm(void)
{
	speed_pwm = (d_speed_pwm/100)*(speed_period)+old_speed_pwm;
	d_speed_pwm = new_speed_pwm - old_speed_pwm;
//	LCD_PrintoutInt(0, 6, data_encoder.speed_real);
}
void set_speed_pwm_balance(void)
{
	speed_pwm_balance+=(d_speed_pwm_balance/100);
}
/*-----------------------------------------------------------------------*/
/* 设置目标速度                                                                      */
/*-----------------------------------------------------------------------*/
void set_speed_target(SWORD speed_target)
{
	data_speed_settings.speed_target = speed_target;
}

/*-----------------------------------------------------------------------*/
/* 设置驱动轮电机目标占空比                                                                      */
/*-----------------------------------------------------------------------*/
void set_pwm1_target(SWORD speed_pwm)
{
	motor_pwm_settings.motor_1_pwm = speed_pwm;
}
/*-----------------------------------------------------------------------*/
/* 设置平衡轮电机目标占空比                                                                      */
/*-----------------------------------------------------------------------*/
void set_pwm2_target(SWORD speed_pwm)
{
	motor_pwm_settings.motor_2_pwm = speed_pwm;
}

/*-----------------------------------------------------------------------*/
/* 设置转向轮电机目标占空比                                                                      */
/*-----------------------------------------------------------------------*/
void set_pwm3_target(SWORD speed_pwm)
{
	motor_pwm_settings.motor_3_pwm = speed_pwm;
}

/*-----------------------------------------------------------------------*/
/* 设置速度PID控制PID值 根据目标速度设置 speed_now                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_PID(void) 
{ 
	
	if(data_speed_settings.speed_target==0)
	{
		data_speed_pid.p=20;
	}
	else if(data_speed_settings.speed_target<10)
	{
		data_speed_pid.p=50;
	}
	else if(data_speed_settings.speed_target<20)
	{
		data_speed_pid.p=45;
	}
	/*int speed_target=data_speed_settings.speed_target;
	int speed_now=data_speed_settings.speed_target_now;
	if(speed_target==0)//420 
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;         
	}
	else if(speed_target>=450&&speed_target<=500)//420 
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;          

		if(speed_now<speed_target)
			speed_now+=25;
		else if(speed_now>speed_target)
			speed_now-=25;
		else
			speed_now=speed_target;
	}

	else if(speed_target>500&&speed_target<=560)//420 
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;         
		if(speed_now<speed_target)
			speed_now+=30;          //30可调 值越大初始时速度变化快
		else if(speed_now>speed_target)
			speed_now-=30;
		else
			speed_now=speed_target;
	} 

	else if(speed_target>560&&speed_target<=620 )
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;          
		if(speed_now<speed_target)
			speed_now+=30;
		else if(speed_now>speed_target)
			speed_now-=30;
		else
			speed_now=speed_target;
	} 

	else
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;          
		if(speed_now<speed_target)
			speed_now+=30;
		else if(speed_now>speed_target)
			speed_now-=30;
		else
			speed_now=speed_target;
	} 
*/
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制P值                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_KP(float kp)
{
	data_speed_pid.p = kp;
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制I值                                                             */
/*-----------------------------------------------------------------------*/
void set_speed_KI(float ki)
{
	data_speed_pid.i = ki;
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制D值                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_KD(float kd)
{
	data_speed_pid.d = kd;
}

/*-----------------------------------------------------------------------*/
/* 设置角度PID控制P值                                                            */
/*-----------------------------------------------------------------------*/
void set_angle_KP(float kp)
{
	data_angle_pid.p = kp;
}


/*-----------------------------------------------------------------------*/
/* 设置角度PID控制I值                                                             */
/*-----------------------------------------------------------------------*/
void set_angle_KI(float ki)
{
	data_angle_pid.i = ki;
}


/*-----------------------------------------------------------------------*/
/* 设置角度PID控制D值                                                            */
/*-----------------------------------------------------------------------*/
void set_angle_KD(float kd)
{
	data_angle_pid.d = kd;
}

/*-----------------------------------------------------------------------*/
/* 设置平衡PID控制P值                                                            */
/*-----------------------------------------------------------------------*/
void set_ROLL_KP(float kp)
{
	data_ROLL_angle_pid.p = kp;
}


/*-----------------------------------------------------------------------*/
/* 设置平衡PID控制I值                                                             */
/*-----------------------------------------------------------------------*/
void set_ROLL_KI(float ki)
{
	data_ROLL_angle_pid.i = ki;
}


/*-----------------------------------------------------------------------*/
/* 设置平衡PID控制D值                                                            */
/*-----------------------------------------------------------------------*/
void set_ROLL_KD(float kd)
{
	data_ROLL_angle_pid.d = kd;
}
/*-----------------------------------------------------------------------*/
/* 获取两个周期计数的差值，常用故写成函数                               */
/*-----------------------------------------------------------------------*/
DWORD diff_time_basis_PIT(const DWORD new_time, const DWORD old_time)
{
	DWORD diff;
	
	if (new_time >= old_time)
	{
		diff = new_time - old_time;
	}
	else
	{
		diff = new_time + (0xFFFFFFFF- old_time);
	}
	
	return diff;
}
#if 0
int abs(int data)
{
	if (data<0)
		data=0-data;
	return data;
}
#endif
