#include "RX8130CE.h"
#include "IIC.h"

void RX8130RegSetBit(unsigned char addr, unsigned char mask, unsigned char sc)
{
	unsigned char temp;
	temp = Single_ReadIIC(RX8130CE_IIC_ADDR, addr);
	if(sc)
		temp |= mask;
	else
		temp &= ~mask;
	Single_WriteIIC(RX8130CE_IIC_ADDR, addr, temp);
}
void RX8130CEReadTime(t_sRX8130CE *t)
{
	unsigned char temp[7];
	unsigned char i = 0;
	I2C_Read_MultiBytes(RX8130CE_IIC_ADDR, 0x10, 7, temp);
	t->second = (temp[0] >> 4) * 10 + (temp[0] & 0x0f);
	t->minute = (temp[1] >> 4) * 10 + (temp[1] & 0x0f);
	t->hour	= (temp[2] >> 4) * 10 + (temp[2] & 0x0f);
	if(temp[3] > 0x40)
		temp[3] = 0x40;
	while(temp[3] != 0x00)
	{
		temp[3] >>= 1;
		i++;
	}
	t->weekday = i;
	t->day = (temp[4] >> 4) * 10 + (temp[4] & 0x0f);
	t->month = (temp[5] >> 4) * 10 + (temp[5] & 0x0f);
	t->year = (temp[6] >> 4) * 10 + (temp[6] & 0x0f);
}
void RX8130CEWriteTime(t_sRX8130CE *t)
{
	unsigned char temp[7];
	temp[0] = ((t->second / 10) << 4) | (t->second % 10);
	temp[1] = ((t->minute / 10) << 4) | (t->minute % 10);
	temp[2] = ((t->hour / 10) << 4) | (t->hour % 10);
	temp[3] = (0x01 << (t->weekday - 1));
	temp[4] = ((t->day / 10) << 4) | (t->day % 10);
	temp[5] = ((t->month / 10) << 4) | (t->month % 10);
	temp[6] = ((t->year / 10) << 4) | (t->year % 10);
	I2C_Write_MultiBytes(RX8130CE_IIC_ADDR, 0x10, 7, temp);
}
void RX8130CESetAlarm(t_sAlarm *a)
{
	unsigned char alarm_hour, alarm_minute, alarm_week_day;
	alarm_hour = ((a->hour / 10) << 4) | (a->hour % 10);
	alarm_minute = ((a->minute / 10) << 4) | (a->minute % 10);
	if(a->mode == ALARM_EVERYDAY || a->mode == ALARM_ONCE)
	{
		alarm_week_day = 0x80;
	}
	else if(a->mode == ALARM_WORKDAY)
	{
		alarm_week_day = 0x1f;
		RX8130RegSetBit(0x1C, 0x08, 0);		//clear WADA
	}
	else if(a->mode == ALARM_SPECIFIC_DAY)
	{
		alarm_week_day = ((a->day / 10) << 4) | (a->day % 10);
		RX8130RegSetBit(0x1C, 0x08, 1);		//set WADA
	}
	Single_WriteIIC(RX8130CE_IIC_ADDR, 0x17, alarm_minute);
	Single_WriteIIC(RX8130CE_IIC_ADDR, 0x18, alarm_hour);
	Single_WriteIIC(RX8130CE_IIC_ADDR, 0x19, alarm_week_day);
	RX8130RegSetBit(0x1E, 0x08, 1);		//set AIE
	RX8130RegSetBit(0x1D, 0x08, 0);		//clear AF
}
void RX8130CEAlarmHandle(t_sAlarm *a)
{
	if(a->mode == ALARM_ONCE)
	{
		RX8130CEDisableAlarm();
		a->mode = ALARM_DISABLE;
	}
}
void RX8130CEClearAlarmFlag(void)
{
	RX8130RegSetBit(0x1D, 0x08, 0);
}
void RX8130CEDisableAlarm(void)
{
	Single_WriteIIC(RX8130CE_IIC_ADDR, 0x17, 0x80);
	Single_WriteIIC(RX8130CE_IIC_ADDR, 0x18, 0x80);
	Single_WriteIIC(RX8130CE_IIC_ADDR, 0x19, 0x80);
	RX8130RegSetBit(0x1E, 0x08, 0);		//set AIE
	RX8130RegSetBit(0x1D, 0x08, 0);		//clear AF
}
void RX8130CESetTimer(unsigned char timer_clk, unsigned int down_cnt_value)
{
	//clear TE bit first
	RX8130RegSetBit(0x1C, 0x10, 0);
	//Set timer counter 0,1
	Single_WriteIIC(RX8130CE_IIC_ADDR, 0x1A, down_cnt_value);
	Single_WriteIIC(RX8130CE_IIC_ADDR, 0x1B, down_cnt_value >> 8);
	//clear TF bit
	RX8130RegSetBit(0x1D, 0x10, 0);
	//set	TIE bit, enable timer interrupt
	RX8130RegSetBit(0x1E, 0xC7, 0);	//clear TEST,STOP,TSTP,TBKON,TBKN bits
	RX8130RegSetBit(0x1E, 0x10, 1);	//set TIE bit
	//set TE bit and timer source clock
	RX8130RegSetBit(0x1C, 0x07, 0);	//clear TSEL2,TSEL1,TSEL0 bits
	RX8130RegSetBit(0x1C, 0x10, 1);	//set TE bit
	RX8130RegSetBit(0x1C, timer_clk, 1);	//set timer source clock
}
void RX8130CEClearTimerFlag(void)
{
	RX8130RegSetBit(0x1D, 0x10, 0);
}

void RX8130CEDisableTimer(void)
{
	//clear TE bit
	RX8130RegSetBit(0x1C, 0x10, 0);
	//clear TIE bit, disable timer interrupt
	RX8130RegSetBit(0x1E, 0x10, 0);
}
void RX8130CESetClockOuput(unsigned char clock_fq)
{
	RX8130RegSetBit(0x1C, 0xC0, 0);
	RX8130RegSetBit(0x1C, (clock_fq << 6), 1);	
}
unsigned char RX8130CEReadIntSrc(void)
{
	unsigned char temp;
	temp = Single_ReadIIC(RX8130CE_IIC_ADDR, 0x1D);
	return (temp & 0x18);
}
void RX8130CEInit(void)
{
	RX8130CESetClockOuput(CLKOUT_DISABLE);
	RX8130CEClearTimerFlag();
	RX8130CEClearAlarmFlag();
	RX8130CESetTimer(TIMERCLK_1_60HZ, 1);
	//RX8130CEDisableTimer();
}