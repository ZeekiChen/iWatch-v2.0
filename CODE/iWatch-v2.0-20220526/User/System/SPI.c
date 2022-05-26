#include "STC32G.h"
#include "SPI.h"

void SPI_Init(void)
{	
	/*
	P_SW2 |= 0x80;
	HSCLKDIV = 0;
	HSSPI_CFG2 |= 0x20;
	P_SW2 &= ~0x80;
	*/
	SPCTL = 0xd3;
	P_SW1 = (P_SW1 & ~(0x03<<2)) | (0x01<<2);     //IO¿ÚÇÐ»». 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2
	SPSTAT = 0xc0;
}
void SPI_Deinit(void)
{
	SPCTL = 0x00;
}
void SPI_WriteByte(unsigned char out)
{
	SPDAT = out;
	while((SPSTAT & 0x80) == 0);
	SPSTAT = 0xc0;
}
unsigned char SPI_ReadByte(void)
{
	SPDAT = 0xff;
	while((SPSTAT & 0X80) == 0);
	SPSTAT = 0xc0;
	return SPDAT;
}
void SPI_WriteMultiBytes(unsigned char *tx_ptr, unsigned int n)
{
	unsigned int idata i;
	for(i = 0; i < n; i++)
	{
		SPDAT = tx_ptr[i];
		while((SPSTAT & 0X80) == 0);
		SPSTAT = 0xc0;
	}
}
void SPI_ReadMultiBytes(unsigned char *rx_ptr, unsigned int n)
{
	unsigned int idata i;
	for(i = 0; i < n; i++)
	{
		SPDAT = 0xff;
		while((SPSTAT & 0X80) == 0);
		SPSTAT = 0xc0;
		rx_ptr[i] = SPDAT;
	}
}