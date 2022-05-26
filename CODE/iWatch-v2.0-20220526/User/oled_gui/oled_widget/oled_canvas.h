#ifndef _OLED_CANVAS_H
#define _OLED_CANVAS_H

#include "oled_widget.h"

t_psWidget og_canvas_create(uint8_t w, uint8_t h, uint16_t x, uint16_t y);
void og_canvas_clear(t_psWidget pWidget);
void og_canvas_drawDot(t_psWidget pWidget, uint8_t x, uint8_t y);
void og_canvas_drawLine(t_psWidget pWidget, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void og_canvas_drawRectangle(t_psWidget pWidget, uint8_t w, uint8_t h, uint8_t x, uint8_t y);
void og_canvas_drawText(t_psWidget pWidget, const char* str, uint8_t x, uint8_t y);
void og_canvas_draw(t_psWidget pWidget);

#endif
