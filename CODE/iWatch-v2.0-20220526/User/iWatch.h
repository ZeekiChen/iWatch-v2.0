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
/***********选择所使用的温湿度传感器**********/
#define	TH_SENSOR_SELECT	1
//0: 不装这个传感器
//1：HDC2080
//2：HDC1080
//3：HTU21D
#if (TH_SENSOR_SELECT == 1)
#include "HDC2080.h"
#elif (TH_SENSOR_SELECT == 2)
#include "HDC1080.h"
#elif (TH_SENSOR_SELECT == 3)
#include "HTU21D.h"
#endif
/************选择所使用的气压传感器***********/
#define	P_SENSOR_SELECT	1
//0: 不装这个传感器
//1：BMP390
#if (P_SENSOR_SELECT == 1)
#include "BMP390.h"
#endif
/************选择所使用的地磁传感器***********/
#define	MAG_SENSOR_SELECT	1
//0: 不装这个传感器
//1：QMC5883L
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

#define	KEY_SCAN_T	20 							//按键扫描周期20ms
#define	KEY_LONGPRESS_THRESHOLD	20	//按键长按阈值 = 20 x KEY_SCAN_T = 400ms
#define	KEY_REPEAT_RATE	18  				//按键长按重复触发的速率 = 1000ms / ((KEY_LONGPRESS_THRESHOLD - 18) x KEY_SCAN_T) = 25Hz

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
//设置信息的结构体
typedef struct{
	unsigned char t_inactive_max;				//最大不活跃的时间
	unsigned char screen_brightness;		//屏幕亮度
	unsigned char screen_inverse;				//屏幕是否反色
	unsigned char key_sound;						//是否有按键音
	unsigned char lsm6dsm_cfg;					//IMU参数
	t_sAlarm alarm;											//闹钟参数
	float cal_anglex;										//x轴倾角偏移
	float cal_angley;										//y轴倾角偏移
	#if (MAG_SENSOR_SELECT == 1)
	tsCaliData mag_cal_Data;						//磁力计校准数据
	#endif
	tsSTEP step_data[7];								//计步器的历史数据，存放过去七天的步数
	t_sRX8130CE last_charge_time;				//最近一次结束充电的时间
	unsigned int check_sum;							//所有设置信息的求和结果，用于简单的验证数据是否完整
}iWatch_config;
#define	CONFIG_SIZE	sizeof(iWatch_config)

/******************全局变量在这里初始化********************/
#ifdef	__IWATCH_C
//若EEPROM中没有数据或者数据不完整，则加载以下默认设置
static iWatch_config code default_config = {
	//默认显示参数
	6,													//自动息屏时间
	1,													//屏幕亮度（1~5）
	0,													//屏幕是否反色
	0,													//关闭按键音
	3,													//开启抬腕唤醒和计步器
	{8, 30, 1, ALARM_DISABLE}, 	//默认闹钟参数
	//默认校正参数
	0,													//x轴倾角偏移
	0,													//y轴倾角偏移
	#if (MAG_SENSOR_SELECT == 1)
	{0, 0, 0, 1, 1, 1},					//磁力计校准参数
	#endif
	{{5,16,6554}, {5,15,3287}, {5,14,4516}, {5,13,3327}, {5,12,1216}, {5,11,5650}, {5,10,6357}},	//计步器的历史数据，存放过去七天的步数
	{0, 0, 0, 0, 0, 0, 0},			//最近一次结束充电的时间
	0														//检验用求和项，不要修改
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
/********************全局变量在这里声明*************************/
IWATCH_EXTERN iWatch_config xdata config;					//设置信息
IWATCH_EXTERN unsigned int xdata iWatch_status;
IWATCH_EXTERN unsigned int xdata iWatch_action;
IWATCH_EXTERN unsigned char xdata sensor_status;	//传感器状态
IWATCH_EXTERN t_sRX8130CE xdata time;							//时间信息
IWATCH_EXTERN float xdata battery_life;						//电池电量
IWATCH_EXTERN t_sLSM6DSM xdata LSM6DSM;						//IMU数据
#if (MAG_SENSOR_SELECT == 1)
IWATCH_EXTERN int xdata mag[3];										//磁场数据
#endif
#if (TH_SENSOR_SELECT == 1)
IWATCH_EXTERN t_sHDC2080 xdata HDC2080;						//温度湿度数据
#elif (TH_SENSOR_SELECT == 2)
IWATCH_EXTERN t_sHDC1080 xdata HDC1080;
#elif (TH_SENSOR_SELECT == 3)
IWATCH_EXTERN t_sHTU21D xdata HTU21D;
#endif
#if (P_SENSOR_SELECT == 1)
IWATCH_EXTERN t_sBMP390	xdata BMP390;							//气压数据
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