#define __GY953_C_
#include "includes.h"

/*------------------------------------------------------------------------------*/
/* ��������GY953��ʼ��
/*------------------------------------------------------------------------------*/
void init_GY953(void)
{
	GY953_Write(SET_A,0x7D);		//�����ų��� ������ ���ٶȼ� �����������200HZ
	GY953_Write(CONTROL_B,0x1D);		//�Լ� У׼ ��λ��1�ɻָ���������
	GY953_Write(STATE_D,0x0D);		//����ģ�龫��
}

/*------------------------------------------------------------------------------*/
/* ��ȡ����У׼����
/* �����ݸ��·���1 �������ݷ���0 
/* Data[0]���ٶȼ�	Data[1]���ݼ�	Data[2]�ų���	(��0~3��)
/*------------------------------------------------------------------------------*/
int Read_Precision(BYTE* Data)
{
	BYTE state;
	while(!GY953_Read(STATE_C,&state)){};
	if(state>>7)
	{
		Data[0]=state&0x30;
		Data[0]>>=4;	
		Data[1]=state&0x0C;
		Data[1]>>=2;
		Data[2]=state&0x03;
		return 1;
	}
	else
		return 0;
}

/*------------------------------------------------------------------------------*/
/* ��ȡȫ������
/* �����ݸ��·���1 �������ݷ���0 
/* GY953 Data[41]
/* ���ٶ�Data[2~7]���ٶ� Data[8~9]X[10~11]Y[12~13]Z��
/* Data[��20~21��]����ǣ� Data[22~23]�����ǣ�Data[24~25]����ǣ�Data[35]���ȣ�
/* Data[36~39]���ٶȼ�6�����ݼĴ��������ۼӺ� �������ǡ������ơ�ŷ����
/*------------------------------------------------------------------------------*/
int Read_GYalldata(BYTE* Data)
{
	BYTE data[]={0};
	BYTE reg=0x01;
	int i=0;
	for(i=0;i<41;i++)
	{
		while(!GY953_Read(reg,&data[i])){};
		reg++;
	}
	return 1;
}
/*------------------------------------------------------------------------------*/
/* GY953�Ĵ���һ�ζ�ȡ һ��8bit
/* �ɹ�����1 
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
/* GY953�Ĵ�������һ���Զ�ȡ
/* �ɹ�����1 
/*------------------------------------------------------------------------------*/
int GY953_multi_Read(uint8_t* Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	uint8_t reg=0x01;
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
/* GY953�Ĵ���д�� һ��8bit
/* �ɹ�����1 
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

