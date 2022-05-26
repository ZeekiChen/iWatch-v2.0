#include "HDC1080.h"
#include "System.h"


unsigned char code TRES = 0;  //Temperature resolution
															//0: 14 bit
															//1: 11 bit
unsigned char code HRES = 0;	//Humidity resolution
															//00: 14 bit
															//01: 11 bit
															//10: 8 bit
															//11: NA
unsigned char code MEAS_CFG = 1;		//Measurement configuration
																		//00: Temperature or Humidity
																		//01: Humidity + Temperature
																		//10: NA
																		//11: NA
static unsigned char step = 0;

void HDC1080WriteByte16(unsigned char addr, unsigned int dat)
{
	P_SW2 |= 0x80;
  I2C_Start();                  //起始信号
  I2C_SendByte(HDC1080_IIC_ADDR);   //发送设备地址+写信号
	I2C_RecvACK();
	I2C_SendByte(addr);    //内部寄存器地址
	I2C_RecvACK();
	I2C_SendByte(dat >> 8);       //内部寄存器数据
	I2C_RecvACK();
	I2C_SendByte(dat & 0x00ff);   //内部寄存器数据
	I2C_RecvACK();
	I2C_Stop();                   //发送停止信号
	P_SW2 &= 0x7f;
}
unsigned int HDC1080ReadByte16(unsigned char addr)
{
	unsigned char temp[2];
	I2C_Read_MultiBytes(HDC1080_IIC_ADDR, addr, 2, (unsigned char *)temp);
	return (temp[0] << 8) | temp[1];
}
unsigned char HDC1080CheckConnection()
{
	if(HDC1080_devID == HDC1080ReadByte16(REG_DEVICE_ID))
		return 1;
	else
		return 0;
}
/*
void HDC2080SoftReset(void)
{
	HDC2080WriteByte(INT_CONF, 0x80);
}
*/
void HDC1080Start(void)
{
	step = 0;
}
unsigned char HDC1080GetTH(t_sHDC1080 *p)
{
	unsigned char temp[4];
	unsigned int temp1, temp2;
	if(step == 0)
	{
		P_SW2 |= 0x80;
		I2C_Start();                   //起始信号
		I2C_SendByte(HDC1080_IIC_ADDR);  //发送设备地址+写信号
		I2C_RecvACK();
		I2C_SendByte(REG_TEMPERATURE);  //发送设备地址+写信号
		I2C_RecvACK();
		step = 1;
		return 0;
	}
	//Delay1ms(20);
	else if(step == 1)
	{
		I2C_Start();                   //起始信号
		I2C_SendByte(HDC1080_IIC_ADDR | 0x01);  //发送设备地址+读信号
		I2C_RecvACK();
		temp[0] = I2C_RecvByte();       //读出寄存器数据
		I2C_SendACK();
		temp[1] = I2C_RecvByte();       //读出寄存器数据
		I2C_SendACK();
		temp[2] = I2C_RecvByte();       //读出寄存器数据
		I2C_SendACK();
		temp[3] = I2C_RecvByte();       //读出寄存器数据
		I2C_SendNAK();								 //发送应答信号
		I2C_Stop();                    //停止信号
		P_SW2 &= 0x7f;
		temp1 = (temp[0] << 8) | temp[1];
		temp2 = (temp[2] << 8) | temp[3];
		//For highest accuracy, correction for a slight PSSR sensitivity 
		//should be applied to 方程式 1 for supplies > 1.8 V
		p->temperature = temp1 * 165.0 / 65536.0 - 40.0;
		p->humidity = temp2 * 100.0 / 65536.0;
		step = 0;
		return 1;
	}
	return 0;
}
unsigned char HDC1080Init(void)
{
	unsigned int time_out = 100;
	Delay1ms(50);
	while(HDC1080CheckConnection() == 0)
	{
		Delay1ms(1);
		if(--time_out == 0)
			return 0;
	}
	HDC1080WriteByte16(REG_CONFIG, (TRES << 10) | (HRES << 8) | (MEAS_CFG << 12));
	return 1;
}
