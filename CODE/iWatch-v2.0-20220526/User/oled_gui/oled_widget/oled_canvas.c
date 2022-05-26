#include "oled_canvas.h"
#include "oled_font.h"

t_psWidget og_canvas_create(uint8_t w, uint8_t h, uint16_t x, uint16_t y)
{
	t_psWidget pWidget = og_widget_create();
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("canvas widget create fail");
		return NULL;
	}
	#endif
	w = w > OLED_WIDTH ? OLED_WIDTH : w;
	h = h > OLED_HEIGHT ? OLED_HEIGHT : h;
	h = h & 0xF8;  //向下取8的整数倍,h = (h / 8) * 8;
	pWidget->pFeature = calloc((w * h) >> 3, 1);
	#ifdef OLED_DEBUG
	if(pWidget->pFeature == NULL)
	{
		free(pWidget);
		OG_PRINTERROR("canvas widget create fail");
		return NULL;
	}
	#endif
	pWidget->w = w;
	pWidget->h = h;
	pWidget->x = x;
	pWidget->y = y;
	pWidget->config |= CANVAS_WIDGET;
	og_widget_add(pWidget);
	return pWidget;
}
void og_canvas_clear(t_psWidget pWidget)
{
	uint16_t i, temp;
	uint8_t *ptr = (uint8_t *)pWidget->pFeature;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");
		return;
	}
	if((pWidget->config & 0x0f) != CANVAS_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	temp = (pWidget->w * pWidget->h) >> 3;
	for(i = 0; i < temp; i++)
	{
		ptr[i] = 0x00;
	}
}
#define	DRAWDOT_FAST(x, y)	((uint8_t *)pWidget->pFeature)[(y >> 3) * pWidget->w + x] |= (0x01 << (y % 8))
void og_canvas_drawDot(t_psWidget pWidget, uint8_t x, uint8_t y)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != CANVAS_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	if(x > (pWidget->w - 1) || y > (pWidget->h - 1))
		return;
	DRAWDOT_FAST(x, y);
}
void og_canvas_drawLine(t_psWidget pWidget, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	float k;
	float b;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != CANVAS_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	if(x1 != x2)
	{
		k = ((float)y2 - y1) / ((float)x2 - x1);
		b = y1 - k * x1;
		if(k < 1 && k > -1)
		{
			if(x1 <= x2)
			{
				while(x1 <= x2)
				{
					DRAWDOT_FAST(x1, (uint8_t)(k * x1 + b));
					x1++;
				}
			}
			else
			{
				while(x2 <= x1)
				{
					DRAWDOT_FAST(x2, (uint8_t)(k * x2 + b));
					x2++;
				}
			}
		}
		else
		{
			if(y1 <= y2)
			{
				while(y1 <= y2)
				{
					DRAWDOT_FAST((uint8_t)((y1 - b)/k), y1);
					y1++;
				}
			}
			else
			{
				while(y2 <= y1)
				{
					DRAWDOT_FAST((uint8_t)((y2 - b)/k), y2);
					y2++; 
				}
			}
		}
	}
	else
	{
		if(y1 <= y2)
		{
			while(y1 <= y2)
			{
				DRAWDOT_FAST(x1, y1);
				y1++;
			}
		}
		else
		{
			while(y2 <= y1)
			{
				DRAWDOT_FAST(x2, y2);
				y2++;
			}
		}
	}
}
void og_canvas_drawRectangle(t_psWidget pWidget, uint8_t w, uint8_t h, uint8_t x, uint8_t y)
{
	uint8_t x2, y2;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != CANVAS_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	if(x > (pWidget->w - 1) || y > (pWidget->h - 1))
		return;
	x2 = (x + w) > (pWidget->w - 1) ? (pWidget->w - 1) : (x + w);
	y2 = (y + h) > (pWidget->h - 1) ? (pWidget->h - 1) : (y + h);
	og_canvas_drawLine(pWidget, x, y, x, y2);
	og_canvas_drawLine(pWidget, x, y, x2, y);
	og_canvas_drawLine(pWidget, x2, y, x2, y2);
	og_canvas_drawLine(pWidget, x, y2, x2, y2);
}
void og_canvas_drawText(t_psWidget pWidget, const char* str, uint8_t x, uint8_t y)
{
	uint8_t *offset;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != CANVAS_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	y = y >> 3;
	offset = (uint8_t *)pWidget->pFeature + y * pWidget->w + x;
	while(*str != '\0')
	{
		#if OLED_USE_BUILDIN_FONT == 1
		font_getCharASCII_6X8(*str, offset);
		#else
		font_getCharASCII_5X7(*str, offset);
		#endif
		offset += 6;
		str++;
	}
}
void og_canvas_draw(t_psWidget pWidget)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != CANVAS_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}	
	#endif
	BMPtoBuf(pWidget->x, pWidget->y, pWidget->w, pWidget->h, pWidget->config & WIDGET_FOCUS, (uint8_t *)pWidget->pFeature);
}
