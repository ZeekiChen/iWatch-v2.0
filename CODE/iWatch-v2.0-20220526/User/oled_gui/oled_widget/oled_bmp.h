#ifndef _OLED_BMP_H
#define _OLED_BMP_H

#include "oled_widget.h"

typedef struct{
	uint8_t w;
	uint8_t h;
	uint8_t bmp;
}t_sBMP;
typedef t_sBMP * t_psBMP;
t_psWidget og_bmp_create(t_psBMP bmp, int16_t x, int16_t y);
void og_bmp_setBmp(t_psWidget w_ptr, t_psBMP bmp);
void og_bmp_setRotation(t_psWidget pWidget, uint8_t rotation);
void og_bmp_draw(t_psWidget pWidget);

#endif
