#ifndef _HTU21D_H
#define _HTU21D_H

#define	HTU21D_IIC_ADDR		0x80

#define	TRG_TEMP_MEA_HOLD		0xE3
#define	TRG_HUMI_MEA_HOLD		0xE5
#define	TRG_TEMP_MEA_NOHOLD	0xF3
#define	TRG_HUMI_MEA_NOHOLD	0xF5
#define	SOFT_RESET					0xFE

typedef struct
{
	float temperature;
	float humidity;
}t_sHTU21D;

void HTU21DStart(void);
unsigned char HTU21DGetTemperature(t_sHTU21D *p);
unsigned char HTU21DGetHumidity(t_sHTU21D *p);
void HTU21DReset(void);

#endif