#ifndef _OLED_DRAW_H
#define _OLED_DRAW_H

#include "oled_conf.h"

#ifdef __OLED_DRAW_C

#define	OLED_BUF_SIZE	1024								// 1024 = 128 x 64 / 8
uint8_t display_buf[OLED_BUF_SIZE];				// фад╩от╢Ф

#include "oled.h"
#define	ScreenRefresh()		OLED_Refresh(display_buf)

#endif

void og_clearDisplayBuf(void);
void og_screenRefresh(void);
void BMPtoBuf(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t inverse, const uint8_t *buf1);

#endif
