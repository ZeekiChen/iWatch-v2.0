#ifndef __HDC2080_H_
#define __HDC2080_H_

#define HDC2080_IIC_ADDR	0x80
#define	HDC2080_devID		0x07D0
#define	HDC2080_mfID		0x5449
#define	TEMPpsrr	0.08

//Register Maps
#define	TEMPERATURE_LOW			0x00
#define	TEMPERATURE_HIGH		0x01
#define	HUMIDITY_LOW				0x02
#define	HUMIDITY_HIGH				0x03
#define	INTERRUPT		_DRDY		0x04
#define	TEMPERATURE_MAX			0x05
#define	HUMIDITY_MAX				0x06
#define	INTERRUPT_ENABLE		0x07
#define	TEMP_OFFSET_ADJUST	0x08
#define	HUM_OFFSET_ADJUST		0x09
#define	TEMP_THR_L	0x0A
#define	TEMP_THR_H	0x0B
#define	RH_THR_L		0x0C
#define	RH_THR_H		0x0D
#define	INT_CONF		0x0E
#define	MEAS_CONF		0x0F
#define	MANUFACTURER_ID_LOW		0xFC
#define	MANUFACTURER_ID_HIGH	0xFD
#define	DEVICE_ID_LOW					0xFE
#define	DEVICE_ID_HIGH				0xFF

typedef struct
{
	float temperature;
	float humidity;
}t_sHDC2080;

unsigned char HDC2080CheckConnection(void);
void HDC2080SetMode(unsigned char mode);
void HDC2080StartConversion(void);
void HDC2080GetTH(t_sHDC2080 *p);
unsigned char HDC2080Init(void);

#endif