#ifndef __HDC1080_H_
#define __HDC1080_H_

#define HDC1080_IIC_ADDR	0x80
#define	HDC1080_devID		0x1050
#define	HDC1080_mfID		0x5449

//Register Maps
#define	REG_TEMPERATURE			0x00
#define	REG_HUMIDITY				0x01
#define	REG_CONFIG					0x02
#define	REG_MANUFACTURER_ID	0xFE
#define	REG_DEVICE_ID				0xFF

typedef struct
{
	float temperature;
	float humidity;
}t_sHDC1080;

unsigned char HDC1080CheckConnection(void);
void HDC1080Start(void);
unsigned char HDC1080GetTH(t_sHDC1080 *p);
unsigned char HDC1080Init(void);

#endif