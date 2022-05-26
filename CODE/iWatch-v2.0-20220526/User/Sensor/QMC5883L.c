#include "QMC5883L.h"
#include "IIC.h"
#include "Delay.h"
#include "math.h"

/******************************设置参数*************************************/
unsigned char OSR = 0;		//Over Sample Ratio
													//00 = 512; 01 = 256; 10 = 128; 11 = 64
unsigned char ODR = 1;		//Typical Data Output Rate (Hz) 
													//00	10Hz
													//01	50Hz
													//10	100Hz
													//11	200Hz
unsigned char RNG = 0;		//Full Scale
													//00	2G
													//01	8G
unsigned char MOD = 0;		//Operating Mode 
													//00 = Standby
													//01 = Continuous(Default).
/*****************************校准参数************************************/											
int mag_x_offset = 0, mag_y_offset = 0, mag_z_offset = 0;		//零位校准参数
float mag_y_scale = 1, mag_z_scale = 1;											//幅度校准参数
/*************************************************************************/

/**
  * @brief  使用IIC总线往QMC5883L的寄存器中写一字节数据
  * @param  addr: 寄存器的地址
  * @param  dat: 	待写入的数据
  * @retval None
  */
void QMC5883L_Write_Byte(unsigned char addr, unsigned char dat)
{
	Single_WriteIIC(QMC5883L_IIC_ADDR, addr, dat);
}
/**
  * @brief  使用IIC总线从QMC5883L的寄存器中读一字节数据
  * @param  addr: 寄存器的地址
  * @retval 读出的一字节数据
  */
unsigned char QMC5883L_Read_Byte(unsigned char addr)
{
	return Single_ReadIIC(QMC5883L_IIC_ADDR, addr);
}
/**
  * @brief  检查与QMC5883L的连接是否正常
  * @param  无
  * @retval 1（成功），0（失败）
  */
unsigned char QMC5883LCheckConnection(void)
{
	if(QMC5883L_Read_Byte(0x0D) != 0xff)
		return 0;
	else
		return 1;
}
/**
	* @brief 	从QMC5883L中读取读传感器原始数据
	* @param  buf：原始数据数组的指针，int*型
	* @retval 无
	*/
void QMC5883LRead(int *buf)
{
	unsigned char temp[6];
	I2C_Read_MultiBytes(QMC5883L_IIC_ADDR, 0x00, 6, temp);
	buf[0] = (temp[1] << 8) | temp[0];
	buf[1] = (temp[3] << 8) | temp[2];
	buf[2] = (temp[5] << 8) | temp[4];
	buf[0] = -buf[0];
	buf[0] -= mag_x_offset;
	buf[1] -= mag_y_offset;
	buf[2] -= mag_z_offset;
	buf[1] *= mag_y_scale;
	buf[2] *= mag_z_scale;
}
/**
	* @brief 	设置QMC5883L原始数据的校准参数
	* @param  tsCaliData *c: 校准参数的结构体指针
	* @retval 无
	*/
void QMC5883LSetCaliValue(tsCaliData *c)
{
	mag_x_offset = c->X0;
	mag_y_offset = c->Y0;
	mag_z_offset = c->Z0;
	mag_y_scale = c->A / c->B;
	mag_z_scale = c->A / c->C;
}
/**
	* @brief 	清除QMC5883L的校准参数
	* @param  无
	* @retval 无
	*/
void QMC5883LClearCaliValue(void)
{
	mag_x_offset = 0;
	mag_y_offset = 0;;
	mag_z_offset = 0;
	mag_y_scale = 1;
	mag_z_scale = 1;
}
/**
	* @brief 	设置QMC5883L的工作模式
	* @param  mode：0	休眠模式
	*								1	连续测量模式
	* @retval 无
	*/
void QMC5883LSetMode(unsigned char mode)
{
	MOD = QMC5883L_Read_Byte(0x09);
	MOD &= (~0x03);
	QMC5883L_Write_Byte(0x09, MOD | mode);
}
/**
	* @brief 	QMC5883L初始化函数
	* @param  无
	* @retval 0：失败
	*					1：成功
	*/
unsigned char QMC5883LInit(void)
{
	unsigned char time_out = 200;
	while(QMC5883LCheckConnection() == 0)
	{
		Delay1ms(1);
		if(--time_out == 0)
			return 0;
	}
	QMC5883L_Write_Byte(0x09, (OSR << 6) | (RNG  << 4) | (ODR << 2) | MOD);
	QMC5883L_Write_Byte(0x0A, 0x40);
	QMC5883L_Write_Byte(0x0B, 0x01);
	return 1;
}
/**
	* @brief 	根据x轴和y轴的倾角补偿磁偏角
	* @param  mag_data：三轴磁力数据
	*					ax，ay：x轴和y轴的倾角
	* @retval Yaw：补偿后的磁偏角弧度
	*/
float QMC5883LCalYaw(int *mag_data, float ax, float ay)
{
	float Yaw, RadX, RadY, Xh, Yh;
	RadX = -ax / 57.2957795f;
	RadY = -ay / 57.2957795f;
	Xh = mag_data[0] * cos(RadX) + mag_data[1] * sin(RadY) * sin(RadX) - mag_data[2] * cos(RadY) * sin(RadX);
	Yh = mag_data[1] * cos(RadY) + mag_data[2] * sin(RadY);
	Yaw = (float)atan2(Yh, Xh) * 180.0 / 3.1415926 + 180.0;
	return Yaw;
}