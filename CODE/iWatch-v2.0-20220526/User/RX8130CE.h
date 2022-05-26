#ifndef _RX8130CE_H
#define _RX8130CE_H

#define	RX8130CE_IIC_ADDR	0x64

#define	RTC_TIMER_INT	0x10
#define	RTC_ALARM_INT	0x08

typedef struct{
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char weekday;
	unsigned char day;
	unsigned char month;
	unsigned char year;
}t_sRX8130CE;

typedef struct{
	unsigned char hour;
	unsigned char minute;
	unsigned char day;
	unsigned char mode;
}t_sAlarm;

enum alarm_mode{
	ALARM_DISABLE = 0,
	ALARM_ONCE,
	ALARM_EVERYDAY,
	ALARM_WORKDAY,
	ALARM_SPECIFIC_DAY
};

enum clock_out{
	CLKOUT_32768_HZ,
	CLKOUT_1024_HZ,
	CLKOUT_1_HZ,
	CLKOUT_DISABLE
};

enum timer_clk{
	TIMERCLK_4096HZ = 0,	//定时器时钟4.096kHz
	TIMERCLK_64HZ,				//定时器时钟64kHz
	TIMERCLK_1HZ,					//定时器时钟1kHz
	TIMERCLK_1_60HZ,			//定时器时钟1/60Hz，相当于一分钟一个时钟
	TIMERCLK_1_3600HZ			//定时器时钟1/3600kHz，相当于一小时一个时钟
};

//RTC func
void RX8130CEReadTime(t_sRX8130CE *t);
void RX8130CEWriteTime(t_sRX8130CE *t);
//Alarm func
void RX8130CESetAlarm(t_sAlarm *a);
void RX8130CEDisableAlarm(void);
void RX8130CEAlarmHandle(t_sAlarm *sAlarm);
void RX8130CEClearAlarmFlag(void);
//Timer func
void RX8130CESetTimer(unsigned char timer_clk, unsigned int down_cnt_value);
void RX8130CEClearTimerFlag(void);
void RX8130CEDisableTimer(void);
//Clock-out func
void RX8130CESetClockOuput(unsigned char clock_fq);
unsigned char RX8130CEReadIntSrc(void);
void RX8130CEInit(void);

#endif