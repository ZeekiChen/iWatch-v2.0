#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef _SYSTEM_C
#define	SYSTEM_EXTERN
#else
#define	SYSTEM_EXTERN extern
#endif

#include "STC32G.h"
#include "Delay.h"
#include "IIC.h"
#include "SPI.h"
#include "EEPROM.h"
#include "Buzzer.h"
#include "Bluetooth.h"

#include "stdio.h"

#define	KEY1	P43
#define	KEY2	P37
#define	KEY3	P06
#define	KEY4	P07
#define	PWR_SENSE		P32
#define	CHRG_SENSE	P13
#define	EN_3V3			P17

#define	KEY1_PRESSED 0x01
#define	KEY2_PRESSED 0x02
#define	KEY3_PRESSED 0x04
#define	KEY4_PRESSED 0x08
#define	AWT					 0x10
#define	DOUBLE_TAP	 0x20

SYSTEM_EXTERN unsigned char Trg;			//按键触发值
SYSTEM_EXTERN unsigned char Cont;			//按键持续值
SYSTEM_EXTERN unsigned char key_mask;

float GetBatVoltage(void);
void KeyScan(void);
void MCUSoftReset(void);
void EnableWatchDog(void);
void FeedWatchDog(void);
void SystemSleep(void);
void SystemPowerDown(void);
void SystemWake(void);
void SystemInit(void);

#endif