#include "HTU21D.h"
#include "System.h"

static unsigned char step = 0;

void HTU21DSendCommand(unsigned char command)
{
	P_SW2 |= 0x80;
	I2C_Start();
	I2C_SendByte(HTU21D_IIC_ADDR);
	I2C_RecvACK();
	I2C_SendByte(command);
	I2C_RecvACK();
	I2C_Stop();
	P_SW2 &= 0x7f;
}
unsigned int HTU21DReadData()
{
	unsigned int temp;
	P_SW2 |= 0x80;
	I2C_Start();
	I2C_SendByte(HTU21D_IIC_ADDR | 0x01);
	I2C_RecvACK();
	temp = I2C_RecvByte() << 8;
	I2C_SendACK();
	temp |= I2C_RecvByte();
	I2C_SendACK();
	I2C_RecvByte();
	I2C_SendNAK();
	I2C_Stop();
	P_SW2 &= 0x7f;
	temp &= 0xfffc;
	return temp;
}
void HTU21DStart(void)
{
	step = 0;
}
unsigned char HTU21DGetTemperature(t_sHTU21D *p)
{
	if(step == 0)
	{
		HTU21DSendCommand(TRG_TEMP_MEA_NOHOLD);
		step = 1;
		return 0;
	}
	else if(step == 1)
	{
		unsigned int temp = HTU21DReadData();
		p->temperature = 175.72 * (long)temp / 65536.0 - 46.85;
		step = 0;
		return 1;
	}
	return 0;
}
unsigned char HTU21DGetHumidity(t_sHTU21D *p)
{
	if(step == 0)
	{
		HTU21DSendCommand(TRG_HUMI_MEA_NOHOLD);
		step = 1;
		return 0;
	}
	else if(step == 1)
	{
		unsigned int temp = HTU21DReadData();
		p->humidity = 125.0 * (long)temp / 65536.0 - 6.0;
		step = 0;
		return 1;
	}
	return 0;
}
void HTU21DReset(void)
{
	P_SW2 |= 0x80;
	I2C_Start();
	I2C_SendByte(HTU21D_IIC_ADDR);
	I2C_RecvACK();
	I2C_SendByte(SOFT_RESET);
	I2C_RecvACK();
	I2C_Stop();
	P_SW2 &= 0x7f;
	Delay1ms(15);
}