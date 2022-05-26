#ifndef __QMC5883L_H
#define __QMC5883L_H

#include "Ellipsoid fitting.h"

#define	QMC5883L_IIC_ADDR   0x1A	  //定义器件在IIC总线中的从地址

unsigned char QMC5883L_Read_Byte(unsigned char addr);
unsigned char QMC5883LCheckConnection(void);
void QMC5883LRead(int *buf);
void QMC5883LSetMode(unsigned char mode);
unsigned char QMC5883LInit(void);
void QMC5883LSetCaliValue(tsCaliData *c);
void QMC5883LClearCaliValue(void);
float QMC5883LCalYaw(int *mag_data, float ax, float ay);

#endif