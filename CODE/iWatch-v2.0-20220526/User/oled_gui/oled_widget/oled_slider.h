#ifndef _OLED_SLIDER_H
#define _OLED_SLIDER_H

#ifdef _OLED_SLIDER_C

#include "oled_widget.h"
#include "oled_font.h"

typedef struct{
	int16_t min;
	int16_t max;
	int16_t value;
}t_sSlider;

#endif

t_psWidget og_slider_create(int16_t min, int16_t max, int16_t value, uint8_t length, int16_t x, int16_t y);
void og_slider_setValue(t_psWidget pWidget, int16_t value);
int16_t og_slider_getValue(t_psWidget pWidget);
void og_slider_draw(t_psWidget pWidget);

#endif
