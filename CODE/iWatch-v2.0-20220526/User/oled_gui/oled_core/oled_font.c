#define _OLED_FONT_C
#include "oled_font.h"

#if (OLED_USE_BUILDIN_FONT == 1)
#if	(USE_FONT_ASIIC_6X8 == 1)
void font_getCharASCII_6X8(uint8_t s, uint8_t *ptr)
{
	uint8_t i;
	s -= ' ';
	for(i = 0; i < 6; i++)
	{
		ptr[i] = ASCII_6X8[s][i];
	}
}
#endif
#if	(USE_FONT_ASIIC_8X16 == 1)
void font_getCharASCII_8X16(uint8_t s, uint8_t *ptr, uint16_t offset)
{
	uint8_t i;
	s -= ' ';
	for(i = 0; i < 8; i++)
	{
		ptr[i] = ASCII_8X16[s][i];
	}
	ptr += (offset - 8);
	for(i = 8; i < 16; i++)
	{
		ptr[i] = ASCII_8X16[s][i];
	}
}
#endif
#if	(USE_FONT_ASIIC_16X32 == 1)
void font_getCharASCII_16X32(uint8_t s, uint8_t *ptr, uint16_t offset)
{
	uint8_t i, j, k;
	s -= ' ';
	for(j = 0; j < 4; j++)
	{
		k = j << 4;
		for(i = 0; i < 16; i++)
		{
			ptr[i] = ASCII_16X32[s][k + i];
		}
		ptr += offset;
	}
}
#endif
#endif
