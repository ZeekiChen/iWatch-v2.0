#include "BMP390.h"
#include "IIC.h"
#include "Delay.h"

/*************************��������*****************************/
//����������ÿ���ϵ���BMP390�ж�ȡһ�μ���
static BMP390_calib_data xdata calib_data;
/*******************δ�����Ĵ�����ԭʼ����*********************/
unsigned long xdata uncomp_pressure;
unsigned long xdata uncomp_temperature;
/**********************���ò���********************************/
static unsigned char xdata BMP390_mode = 0x00;
#define	INDOOR_NAVIGATION
#define	press_en	0x01
#define	temp_en 	0x01
#ifdef	HANDEHELD_DEVICE_LOW_POWER
	//high resolution
	#define	osr_p	0x03	//x8 oversampling
	#define	osr_t	0x00	//x1 oversampling
	#define	iir_filter	2	
	#define	odr_sel 0x04	//12.5Hz
#endif
#ifdef	HANDEHELD_DEVICE_DYNAMIC
	//standard resolution
	#define	osr_p	0x02	//x4 oversampling
	#define	osr_t	0x00	//x1 oversampling
	#define	iir_filter	4
	#define	odr_sel 0x02	//50Hz
#endif
#ifdef	DROP_DETECTION
	//low power
	#define	osr_p	0x01	//x2 oversampling
	#define	osr_t	0x00	//x1 oversampling
	#define	iir_filter	0
	#define	odr_sel 0x01	//100Hz
#endif
#ifdef	INDOOR_NAVIGATION
	//ultra high resolution
	#define	osr_p	0x04	//x16 oversampling
	#define	osr_t	0x01	//x2 oversampling
	#define	iir_filter	4
	#define	odr_sel 0x03	//25Hz
#endif
#ifdef	DRONE
	//standard resolution
	#define	osr_p	0x03	//x8 oversampling
	#define	osr_t	0x00	//x1 oversampling
	#define	iir_filter	2
	#define	odr_sel 0x02	//50Hz
#endif
/***************************************************************************/

/**
  * @brief  ʹ��IIC������BMP390�ļĴ�����дһ�ֽ�����
  * @param  addr: �Ĵ����ĵ�ַ
  * @param  dat: 	��д�������
  * @retval ��
  */
void BMP390WriteByte(unsigned char addr, unsigned char dat)
{
	Single_WriteIIC(BMP390_IIC_ADDR, addr, dat);
}
/**
  * @brief  ʹ��IIC���ߴ�BMP390�ļĴ����ж�һ�ֽ�����
  * @param  addr: �Ĵ����ĵ�ַ
  * @retval ������һ�ֽ�����
  */
unsigned char BMP390ReadByte(unsigned char addr)
{
	unsigned char temp;
	temp = Single_ReadIIC(BMP390_IIC_ADDR,addr);
	return temp;
}
/**
  * @brief  �����BME280�������Ƿ�����
  * @param  ��
  * @retval 1���ɹ�����0��ʧ�ܣ�
  */
unsigned char BMP390CheckConnection(void)
{
	if(BMP390ReadByte(CHIP_ID) == 0x60)
		return 1;
	else
		return 0;
}
/**
	* @brief 	��BME280�ж�ȡ��������
	*					ÿ�������ϵ�����һ�μ���
	* @param  ��
	* @retval ��
	*/
void BMP390ReadCompensationParameter(void)
{
	unsigned char buf1[21];
	char int8_temp;
	int int16_temp;
	unsigned int uint16_temp;
	
	I2C_Read_MultiBytes(BMP390_IIC_ADDR, 0x31, 21, buf1);
	
	uint16_temp = (buf1[1] << 8) | buf1[0];
	calib_data.par_t1 = (float)uint16_temp / 0.00390625; //2^-8
	
	uint16_temp = (buf1[3] << 8) | buf1[2];
	calib_data.par_t2 = (float)uint16_temp / 1073741824;//2^30
	
	int8_temp = (char)buf1[4];
	calib_data.par_t3 = (double)int8_temp / 16777216 / 16777216; //2^24 / 2^24 = 2^48
	
	int16_temp = (int)((buf1[6] << 8) | buf1[5]);
	calib_data.par_p1 = ((float)int16_temp - 16384) / 1048576; //2^14, 2^20
	
	int16_temp = (int)((buf1[8] << 8) | buf1[7]); 
	calib_data.par_p2 = ((float)int16_temp - 16384) / 536870912; //2^14, 2^29
	
	int8_temp = (char)buf1[9];
	calib_data.par_p3 = (float)int8_temp / 65536 / 65536; //2^16 / 2^16
	
	int8_temp = (char)buf1[10];
	calib_data.par_p4 = (float)int8_temp / 65536 / 2097152; //2^16 / 2^21 = 2^37
	
	uint16_temp = (buf1[12] << 8) | buf1[11];
	calib_data.par_p5 = (float)uint16_temp / 0.125; //2^-3
	
	uint16_temp = (buf1[14] << 8) | buf1[13];
	calib_data.par_p6 = (float)uint16_temp / 64; //2^6
	
	int8_temp = (char)buf1[15];
	calib_data.par_p7 = (float)int8_temp / 256; //2^8
	
	int8_temp = (char)buf1[16];
	calib_data.par_p8 = (float)int8_temp / 32768; //2^15
	
	int16_temp = (int)((buf1[18] << 8) | buf1[17]); 
	calib_data.par_p9 = (double)int16_temp / 16777216 / 16777216; //2^24 / 2^24 = 2^48
	
	int8_temp = (char)buf1[19];
	calib_data.par_p10 = (double)int8_temp / 16777216 / 16777216; //2^24 / 2^24 = 2^48
	
	int8_temp = (char)buf1[20];
	calib_data.par_p11 = (double)int8_temp / 16777216 / 16777216 / 131072; //2^24 / 2^24 / 2^17 = 2^65
}
/**
	* @brief 	��BME280�ж�ȡ��������ԭʼ����
	* @param  ��
	* @retval ��
	*/
void BMP390ReadSensorRawData(void)
{
	unsigned char buf[6];
	I2C_Read_MultiBytes(BMP390_IIC_ADDR, DATA_0, 6, buf);
	uncomp_pressure 	 = ((unsigned long)buf[2] << 16) | ((unsigned long)buf[1] << 8) | buf[0];
	uncomp_temperature = ((unsigned long)buf[5] << 16) | ((unsigned long)buf[4] << 8) | buf[3];
}
/**
	* @brief This internal API is used to compensate the raw temperature data and
	* return the compensated temperature data in float data type.
	*/
float compensate_temperature(void)
{
	float partial_data1;
  float partial_data2;

	partial_data1 = (float)(uncomp_temperature - calib_data.par_t1);
	partial_data2 = (float)(partial_data1 * calib_data.par_t2);
	calib_data.t_lin = partial_data2 + (partial_data1 * partial_data1) * calib_data.par_t3;
	return calib_data.t_lin;
}
/**
	* @brief This internal API is used to compensate the raw pressure data and
	* return the compensated pressure data in float data type.
	*/
float compensate_pressure(void)
{
	float comp_press;
	float partial_data1;
  float partial_data2;
  float partial_data3;
  float partial_data4;
  float partial_out1;
	float partial_out2;
	
	partial_data1 = calib_data.par_p6 * calib_data.t_lin;
	partial_data2	= calib_data.par_p7 * (calib_data.t_lin * calib_data.t_lin);
	partial_data3 = calib_data.par_p8 * (calib_data.t_lin * calib_data.t_lin * calib_data.t_lin);
	partial_out1 = calib_data.par_p5 + partial_data1 + partial_data2 + partial_data3;
	
	partial_data1 = calib_data.par_p2 * calib_data.t_lin;
	partial_data2 = calib_data.par_p3 * (calib_data.t_lin * calib_data.t_lin);
	partial_data3 = calib_data.par_p4 * (calib_data.t_lin * calib_data.t_lin * calib_data.t_lin);
	partial_out2 = (float)uncomp_pressure * (calib_data.par_p1 + partial_data1 + partial_data2 + partial_data3);
  
	partial_data1 = (float)uncomp_pressure * (float)uncomp_pressure;
	partial_data2 = calib_data.par_p9 + calib_data.par_p10 * calib_data.t_lin;
	partial_data3 = partial_data1 * partial_data2;
	partial_data4 = partial_data3 + ((float)uncomp_pressure * (float)uncomp_pressure * (float)uncomp_pressure) * calib_data.par_p11;
	comp_press = partial_out1 + partial_out2 + partial_data4;
	
	return comp_press;
}
/**
	* @brief 	��BMP390�ж�ȡ�������������������
	* @param  ��
	* @retval ��
	*/
void BMP390GetSensorData(t_sBMP390 *p)
{
	BMP390ReadSensorRawData();
	p->temperature = compensate_temperature();
	p->pressure = compensate_pressure();
}
/**
	* @brief 	BMP390��λ
	* @param  ��
	* @retval ��
	*/
void BMP390SoftReset(void)
{
	BMP390WriteByte(CMD, 0xB6);
	Delay1ms(5);
}
/**
	* @brief �趨BMP390�Ĺ���ģʽ
	* @param  mode:����ģʽѡ��
	*				SLEEP_MODE	˯��ģʽ���͹��ģ�
	*				FORCED_MODE	���β���ģʽ
	*				NORMAL_MODE	�Զ���������ģʽ
	* @retval ��
	*/
void BMP390SetMode(unsigned char mode)
{
	BMP390_mode = mode;
	BMP390WriteByte(PWR_CTRL,  (BMP390_mode << 4) | (temp_en << 1) | press_en);
}
/**
	* @brief 	BMP390��ʼ������
	* @param  ��
	* @retval 0����ʼ��ʧ��
	*				 	1����ʼ���ɹ�
	*/
unsigned char BMP390Init()
{
	unsigned char time_out = 200;
	//Ӳ����·���Ѿ���оƬ��������������������
	//����������������
	//CSB = 1;
	//SDO = 1;
	Delay1ms(5);
	while(BMP390CheckConnection() == 0)
	{
		Delay1ms(1);
		if(--time_out == 0)
			return 0;
	}
	BMP390SoftReset();
	BMP390ReadCompensationParameter();
	BMP390WriteByte(PWR_CTRL,  (BMP390_mode << 4) | (temp_en << 1) | press_en);
	BMP390WriteByte(OSR, (osr_t << 3) | osr_p);
	BMP390WriteByte(ODR, odr_sel);
	BMP390WriteByte(CONFIG, iir_filter << 1);
	return 1;
}