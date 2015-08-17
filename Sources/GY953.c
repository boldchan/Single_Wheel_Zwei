#define __GY953_C_
#include "includes.h"

uint8_t GY953_Data[41];

/*------------------------------------------------------------------------------*/
/* 电子罗盘GY953初始化
/*------------------------------------------------------------------------------*/
void init_GY953(void)
{
	GY953_Write(SET_A,0x7D);		//开启磁场计 陀螺仪 加速度计 设置输出速率200HZ
	GY953_Write(CONTROL_B,0x1D);		//自检 校准 高位置1可恢复出厂设置
	GY953_Write(STATE_D,0x0D);		//设置模块精度
}

/*------------------------------------------------------------------------------*/
/* 读取三计校准精度
/* 有数据更新返回1 无新数据返回0 
/* Data[0]加速度计	Data[1]陀螺计	Data[2]磁场计 Data[3]数据输出速率	(低0~4高)
/*------------------------------------------------------------------------------*/
int Read_Precision(BYTE* Data)
{
	BYTE state;
	BYTE set;
	while(!GY953_Read(STATE_C,&state)){};
	while(!GY953_Read(SET_A,&set)){};
	if(state>>7)
	{
		Data[0]=state&0x30;
		Data[0]>>=4;	
		Data[1]=state&0x0C;
		Data[1]>>=2;
		Data[2]=state&0x03;
		Data[3]=set&0x07;
		return 1;
	}
	else
		return 0;
}

/*------------------------------------------------------------------------------*/
/* 读取全部数据
/* 有数据更新返回1 无新数据返回0 
/* GY953 Data[41]
/* 加速度Data[2~7]角速度 Data[8~9]X[10~11]Y[12~13]Z；
/* Data[高20~21低]横滚角； Data[22~23]俯仰角；Data[24~25]航向角；Data[35]精度；
/* Data[36~39]加速度计6个数据寄存器数据累加和 、陀螺仪、磁力计、欧拉角
/*------------------------------------------------------------------------------*/
int Read_GYalldata(BYTE* Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	uint8_t reg=0x01;
	int i=0;
	
	tmp_tx = 0xA8080000|(reg&0x3F)|0xC0;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	for(i=0;i<41;i++)
	{
		tmp_tx = 0x28080000|0xff;
		DSPI_1.PUSHR.R = tmp_tx;
		while(!DSPI_1.SR.B.TCF){}
		tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
		DSPI_1.SR.B.TCF = 1;
		
		Data[i]=tmp_rx;	
		reg++;
	}

	return 1;
}
/*------------------------------------------------------------------------------*/
/* GY953寄存器一次读取 一次8bit
/* 成功返回1 
/*------------------------------------------------------------------------------*/
int GY953_Read(uint8_t reg,uint8_t* Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	
	tmp_tx = 0xA8080000|(reg&0x3F)|0x80;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;

	tmp_tx = 0x28080000|0xff;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	*Data=tmp_rx;
	return 1;
}

/*------------------------------------------------------------------------------*/
/* GY953寄存器连续一次性读取
/* 成功返回1 
/*------------------------------------------------------------------------------*/
int GY953_multi_Read(uint8_t* Data,uint8_t reg)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	int i=0;
	
	for(i=0;i<41;i++)
	{
		tmp_tx = 0xA8080000|(reg&0x3F)|0xC0;
		DSPI_1.PUSHR.R = tmp_tx;
		while(!DSPI_1.SR.B.TCF){}
		tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
		DSPI_1.SR.B.TCF = 1;

		tmp_tx = 0x28080000|0xff;
		DSPI_1.PUSHR.R = tmp_tx;
		while(!DSPI_1.SR.B.TCF){}
		tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
		DSPI_1.SR.B.TCF = 1;
		
		Data[i]=tmp_rx;	
		reg++;
	}

	return 1;
}
/*------------------------------------------------------------------------------*/
/* GY953寄存器写入 一次8bit
/* 成功返回1 
/*------------------------------------------------------------------------------*/
int GY953_Write(uint8_t reg,uint8_t Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	
	tmp_tx = 0xA8080000|(reg&0x3F)|0x40;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	tmp_tx = 0x28080000|Data;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	return 1;
}

int GY953_READ_ACC_GYRO(int16_t *ax,int16_t *ay,int16_t *az,int16_t *gx,int16_t *gy,int16_t *gz)
{
	uint8_t ax_H,ax_L,ay_H,ay_L,az_H,az_L,gx_H,gx_L,gy_H,gy_L,gz_H,gz_L;
	
	GY953_Read(ACC_X_H,&ax_H);
	GY953_Read(ACC_X_L,&ax_L);
	*ax=ax_H;
	*ax=(*ax<<8)|ax_L;

	
	GY953_Read(ACC_Y_H,&ay_H);
	GY953_Read(ACC_Y_L,&ay_L);
	*ay=ay_H;
	*ay=(*ay<<8)|ay_L;

	
	GY953_Read(ACC_Z_H,&az_H);
	GY953_Read(ACC_Z_L,&az_L);
	*az=az_H;
	*az=(*az<<8)|az_L;


	GY953_Read(GYRO_X_H,&gx_H);
	GY953_Read(GYRO_X_L,&gx_L);
	*gx=gx_H;
	*gx=(*gx<<8)|gx_L;

	
	GY953_Read(GYRO_Y_H,&gy_H);
	GY953_Read(GYRO_Y_L,&gy_L);
	*gy=gy_H;
	*gy=(*gy<<8)|gy_L;

	
	GY953_Read(GYRO_Z_H,&gz_H);
	GY953_Read(GYRO_Z_L,&gz_L);
	*gz=gz_H;
	*gz=(*gz<<8)|gz_L;

	
	return 1;
}

int GY953_READ_ACC(float *ax,float *ay,float *az)
{
	uint8_t ax_H,ax_L,ay_H,ay_L,az_H,az_L,gx_H,gx_L,gy_H,gy_L,gz_H,gz_L;
	int16_t axt,ayt,azt;//中间变量
	
	GY953_Read(ACC_X_H,&ax_H);
	GY953_Read(ACC_X_L,&ax_L);
	axt=ax_H;
	axt=(axt<<8)|ax_L;
	*ax=axt*2*9.8/32768;

	
	GY953_Read(ACC_Y_H,&ay_H);
	GY953_Read(ACC_Y_L,&ay_L);
	ayt=ay_H;
	ayt=(ayt<<8)|ay_L;
	*ay=ayt*2*9.8/32768;
	
	GY953_Read(ACC_Z_H,&az_H);
	GY953_Read(ACC_Z_L,&az_L);
	azt=az_H;
	azt=(azt<<8)|az_L;
	*az=azt*2*9.8/32768;
	
	
	return 1;
}

int GY953_READ_Angle(float *yaw,float *pitch,float *roll)
{
	uint8_t yaw_H,yaw_L,pitch_H,pitch_L,roll_H,roll_L;
	int16_t yawt,pitcht,rollt;//中间变量
	
	GY953_Read(YAW_H,&yaw_H);
	GY953_Read(YAW_L,&yaw_L);
	yawt=yaw_H;
	yawt=(yawt<<8)|yaw_L;
	*yaw=yawt/100.0;

	
	GY953_Read(PITCH_H,&pitch_H);
	GY953_Read(PITCH_L,&pitch_L);
	pitcht=pitch_H;
	pitcht=(pitcht<<8)|pitch_L;
	*pitch=pitcht/100.0;
	
	GY953_Read(ROLL_H,&roll_H);
	GY953_Read(ROLL_L,&roll_L);
	rollt=roll_H;
	rollt=(rollt<<8)|roll_L;
	*roll=rollt/100.0;
	
	return 1;
}
int GY953_READ_Quat(float *q1,float *q2,float *q3,float *q4)
{
	uint8_t q1_H,q1_L,q2_H,q2_L,q3_H,q3_L,q4_H,q4_L;
	int16_t q1t,q2t,q3t,q4t;//中间变量
	
	GY953_Read(Q0_H,&q1_H);
	GY953_Read(Q0_L,&q1_L);
	q1t=q1_H;
	q1t=(q1t<<8)|q1_L;
	*q1=q1t/10000.0;
	
	GY953_Read(Q1_H,&q2_H);
	GY953_Read(Q1_L,&q2_L);
	q2t=q2_H;
	q2t=(q2t<<8)|q2_L;
	*q2=q2t/10000.0;
	
	GY953_Read(Q2_H,&q3_H);
	GY953_Read(Q2_L,&q3_L);
	q3t=q3_H;
	q3t=(q3t<<8)|q3_L;
	*q3=q3t/10000.0;
	
	GY953_Read(Q3_H,&q4_H);
	GY953_Read(Q3_L,&q4_L);
	q4t=q4_H;
	q4t=(q4t<<8)|q4_L;
	*q4=q4t/10000.0;
	
	return 1;
}

void GY953_deviation_adjust_accx(float *xdev,float *ydev,float *zdev /*, int32_t *xA*/)		//x方向加速度零位偏差，现暂定xA为500
{
	int i;
	float Xdev=0;
	float Ydev=0;
	float Zdev=0;
	float ax,ay,az;
//	int32_t max;
//	int32_t min;
	for(i=0;i<800;i++)			//取样800次，与后面求平均值要对应
	{
		GY953_READ_ACC(&ax,&ay,&az);
		Xdev=Xdev+ax;
		Ydev=Ydev+ay;
		Zdev=Zdev+az;
//		if(i==0)
//		{
//			max=Data[3];
//			min=Data[3];
//		}
//		else
//		{
//			if(Data[3]>max)
//				max=Data[3];
//			if(Data[3]<min)
//				min=Data[3];
//		}
		delay_ms(5);
		
	}
	*xdev=Xdev/800;	
	*ydev=Ydev/800;
	*zdev=Zdev/800;
//	*xA=(max-*xdev)>(*xdev-min)?(max-*xdev):(*xdev-min);
}

int Sframe2Eframe(float *ax,float *ay,float *az)//将传感器平面的加速度通过四元数转换为导航平面内的加速度
{
	float Sax,Say,Saz,Eax,Eay,Eaz;//S:sensor E:earth
	float q1,q2,q3,q4;//四元数
	GY953_READ_ACC(&Sax,&Say,&Saz);
	GY953_READ_Quat(&q1,&q2,&q3,&q4);
	Eax=(2*q1*q1+2*q2*q2-1)*Sax+2*(q2*q3-q1*q4)*Say+2*(q2*q4+q1*q3)*Saz;
	Eay=2*(q2*q3+q1*q4)*Sax+(2*q1*q1-1+2*q3*q3)*Say+2*(q3*q4-q1*q2)*Saz;
	Eaz=2*(q2*q4-q1*q3)*Sax+2*(q3*q4+q1*q2)+Say*(2*q1*q1-1+2*q4*q4)*Saz;
	*ax=Eax;
	*ay=Eay;
	*az=Eaz;
	
	return 1;
}

