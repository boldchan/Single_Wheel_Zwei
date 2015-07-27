
#ifndef GY953_H_
#define GY953_H_

#define SET_A 0x01//���üĴ���A
#define CONTROL_B 0x02//���ƼĴ���B
#define STATE_D 0x23//״̬�Ĵ���D
#define STATE_C 0x24//���ƾ���״̬�Ĵ��� 
#define ACC_X_H 0x03
#define ACC_X_L 0x04





void init_GY953(void);
int GY953_Read(uint8_t reg,uint8_t* Data);
int GY953_multi_Read(uint8_t* Data);
int GY953_Write(uint8_t reg,uint8_t Data);
int Read_Precision(BYTE* Data);	
int Read_GYalldata(BYTE* Data);






#endif /* GY953_H_ */
