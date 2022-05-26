#include "QMC5883L.h"
#include "IIC.h"
#include "Delay.h"
#include "math.h"

/******************************���ò���*************************************/
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
/*****************************У׼����************************************/											
int mag_x_offset = 0, mag_y_offset = 0, mag_z_offset = 0;		//��λУ׼����
float mag_y_scale = 1, mag_z_scale = 1;											//����У׼����
/*************************************************************************/

/**
  * @brief  ʹ��IIC������QMC5883L�ļĴ�����дһ�ֽ�����
  * @param  addr: �Ĵ����ĵ�ַ
  * @param  dat: 	��д�������
  * @retval None
  */
void QMC5883L_Write_Byte(unsigned char addr, unsigned char dat)
{
	Single_WriteIIC(QMC5883L_IIC_ADDR, addr, dat);
}
/**
  * @brief  ʹ��IIC���ߴ�QMC5883L�ļĴ����ж�һ�ֽ�����
  * @param  addr: �Ĵ����ĵ�ַ
  * @retval ������һ�ֽ�����
  */
unsigned char QMC5883L_Read_Byte(unsigned char addr)
{
	return Single_ReadIIC(QMC5883L_IIC_ADDR, addr);
}
/**
  * @brief  �����QMC5883L�������Ƿ�����
  * @param  ��
  * @retval 1���ɹ�����0��ʧ�ܣ�
  */
unsigned char QMC5883LCheckConnection(void)
{
	if(QMC5883L_Read_Byte(0x0D) != 0xff)
		return 0;
	else
		return 1;
}
/**
	* @brief 	��QMC5883L�ж�ȡ��������ԭʼ����
	* @param  buf��ԭʼ���������ָ�룬int*��
	* @retval ��
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
	* @brief 	����QMC5883Lԭʼ���ݵ�У׼����
	* @param  tsCaliData *c: У׼�����Ľṹ��ָ��
	* @retval ��
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
	* @brief 	���QMC5883L��У׼����
	* @param  ��
	* @retval ��
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
	* @brief 	����QMC5883L�Ĺ���ģʽ
	* @param  mode��0	����ģʽ
	*								1	��������ģʽ
	* @retval ��
	*/
void QMC5883LSetMode(unsigned char mode)
{
	MOD = QMC5883L_Read_Byte(0x09);
	MOD &= (~0x03);
	QMC5883L_Write_Byte(0x09, MOD | mode);
}
/**
	* @brief 	QMC5883L��ʼ������
	* @param  ��
	* @retval 0��ʧ��
	*					1���ɹ�
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
	* @brief 	����x���y�����ǲ�����ƫ��
	* @param  mag_data�������������
	*					ax��ay��x���y������
	* @retval Yaw��������Ĵ�ƫ�ǻ���
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