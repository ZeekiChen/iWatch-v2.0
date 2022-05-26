#define _OLED_SWITCH_C
#include "oled_switch.h"

t_psWidget og_switch_create(uint8_t ucSize, uint8_t ucStatus, int16_t x, int16_t y)
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
		pWidget->pFeature = (void *)bmp_switch8[ucStatus];
		pWidget->w = 12;
		pWidget->h = 8;
	}
	else if(ucSize == 16)
	{
		pWidget->config |= 0x40;
		pWidget->pFeature = (void *)bmp_switch16[ucStatus];
		pWidget->w = 24;
		pWidget->h = 16;
	}
	pWidget->x = x;
	pWidget->y = y;
	pWidget->config |= SWITCH_WIDGET;
	og_widget_add(pWidget);
	return pWidget;
}
void og_switch_setStatus(t_psWidget pWidget, uint8_t ucStatus)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");
		return;
	}
	if((pWidget->config & 0x0f) != SWITCH_WIDGET)
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
	if((pWidget->config | 0x40) == 0)
		pWidget->pFeature = (void *)bmp_switch8[ucStatus];
	else
		pWidget->pFeature = (void *)bmp_switch16[ucStatus];
}
uint8_t og_switch_getStatus(t_psWidget pWidget)
{
	if(pWidget->config & 0x80)
		return 1;
	else 
		return 0;
}
void og_switch_draw(t_psWidget pWidget)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");
		return;
	}
	if((pWidget->config & 0x0f) != SWITCH_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	BMPtoBuf(pWidget->x, pWidget->y, pWidget->w, pWidget->h, pWidget->config & WIDGET_FOCUS, (uint8_t *)pWidget->pFeature);
}
