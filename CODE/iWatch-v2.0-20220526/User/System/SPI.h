#ifndef _SPI_H
#define _SPI_H

#define	SCLK_Set()		P25 = 1
#define	SCLK_Clr()		P25 = 0
#define	MOSI_Set()		P23 = 1
#define	MOSI_Clr()		P23 = 0
#define	MISO_Read()		P24

void SPI_Init(void);
void SPI_Deinit(void);
void SPI_WriteByte(unsigned char out);
unsigned char SPI_ReadByte(void);
void SPI_WriteMultiBytes(unsigned char *tx_ptr, unsigned int n);
void SPI_ReadMultiBytes(unsigned char *rx_ptr, unsigned int n);

#endif