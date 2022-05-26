#ifndef _IWATCH_H
#define _IWATCH_H

#include "System.h"
#include "RX8130CE.h"
#include "OLED.h"
#include "oled_gui.h"
#include "pageManager.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "LSM6DSM.h"
/***********ѡ����ʹ�õ���ʪ�ȴ�����**********/
#define	TH_SENSOR_SELECT	1
//0: ��װ���������
//1��HDC2080
//2��HDC1080
//3��HTU21D
#if (TH_SENSOR_SELECT == 1)
#include "HDC2080.h"
#elif (TH_SENSOR_SELECT == 2)
#include "HDC1080.h"
#elif (TH_SENSOR_SELECT == 3)
#include "HTU21D.h"
#endif
/************ѡ����ʹ�õ���ѹ������***********/
#define	P_SENSOR_SELECT	1
//0: ��װ���������
//1��BMP390
#if (P_SENSOR_SELECT == 1)
#include "BMP390.h"
#endif
/************ѡ����ʹ�õĵشŴ�����***********/
#define	MAG_SENSOR_SELECT	1
//0: ��װ���������
//1��QMC5883L
#if (MAG_SENSOR_SELECT == 1)
#include "QMC5883L.h"
#include "Ellipsoid fitting.h"
#endif

enum sensor_sta{
	LSM6DSM_ISPRESENTED		= 0x01,
	BMP390_ISPRESENTED 		= 0x02,
	QMC5883L_ISPRESENTED	= 0x04,
	HDC2080_ISPRESENTED 	= 0x08,
	HDC1080_ISPRESENTED 	= 0x10,
	HTU21D_ISPRESENTED 		= 0x20
};

#define	KEY_SCAN_T	20 							//����ɨ������20ms
#define	KEY_LONGPRESS_THRESHOLD	20	//����������ֵ = 20 x KEY_SCAN_T = 400ms
#define	KEY_REPEAT_RATE	18  				//���������ظ����������� = 1000ms / ((KEY_LONGPRESS_THRESHOLD - 18) x KEY_SCAN_T) = 25Hz

#ifdef	__IWATCH_C
#define	IWATCH_EXTERN	
#else
#define	IWATCH_EXTERN	extern
#endif

#define	IWATCH_ACTIVE					0x0001
#define	IWATCH_IDLE						0x0002
#define	IWATCH_BEFORESLEEP		0x0004
#define	IWATCH_SLEEP					0x0008
#define	IWATCH_POWERDOWN			0x0010
#define	IWATCH_SCREENON				0x0020
#define	IWATCH_TIMERON				0x0040
#define	IWATCH_ALARMWENTOFF		0x0080

#define	IWATCH_KEYPRESSED			0x0001
#define	IWATCH_CABLEPLUGININT	0x0002
#define	IWATCH_CABLEUNPLUGINT	0x0004
#define	IWATCH_KEYINT					0x0008
#define	IWATCH_WRISTUPINT			0x0010
#define	IWATCH_RTCINT					0x0020
#define	IWATCH_ALARMINT				0x0040

typedef struct{
	unsigned char month;
	unsigned char day;
	unsigned int step;
}tsSTEP;
//������Ϣ�Ľṹ��
typedef struct{
	unsigned char t_inactive_max;				//��󲻻�Ծ��ʱ��
	unsigned char screen_brightness;		//��Ļ����
	unsigned char screen_inverse;				//��Ļ�Ƿ�ɫ
	unsigned char key_sound;						//�Ƿ��а�����
	unsigned char lsm6dsm_cfg;					//IMU����
	t_sAlarm alarm;											//���Ӳ���
	float cal_anglex;										//x�����ƫ��
	float cal_angley;										//y�����ƫ��
	#if (MAG_SENSOR_SELECT == 1)
	tsCaliData mag_cal_Data;						//������У׼����
	#endif
	tsSTEP step_data[7];								//�Ʋ�������ʷ���ݣ���Ź�ȥ����Ĳ���
	t_sRX8130CE last_charge_time;				//���һ�ν�������ʱ��
	unsigned int check_sum;							//����������Ϣ����ͽ�������ڼ򵥵���֤�����Ƿ�����
}iWatch_config;
#define	CONFIG_SIZE	sizeof(iWatch_config)

/******************ȫ�ֱ����������ʼ��********************/
#ifdef	__IWATCH_C
//��EEPROM��û�����ݻ������ݲ����������������Ĭ������
static iWatch_config code default_config = {
	//Ĭ����ʾ����
	6,													//�Զ�Ϣ��ʱ��
	1,													//��Ļ���ȣ�1~5��
	0,													//��Ļ�Ƿ�ɫ
	0,													//�رհ�����
	3,													//����̧���ѺͼƲ���
	{8, 30, 1, ALARM_DISABLE}, 	//Ĭ�����Ӳ���
	//Ĭ��У������
	0,													//x�����ƫ��
	0,													//y�����ƫ��
	#if (MAG_SENSOR_SELECT == 1)
	{0, 0, 0, 1, 1, 1},					//������У׼����
	#endif
	{{5,16,6554}, {5,15,3287}, {5,14,4516}, {5,13,3327}, {5,12,1216}, {5,11,5650}, {5,10,6357}},	//�Ʋ�������ʷ���ݣ���Ź�ȥ����Ĳ���
	{0, 0, 0, 0, 0, 0, 0},			//���һ�ν�������ʱ��
	0														//������������Ҫ�޸�
};
unsigned int xdata iWatch_status = IWATCH_ACTIVE | IWATCH_SCREENON;
unsigned int xdata iWatch_action = 0x0000;
unsigned char xdata battery[26] = {
	24, 8,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned char code BATTERY_LIFE_ICON[24] = {//24x8
	0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0x81,
	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,
	0x81,0x81,0x81,0x81,0x81,0xFF,0x3C,0x3C
};
unsigned char code BATTERY_CHARGING[24 + 2] = {//24x8
	24, 8,
	0xFF,0x81,0xBD,0xBD,0xBD,0xBD,0xBD,0xBD,
	0x81,0x99,0x1C,0xDE,0x7B,0x38,0x99,0x81,
	0xBD,0xBD,0xBD,0xBD,0x81,0xFF,0x3C,0x3C
	/* (24 X 8 )*/
};
#endif
/********************ȫ�ֱ�������������*************************/
IWATCH_EXTERN iWatch_config xdata config;					//������Ϣ
IWATCH_EXTERN unsigned int xdata iWatch_status;
IWATCH_EXTERN unsigned int xdata iWatch_action;
IWATCH_EXTERN unsigned char xdata sensor_status;	//������״̬
IWATCH_EXTERN t_sRX8130CE xdata time;							//ʱ����Ϣ
IWATCH_EXTERN float xdata battery_life;						//��ص���
IWATCH_EXTERN t_sLSM6DSM xdata LSM6DSM;						//IMU����
#if (MAG_SENSOR_SELECT == 1)
IWATCH_EXTERN int xdata mag[3];										//�ų�����
#endif
#if (TH_SENSOR_SELECT == 1)
IWATCH_EXTERN t_sHDC2080 xdata HDC2080;						//�¶�ʪ������
#elif (TH_SENSOR_SELECT == 2)
IWATCH_EXTERN t_sHDC1080 xdata HDC1080;
#elif (TH_SENSOR_SELECT == 3)
IWATCH_EXTERN t_sHTU21D xdata HTU21D;
#endif
#if (P_SENSOR_SELECT == 1)
IWATCH_EXTERN t_sBMP390	xdata BMP390;							//��ѹ����
#endif

IWATCH_EXTERN unsigned char xdata battery[26];
IWATCH_EXTERN unsigned char code BATTERY_LIFE_ICON[24];
IWATCH_EXTERN unsigned char code BATTERY_CHARGING[24 + 2];

#define	iWatchCheckStatus(status) (iWatch_status & status)
#define	iWatchSetStatus(status)		(iWatch_status |= status)
#define	iWatchClearStatus(status) (iWatch_status &= ~status)

void iWatchStatusUpdate(void);
void iWatchStatusHandle(void);
void iWatchKeepActive(void);
void iWatchAutoWakeHandle(void);
void iWatchSleep(void);
void iWatchPowerDown(void);
float iWatchGetBatteryLife(void);
unsigned char *DrawBatteryIcon(float bat);
void iWatchSaveConfig(iWatch_config *config);
unsigned char iWatchReadConfig(iWatch_config *config);
void iWatchInit(void);

#endif