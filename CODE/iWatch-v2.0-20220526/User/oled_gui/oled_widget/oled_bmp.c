#include "oled_bmp.h"

t_psWidget og_bmp_create(t_psBMP psBMP, int16_t x, int16_t y)
{
	t_psWidget pWidget = og_widget_create();
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("bmp widget create fail");
		return NULL;
	}
	#endif
	pWidget->pFeature = &(psBMP->bmp);
	pWidget->w = psBMP->w;
	pWidget->h = psBMP->h;
	pWidget->x = x;
	pWidget->y = y;
	pWidget->config |= BMP_WIDGET;
	og_widget_add(pWidget);
	return pWidget;
}
void og_bmp_setBmp(t_psWidget pWidget, t_psBMP psBMP)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != BMP_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}	
	#endif
	pWidget->w = psBMP->w;
	pWidget->h = psBMP->h;
	pWidget->pFeature = &(psBMP->bmp);
}
/*
void og_bmp_setRotation(t_psWidget pWidget, uint8_t rotation)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != BMP_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}	
	#endif
	pWidget->config &= ~0xC0;
	pWidget->config |= (rotation << 6);
}
uint8_t aabb(uint8_t x, uint8_t y, t_psWidget pWidget)
{
	uint8_t *ptr = (uint8_t *)pWidget->pFeature;
	uint8_t *offset = ptr + (pWidget->h/8 - 1 - x/8)*pWidget->w + y * 8;
	uint8_t bit_mask = (0x80 >> (x%8));
	uint8_t i, temp = 0;;
	for(i = 0; i < 8; i++)
	{
		if(offset[i] & bit_mask)
		{
			temp |= (0x01 << i);
		}
	}
	return temp;
}
*/
void og_bmp_draw(t_psWidget pWidget)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != BMP_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}	
	#endif
	BMPtoBuf(pWidget->x, pWidget->y, pWidget->w, pWidget->h, pWidget->config & WIDGET_FOCUS, (uint8_t *)pWidget->pFeature);
	/*
	if((pWidget->config >> 6) == 0)
		BMPtoBuf(pWidget->x, pWidget->y, pWidget->w, pWidget->h, pWidget->config & WIDGET_FOCUS, (uint8_t *)pWidget->pFeature);
	else
	{
		uint8_t rotation = pWidget->config >> 6;
		uint8_t *ptr1;
		if(rotation == 1)
		{		
			uint8_t w, h;
			uint8_t i, j;
			h = pWidget->w / 8 + ((pWidget->w % 8)!=0?1:0);
			w = pWidget->h;
			ptr1 = malloc(w * h);
			for(i = 0; i < h; i++)
			{
				for(j = 0; j < w; j++)
				{
					ptr1[i * w + j] = aabb(j, i, pWidget);
				}
			}
			BMPtoBuf(pWidget->x, pWidget->y, w, h * 8, pWidget->config & WIDGET_FOCUS, ptr1);
			free(ptr1);
		}
		else if(rotation == 2)
		{
			uint16_t temp = pWidget->w * pWidget->h / 8;
			uint8_t *ptr2 = (uint8_t *)pWidget->pFeature + temp - 1;
			uint16_t i, j;
			ptr1 = malloc(temp);
			for(i = 0; i < temp; i++)
			{
				ptr1[i] = 0;
				for(j = 0; j < 8; j++)
				{
					if(*ptr2 & (0x01 << j))
						ptr1[i] |= (0x80 >> j);
				}
				ptr2--;
			}
			BMPtoBuf(pWidget->x, pWidget->y, pWidget->w, pWidget->h, pWidget->config & WIDGET_FOCUS, ptr1);
			free(ptr1);
		}
		else if(rotation == 3)
		{
		}
	}
	*/
}
