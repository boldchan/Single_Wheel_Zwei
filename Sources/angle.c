#define __ANGLE_C_
#include "includes.h"

unsigned int jos[4]={0};
unsigned int ADResult[4]={0};
unsigned int AngleResult[4]={0}; //ǰ��Ƕ�
float AngleCalculate[4]={0};


/*-----------------------------------------------------------------------*/
/*-----------����������ģ���ֵ����-------------*/
/*--------------�ǶȽ��ٶ�-----------------*/
/*-----------------------------------------------------------------------*/
void angle_read(unsigned int *AngleGra)
{
	int i;
	unsigned int temp[4]={0};
	for(i=0;i<20;i++)
	{
		jos[0]=(unsigned int)ADC.CDR[34].B.CDATA;
		temp[0]+=jos[0];//PB10
		jos[1]=(unsigned int)ADC.CDR[35].B.CDATA;
		temp[1]+=jos[1];//PB11
		jos[2]=(unsigned int)ADC.CDR[37].B.CDATA;
		temp[2]+=jos[2];//PD13
		jos[3]=(unsigned int)ADC.CDR[39].B.CDATA;
		temp[3]+=jos[3];//PD15
	}
	ADResult[0]=5000/1024*(temp[0]/20);
	ADResult[1]=5000/1024*(temp[1]/20);
	ADResult[2]=5000/1024*(temp[2]/20);
	ADResult[3]=5000/1024*(temp[3]/20);
	
	for(i=0;i<4;i++)
		*AngleGra++=ADResult[i]; 
}

/*-----------------------------------------------------------------------*/
/*-----------����+ƽ��������ģ�����ݴ���-------------*/
/*-----------�����Ǿ���0.67-----------------*/
/*-----------------------------------------------------------------------*/
void angle_calculate(void)
{

	AngleCalculate[0]=((float)AngleResult[0]-(float)angle_data.PITCH_angle_zero)/5.49; //�����ǵĽǶ�  ǰ��ĽǶ�//1380   
	AngleCalculate[1]=((float)AngleResult[1]-(float)angle_data.PITCH_anglespeed_zero)/0.67;//�����ǽ��ٶ�
	AngleCalculate[2]=((float)AngleResult[2]-(float)angle_data.ROLL_angle_zero)/5; // ���ҽǶ�
	AngleCalculate[3]=((float)AngleResult[3]-(float)angle_data.ROLL_anglespeed_zero)/0.67; //���ҽ��ٶ�//1380
}




void set_PITCH_angle_zero(SWORD zero)
{
	angle_data.PITCH_angle_zero = zero;
}

void set_PITCH_angle_speed_zero(SWORD zero)
{
	angle_data.PITCH_anglespeed_zero = zero;
}

void set_ROLL_angle_zero(SWORD zero)
{
	angle_data.ROLL_angle_zero = zero;
}

void set_ROLL_angle_speed_zero(SWORD zero)
{
	angle_data.ROLL_anglespeed_zero = zero;
}

void set_YAW_angle_zero(SWORD zero)
{
	angle_data.YAW_angle_zero = zero;
}

void set_YAW_angle_speed_zero(SWORD zero)
{
	angle_data.YAW_anglespeed_zero = zero;
}
