#include "IIC.h"
#include "Delay.h"
#include "HDC2080.h"

unsigned char code TRES = 0;  //Temperature resolution
															//00: 14 bit
															//01: 11 bit
															//10: 9 bit
															//11: NA
unsigned char code HRES = 0;	//Humidity resolution
															//00: 14 bit
															//01: 11 bit
															//10: 9 bit
															//11: NA
unsigned char code MEAS_CFG = 0;		//Measurement configuration
																		//00: Humidity + Temperature
																		//01: Temperature only
																		//10: NA
																		//11: NA

void HDC2080WriteByte(unsigned char addr, unsigned char dat)
{
	Single_WriteIIC(HDC2080_IIC_ADDR, addr, dat);
}
unsigned char HDC2080ReadByte(unsigned char addr)
{
	return Single_ReadIIC(HDC2080_IIC_ADDR, addr);
}
unsigned char HDC2080CheckConnection()
{
	unsigned char temp1, temp2;
	temp1 = HDC2080ReadByte(DEVICE_ID_LOW);
	temp2 = HDC2080ReadByte(DEVICE_ID_HIGH);
	if((temp2 == (HDC2080_devID >> 8)) && (temp1 == (HDC2080_devID & 0x00ff)))
		return 1;
	else
		return 0;
}
/*
Auto Measurement Mode (AMM)
000 = Disabled. Initiate measurement via I2C (single convertion mode)
001 = 1/120Hz (1 samples every 2 minutes)
010 = 1/60Hz (1 samples every minute)
011 = 0.1Hz (1 samples every 10 seconds)
100 = 0.2 Hz (1 samples every 5 second)
101 = 1Hz (1 samples every second)
110 = 2Hz (2 samples every second)
111 = 5Hz (5 samples every second)
*/
void HDC2080SetMode(unsigned char mode)
{
	HDC2080WriteByte(INT_CONF, mode << 4);
}
/*
void HDC2080SoftReset(void)
{
	HDC2080WriteByte(INT_CONF, 0x80);
}
*/
void HDC2080StartConversion(void)
{
	unsigned char temp;
	temp = HDC2080ReadByte(MEAS_CONF);
	temp |= 0x01;
	HDC2080WriteByte(MEAS_CONF, temp);
}
void HDC2080GetTH(t_sHDC2080 *p)
{
	unsigned char temp[4];
	unsigned long temp1, temp2;
	I2C_Read_MultiBytes(HDC2080_IIC_ADDR, TEMPERATURE_LOW, 4, temp);
	temp1 = (temp[1] << 8) | temp[0];
	temp2 = (temp[3] << 8) | temp[2];
	//For highest accuracy, correction for a slight PSSR sensitivity 
	//should be applied to ·½³ÌÊ½ 1 for supplies > 1.8 V
	p->temperature = temp1 * 165.0 / 65536.0 - (40.5 + TEMPpsrr * (3.3 - 1.8));
	p->humidity = temp2 * 100.0 / 65536.0;
}
unsigned char HDC2080Init(void)
{
	unsigned int time_out = 100;
	Delay1ms(50);
	while(HDC2080CheckConnection() == 0)
	{
		Delay1ms(1);
		if(--time_out == 0)
			return 0;
	}
	HDC2080WriteByte(MEAS_CONF, (TRES << 6) | (HRES << 4) | (MEAS_CFG << 1));
	HDC2080SetMode(0);
	return 1;
}
