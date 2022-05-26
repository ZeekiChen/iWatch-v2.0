#define	_GT20L16S1Y_C
#include "GT20L16S1Y.h"
#include "SPI.h"

void GT20L16S1YGetCharASCII_5X7(unsigned char s, unsigned char *ptr)
{
	unsigned long addr = font_addr[ASCII_5X7] + (s - 0x20) * 8;
	GT20L16S1YClrCS();
	SPI_WriteByte(0x03);
	SPI_WriteMultiBytes((unsigned char *)&addr + 1, 3);
	SPI_ReadMultiBytes(ptr, 5);
	//ptr[i] = 0;
	GT20L16S1YSetCS();
}
void GT20L16S1YGetCharASCII_7X8(unsigned char s, unsigned char *ptr)
{
	unsigned long addr = font_addr[ASCII_7X8] + ((unsigned long)(s - 0x20) << 3);
	GT20L16S1YClrCS();
	SPI_WriteByte(0x03);
	SPI_WriteMultiBytes((unsigned char *)&addr + 1, 3);
	SPI_ReadMultiBytes(ptr, 7);
	//ptr[i] = 0;
	GT20L16S1YSetCS();
}
void GT20L16S1YGetCharASCII_8X16(unsigned char s, unsigned char *ptr, unsigned int offset)
{
	unsigned long addr = font_addr[ASCII_8X16] + ((unsigned long)(s - 0x20) << 4);
	GT20L16S1YClrCS();
	SPI_WriteByte(0x03);
	SPI_WriteMultiBytes((unsigned char *)&addr + 1, 3);
	SPI_ReadMultiBytes(ptr, 8);
	ptr += offset;
	SPI_ReadMultiBytes(ptr, 8);
	GT20L16S1YSetCS();
}
void GT20L16S1YGetCharASCII_8X16_BOLD(unsigned char s, unsigned char *ptr, unsigned int offset)
{
	unsigned long addr = font_addr[ASCII_8X16_BOLD] + ((unsigned long)(s - 0x20) << 4);
	GT20L16S1YClrCS();
	SPI_WriteByte(0x03);
	SPI_WriteMultiBytes((unsigned char *)&addr + 1, 3);
	SPI_ReadMultiBytes(ptr, 8);
	ptr += offset;
	SPI_ReadMultiBytes(ptr, 8);
	GT20L16S1YSetCS();
}
void GT20L16S1YGetCharGB2312_15X16(unsigned int s, unsigned char *ptr, unsigned int offset)
{
	unsigned long addr;
	if(s >= 0xB0A1)
		addr = font_addr[GB2312_15X16] + ((unsigned long)((s >> 8) - 0xB0) * 94 + (s & 0x00ff) - 0xA1 + 846) << 5;
	else if((s & 0xff00) <= 0xA300)
		addr = font_addr[GB2312_15X16] + ((unsigned long)((s >> 8) - 0xA1) * 94 + (s & 0x00ff) - 0xA1) << 5;
	GT20L16S1YClrCS();
	SPI_WriteByte(0x03);
	SPI_WriteMultiBytes((unsigned char *)&addr + 1, 3);
	SPI_ReadMultiBytes(ptr, 16);
	ptr += offset;
	SPI_ReadMultiBytes(ptr, 16);
	GT20L16S1YSetCS();
}

