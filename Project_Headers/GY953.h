
#ifndef GY953_H_
#define GY953_H_

#define SET_A 0x01//���üĴ���A
#define CONTROL_B 0x02//���ƼĴ���B
#define STATE_D 0x23//״̬�Ĵ���D
#define STATE_C 0x24//���ƾ���״̬�Ĵ��� 
#define ACC_X_H 0x03
#define ACC_X_L 0x04
#define ACC_Y_H 0x05
#define ACC_Y_L 0x06
#define ACC_Z_H 0x07
#define ACC_Z_L 0x08
#define GYRO_X_H 0x09
#define GYRO_X_L 0x0A
#define GYRO_Y_H 0x0B
#define GYRO_Y_L 0x0C
#define GYRO_Z_H 0x0D
#define GYRO_Z_L 0x0E
#define Q0_H 0x1B
#define Q0_L 0x1C
#define Q1_H 0x1D
#define Q1_L 0x1E
#define Q2_H 0x1F
#define Q2_L 0x20
#define Q3_H 0x21
#define Q3_L 0x22
#define ROLL_H 0x15
#define ROLL_L 0x16
#define PITCH_H 0x17
#define PITCH_L 0x18
#define YAW_H 0x19
#define YAW_L 0x1A



extern uint8_t GY953_Data[41];

extern void init_GY953(void);
extern int GY953_Read(uint8_t reg,uint8_t* Data);
extern int GY953_multi_Read(uint8_t* Data,uint8_t reg);
extern int GY953_Write(uint8_t reg,uint8_t Data);
extern int Read_Precision(BYTE* Data);	
extern int Read_GYalldata(BYTE* Data);
extern int GY953_READ_ACC_GYRO(int16_t *ax,int16_t *ay,int16_t *az,int16_t *gx,int16_t *gy,int16_t *gz);
extern int GY953_READ_ACC(float *ax,float *ay,float *az);
extern int GY953_READ_Quat(float *q1,float *q2,float *q3,float *q4);
extern void GY953_deviation_adjust_accx(float *xdev,float *ydev,float *zdev /*, int32_t *xA*/);	
extern int GY953_READ_Angle(float *yaw,float *pitch,float *roll);
extern int Sframe2Eframe(float *ax,float *ay,float *az);





#endif /* GY953_H_ */
