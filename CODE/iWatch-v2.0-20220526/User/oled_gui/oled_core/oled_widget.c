#define _OLED_WIDGET_C
#include "oled_widget.h"

#if OLED_USE_BMP == 1
#include "oled_bmp.h"
#endif

#if OLED_USE_LABEL == 1
#include "oled_label.h"
#endif

#if OLED_USE_CHECKBOX == 1
#include "oled_checkbox.h"
#endif

#if OLED_USE_SLIDER == 1
#include "oled_slider.h"
#endif

#if OLED_USE_SWITCH == 1
#include "oled_switch.h"
#endif

#if OLED_USE_CANVAS == 1
#include "oled_canvas.h"
#endif

static t_psWidget pFirst, pLast;
static uint8_t	ucSum;

void og_widget_init(void)
{
	pFirst = NULL;
	pLast = NULL;
	ucSum = 0;
}
t_psWidget og_widget_create(void)
{
	t_psWidget pWidget = malloc(T_SWIDGET_SIZE);
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("widget create fail");
		return NULL;
	}
	#endif
	pWidget->config = WIDGET_SHOW;
	pWidget->pFeature = NULL;
	return pWidget;
}
void og_widget_add(t_psWidget pWidget)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to add a null widget to widget_list");
		return;
	}
	#endif
	if(ucSum == 0)
	{
		pWidget->pPrev = NULL;
		pWidget->pNext = NULL;
		pFirst = pWidget;
		pLast = pWidget;
	}
	else
	{
		pWidget->pPrev = pLast;
		pWidget->pNext = NULL;
		pLast->pNext = pWidget;
		pLast = pWidget;
	}
	ucSum++;
}
void og_widget_delet(t_psWidget pWidget)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to delet a null widget from widget_list");
		return;
	}
	#endif
	
	if(pWidget == pFirst)
	{
		if(pWidget->pNext != NULL)
		{
			((t_psWidget)pWidget->pNext)->pPrev = NULL;
			pFirst = (t_psWidget)pWidget->pNext;
		}
		else
		{
			pFirst = NULL;
			pLast = NULL;
		}
	}
	else if(pWidget == pLast)
	{
		((t_psWidget)pWidget->pPrev)->pNext = NULL; 
		pLast = (t_psWidget)pWidget->pPrev;
	}
	else
	{
		((t_psWidget)pWidget->pPrev)->pNext = pWidget->pNext;
		((t_psWidget)pWidget->pNext)->pPrev = pWidget->pPrev;
	}
	ucSum--;
	switch(pWidget->config&0x0f)
	{
		case LABEL_WIDGET:
		case SLIDER_WIDGET:
		case CANVAS_WIDGET:
		{
			if(pWidget->pFeature != NULL)
				free(pWidget->pFeature);
			break;
		}
		case BMP_WIDGET:
		case CHECKBOX_WIDGET:
		case SWITCH_WIDGET:
			break;
	}
	free(pWidget);
	pWidget = NULL;
}
void og_widget_setShow(t_psWidget pWidget, uint8_t k)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");
		return;
	}
	#endif
	if(k)
		pWidget->config |= WIDGET_SHOW;
	else
		pWidget->config &= ~WIDGET_SHOW;
}
void og_widget_setPos(t_psWidget pWidget, int16_t x, int16_t y)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");
		return;
	}
	#endif
	pWidget->x = x;
	pWidget->y = y;
}
void og_widget_setPosOffset(t_psWidget pWidget, int16_t x, int16_t y)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");
		return;
	}
	#endif
	pWidget->x += x;
	pWidget->y += y;
}
void og_widget_setAlignment(t_psWidget pWidget, uint8_t alignment)
{
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");
		return;
	}
	#endif
	switch(alignment)
	{
		case ALIGN_TOP:
			pWidget->y = 0;
			break;
		case ALIGN_BOTTOM:
			pWidget->y = OLED_HEIGHT - pWidget->h;
			break;
		case ALIGN_LEFT:
			pWidget->x = 0;
			break;
		case ALIGN_RIGHT:
			pWidget->x = OLED_WIDTH - pWidget->w;
			break;
		case ALIGN_HORIZONMID:
			pWidget->x = (OLED_WIDTH - pWidget->w) / 2;
			break;
		case ALIGN_VERTICALMID:
			pWidget->y = (OLED_HEIGHT - pWidget->h) / 2;
			break;
		case ALIGN_TOPLEFT:
			pWidget->x = 0;
			pWidget->y = 0;
			break;
		case ALIGN_TOPMID:
			pWidget->x = (OLED_WIDTH - pWidget->w) / 2;
			pWidget->y = 0;
			break;
		case ALIGN_TOPRIGHT:
			pWidget->x = OLED_WIDTH - pWidget->w;
			pWidget->y = 0;
			break;
		case ALIGN_BOTTOMLEFT:
			pWidget->x = 0;
			pWidget->y = OLED_HEIGHT - pWidget->h;
			break;
		case ALIGN_BOTTOMMID:
			pWidget->x = (OLED_WIDTH - pWidget->w) / 2;
			pWidget->y = OLED_HEIGHT - pWidget->h;
			break;
		case ALIGN_BOTTOMRIGHT:
			pWidget->x = OLED_WIDTH - pWidget->w;
			pWidget->y = OLED_HEIGHT - pWidget->h;
			break;
	}
}

void og_widget_draw(void)
{
	t_psWidget pWidget = pFirst;
	og_clearDisplayBuf();
	while(pWidget != NULL)
	{
		if((pWidget->config & WIDGET_SHOW) == 0)
			goto next;
		if((pWidget->x >= OLED_WIDTH) || (pWidget->x + pWidget->w) <= 0)
			goto next;
		if((pWidget->y >= OLED_HEIGHT) || (pWidget->y + pWidget->h) <= 0)
			goto next;
		switch(pWidget->config & 0x0f)
		{
			#if OLED_USE_BMP == 1
			case BMP_WIDGET:
			{
				og_bmp_draw(pWidget);
				break;
			}
			#endif
			#if OLED_USE_SWITCH == 1
			case SWITCH_WIDGET:
			{
				og_switch_draw(pWidget);
				break;
			}
			#endif
			#if OLED_USE_CHECKBOX == 1
			case CHECKBOX_WIDGET:
			{
				og_checkbox_draw(pWidget);
				break;
			}
			#endif
			#if OLED_USE_LABEL == 1
			case LABEL_WIDGET:
			{
				og_label_draw(pWidget);
				break;
			}
			#endif
			#if OLED_USE_SLIDER == 1
			case SLIDER_WIDGET:
			{
				og_slider_draw(pWidget);
				break;
			}
			#endif
			#if OLED_USE_CANVAS == 1
			case CANVAS_WIDGET:
			{
				og_canvas_draw(pWidget);
				break;
			}
			#endif
		}
		next:
		pWidget = (t_psWidget)pWidget->pNext;
	}
	og_screenRefresh();
}
