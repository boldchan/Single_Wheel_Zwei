#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#ifndef _TYPEDEF_BYTE_
#define _TYPEDEF_BYTE_
typedef unsigned char BYTE;
#endif

#ifndef _TYPEDEF_SBYTE_
#define _TYPEDEF_SBYTE_
typedef signed char SBYTE;
#endif

#ifndef _TYPEDEF_WORD_
#define _TYPEDEF_WORD_
typedef unsigned short WORD;
#endif

#ifndef _TYPEDEF_SWORD_
#define _TYPEDEF_SWORD_
typedef signed short SWORD;
#endif

#ifndef _TYPEDEF_DWORD_
#define _TYPEDEF_DWORD_
//typedef unsigned int DWORD;
typedef unsigned long int DWORD;
#endif

#if 0
#ifndef _TYPEDEF_byte_
#define _TYPEDEF_byte_
typedef unsigned char byte;
#endif

#ifndef _TYPEDEF_word_
#define _TYPEDEF_word_
typedef unsigned short word;
#endif
#endif

#include "MPC5604B.h"
#include "init.h"
#include "control.h"
#include "time.h"
#include "lcd.h"
#include "UART.h"
#include "IntcInterrupts.h"
#include "stepmotor.h"
#include "angle.h"
#include "MPU9250.h"
#include "analyse.h"
#include "matrix.h"
#include "GY953.h"
#include "Bluetooth.h"
#include "debug.h"




///****************第二版载LED&车灯****************///
#if 1
#define D5 SIU.GPDO[12].B.PDO
#define D6 SIU.GPDO[13].B.PDO
#define D7 SIU.GPDO[14].B.PDO
#define D8 SIU.GPDO[15].B.PDO
#endif


/***************DIP*****************/
#define switch1 !SIU.GPDI[50].R//PD2左
#define switch2 !SIU.GPDI[51].R//PD3
#define switch3 !SIU.GPDI[52].R//PD4
#define switch4 !SIU.GPDI[53].R//PD5右


#endif
