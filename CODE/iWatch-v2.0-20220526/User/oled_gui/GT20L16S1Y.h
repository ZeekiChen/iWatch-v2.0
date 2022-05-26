#ifndef _GT20L16S1Y_H
#define _GT20L16S1Y_H

#ifdef _GT20L16S1Y_C
#include "STC32G.h"
#define	GT20L16S1YSetCS()		P44 = 1
#define	GT20L16S1YClrCS()		P44 = 0
const unsigned long int font_addr[6] = {0x3BFC0, 0x0066C0, 0x03B7C0, 0x03CF80, 0x000000, 0x03B7D0};
#endif

enum font
{
	ASCII_5X7,
	ASCII_7X8,
	ASCII_8X16,
	ASCII_8X16_BOLD,
	GB2312_15X16,
	GB2312_8X16
};

void GT20L16S1YGetCharASCII_5X7(unsigned char s, unsigned char *ptr);
void GT20L16S1YGetCharASCII_7X8(unsigned char s, unsigned char *ptr);
void GT20L16S1YGetCharASCII_8X16(unsigned char s, unsigned char *ptr, unsigned int offset);
void GT20L16S1YGetCharASCII_8X16_BOLD(unsigned char s, unsigned char *ptr, unsigned int offset);
void GT20L16S1YGetCharGB2312_15X16(unsigned int s, unsigned char *ptr, unsigned int offset);

#endif