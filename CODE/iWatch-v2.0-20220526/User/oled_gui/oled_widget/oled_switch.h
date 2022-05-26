#ifndef _OLED_SWITCH_H
#define _OLED_SWITCH_H

#ifdef _OLED_SWITCH_C

#include "oled_widget.h"

static uint8_t CODE bmp_switch8[2][12] = 
{
	{
		0x1C,0x22,0x49,0x5D,0x5D,0x49,0x41,0x41,0x41,0x41,0x22,0x1C
	},
	{
		0x1C,0x22,0x49,0x5D,0x5D,0x55,0x49,0x5D,0x5D,0x49,0x22,0x1C
	}
};
static uint8_t CODE bmp_switch16[2][48] =
{
	{
		0xF8,0x04,0x02,0xE2,0xF1,0xF9,0xF9,0xF9,0xF9,0xF9,0xF1,0xE1,0x01,0x01,0x01,0x01,
		0x01,0x01,0x01,0x01,0x02,0x02,0x04,0xF8,0x0F,0x10,0x20,0x23,0x47,0x4F,0x4F,0x4F,
		0x4F,0x4F,0x47,0x43,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x20,0x20,0x10,0x0F
	},
	{	
		0xF8,0x04,0x02,0xE2,0x51,0xA9,0x59,0xA9,0x59,0xE9,0x19,0x09,0xE1,0xF1,0xF9,0xF9,
		0xF9,0xF9,0xF9,0xF1,0xE2,0x02,0x04,0xF8,0x0F,0x10,0x20,0x23,0x45,0x4A,0x4D,0x4A,
		0x4D,0x4B,0x4C,0x48,0x43,0x47,0x4F,0x4F,0x4F,0x4F,0x4F,0x47,0x23,0x20,0x10,0x0F
	}
};
#endif

t_psWidget og_switch_create(uint8_t ucSize, uint8_t ucStatus, int16_t x, int16_t y);
void og_switch_setStatus(t_psWidget pWidget, uint8_t ucStatus);
uint8_t og_switch_getStatus(t_psWidget pWidget);
void og_switch_draw(t_psWidget pWidget);

#endif
