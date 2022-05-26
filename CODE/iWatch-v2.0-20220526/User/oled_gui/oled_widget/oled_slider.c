#define _OLED_SLIDER_C
#include "oled_slider.h"

//将一个16位有符号整型数转换成字符串，并返回字符串的长度
uint8_t int16ToStr(char *str, int16_t num)
{
	unsigned char i = 0;
	if(num < 0)
	{
		str[i++] = '-';
		num = -num;
	}
	if(num >= 10000)
	{
		str[i++] = num / 10000 + '0';
		goto step1;
	}
	else if(num >= 1000)
	{
		str[i++] = num / 1000 + '0';
		goto step2;
	}
	else if(num >= 100)
	{
		str[i++] = num / 100 + '0';
		goto step3;
	}
	else if(num >= 10)
	{
		str[i++] = num / 10 + '0';
		goto step4;
	}
	else
	{
		str[i++] = num + '0';
		return i;
	}
	step1:
	str[i++] = num % 10000 / 1000 + '0';
	step2:
	str[i++] = num % 1000 / 100 + '0';
	step3:
	str[i++] = num % 100 / 10 + '0';
	step4:
	str[i++] = num % 10 + '0';
	return i;
}
t_psWidget og_slider_create(int16_t min, int16_t max, int16_t value, uint8_t length, int16_t x, int16_t y)
{
	t_psWidget pWidget;
	t_sSlider *pSlider;
	pWidget = og_widget_create();
	pSlider = malloc(sizeof(t_sSlider));
	#ifdef OLED_DEBUG
	if(max <= min)
	{
		OG_PRINTERROR("the maximum value of slider is smaller than its minimum value");
		return NULL;
	}
	if(pWidget == NULL)
	{
		OG_PRINTERROR("slider widget create fail");
		return NULL;
	}
	if(pSlider == NULL)
	{
		free(pWidget);
		og_widget_delet(pWidget);
		return NULL;
	}	
	#endif
	pWidget->x = x;
	pWidget->y = y;
	pWidget->config |= SLIDER_WIDGET;
	pWidget->pFeature = pSlider;
	pWidget->w = length;
	pWidget->h = 8;
	pSlider->min = min;
	pSlider->max = max;
	og_slider_setValue(pWidget, value);
	og_widget_add(pWidget);
	return pWidget;
}
void og_slider_setValue(t_psWidget pWidget, int16_t value)
{
	t_sSlider *pSlider = (t_sSlider *)pWidget->pFeature;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != SLIDER_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	if(pSlider->value == value)
		return;
	pSlider->value = value < pSlider->max ? value : pSlider->max;
	pSlider->value = value > pSlider->min ? value : pSlider->min;
}
int16_t og_slider_getValue(t_psWidget pWidget)
{
	t_sSlider *pSlider = (t_sSlider *)pWidget->pFeature;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");		
		return 0;
	}
	if((pWidget->config & 0x0f) != SLIDER_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return 0;
	}
	#endif
	return pSlider->value;
}
void og_slider_draw(t_psWidget pWidget)
{
	t_sSlider *pSlider = (t_sSlider *)pWidget->pFeature;
	char str[8];
	uint8_t i, temp;
	uint8_t *ptr;
	uint8_t slider_len;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != SLIDER_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	ptr = calloc(pWidget->w, 1);
	#ifdef OLED_DEBUG
	if(ptr == NULL)
	{
		OG_PRINTERROR("slider draw fail");
		return;
	}
	#endif
	temp = int16ToStr(str, pSlider->value);
	slider_len = pWidget->w - temp * 6;
	for(i = 0; i < temp; i++)
	{
		#if OLED_USE_BUILDIN_FONT == 1
		font_getCharASCII_6X8(str[i], ptr + slider_len + i * 6);
		#else
		font_getCharASCII_5X7(str[i], ptr + slider_len + i * 6);
		#endif
	}
	slider_len -= 2;
	for(i = 1; i < slider_len; i++)
	{
		ptr[i] = 0x18;
	}
	temp = 1 + (float)(pSlider->value - pSlider->min) / (pSlider->max - pSlider->min) * (slider_len - 2);
	ptr[temp - 1] = 0xFF;
	ptr[temp] = 0xFF;
	ptr[temp + 1] = 0xFF;
	BMPtoBuf(pWidget->x, pWidget->y, pWidget->w, pWidget->h, pWidget->config & WIDGET_FOCUS, ptr);
	free(ptr);
}
