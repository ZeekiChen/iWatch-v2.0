#define _OLED_CHECKBOX_C
#include "oled_checkbox.h"

t_psWidget og_checkbox_create(uint8_t ucSize, uint8_t ucStatus, int16_t x, int16_t y)
{
	t_psWidget pWidget = og_widget_create();
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("switch widget create fail");
		return NULL;
	}
	#endif
	if(ucStatus)
	{
		ucStatus = 1;
		pWidget->config |= 0x80;
	}
	else
	{
		pWidget->config &= ~0x80;
	}
	if(ucSize == 8)
	{
		pWidget->config &= ~0x40;
		pWidget->pFeature = (void *)bmp_checkbox8[ucStatus];
		pWidget->w = 8;
		pWidget->h = 8;
	}
	else if(ucSize == 16)
	{
		pWidget->config |= 0x40;
		pWidget->pFeature = (void *)bmp_checkbox16[ucStatus];
		pWidget->w = 16;
		pWidget->h = 16;
	}
	pWidget->x = x;
	pWidget->y = y;
	pWidget->config |= CHECKBOX_WIDGET;
	og_widget_add(pWidget);
	return pWidget;
}
void og_checkbox_setCheck(t_psWidget pWidget, uint8_t ucStatus)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");
		return;
	}
	if(pWidget->type != CHECKBOX_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	if(ucStatus)
	{
		ucStatus = 1;
		pWidget->config |= 0x80;
	}
	else
	{
		pWidget->config &= ~0x80;
	}
	if((pWidget->config & 0x40) == 0)
		pWidget->pFeature = (void *)bmp_checkbox8[ucStatus];
	else
		pWidget->pFeature = (void *)bmp_checkbox16[ucStatus];
	return;
}
void og_checkbox_draw(t_psWidget pWidget)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");
		return;
	}
	if(pWidget->type != CHECKBOX_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	BMPtoBuf(pWidget->x, pWidget->y, pWidget->w, pWidget->h, pWidget->config & WIDGET_FOCUS, (uint8_t *)pWidget->pFeature);
}
