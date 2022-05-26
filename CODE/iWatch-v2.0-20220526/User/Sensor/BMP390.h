#ifndef _BME280_H
#define _BME280_H

//IICµÿ÷∑‘ˆ¡ø£¨(SD0 = 0) £∫0xec£¨ (SDO = 1) £∫0xee
#define BMP390_IIC_ADDR	0xee

#define CMD			0x7E
#define CONFIG	0x1F
#define	ODR			0x1D
#define	OSR			0x1C
#define	PWR_CTRL	0x1B	
#define	IF_CONF		0x1A
#define	INT_CTRL	0x19
#define	FIFO_CONFIG_2		0x18
#define	FIFO_CONFIG_1		0x17
#define	FIFO_WTM_1			0x16
#define	FIFO_WTM_0			0x15
#define	FIFO_DATA				0x14
#define	FIFO_LENGTH_1		0x13
#define	FIFO_LENGTH_0		0x12
#define	INT_STATUS			0x11
#define	SENSORTIME_2		0x0E
#define	SENSORTIME_1		0x0D
#define	SENSORTIME_0		0x0C
#define	DATA_5		0x09
#define	DATA_4		0x08
#define	DATA_3		0x07
#define	DATA_2		0x06
#define	DATA_1		0x05
#define	DATA_0		0x04
//#define	STATUS		0x03
#define	ERR_REG		0x02
#define	REC_ID		0x01
#define	CHIP_ID		0x00

#define	SLEEP_MODE	0
#define	FORCED_MODE	1
#define	NORMAL_MODE	3

typedef struct
{
	float par_t1;
	float par_t2;
	double par_t3;
	float par_p1;
	float par_p2;
	float par_p3;
	float par_p4;
	float par_p5;
	float par_p6;
	float par_p7;
	float par_p8;
	double par_p9;
	double par_p10;
	double par_p11;
	float t_lin;
}BMP390_calib_data;

typedef struct
{
	float temperature;
	float pressure;
}t_sBMP390;

unsigned char BMP390CheckConnection(void);
void BMP390ReadSensorRawData(void);
float compensate_temperature(void);
float compensate_pressure(void);
void BMP390GetSensorData(t_sBMP390 *p);
void BMP390SoftReset(void);
unsigned char BMP390Init();
void BMP390SetMode(unsigned char mode);

#endif