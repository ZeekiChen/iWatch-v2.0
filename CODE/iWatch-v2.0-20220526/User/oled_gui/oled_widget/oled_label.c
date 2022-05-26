#define _OLED_LABEL_C
#include "oled_label.h"

uint16_t str_len(const char *str)
{
	uint16_t cnt = 0;
	if(str == NULL)
		return 0;
	while(*str)
	{
		str++;
		cnt++;
	};
	return cnt;
}
void str_cpy(const char *str_source, char *str_target)
{
	while(*str_source)
	{
		*str_target++ = *str_source++;
	};
	*str_target = '\0';
}
uint8_t str_cmp(const char *str_source, char *str_target)
{
	while(*str_source)
	{
		if(*str_target++ != *str_source++)
			return 0;
	};
	if(*str_target)
		return 0;
	return 1;
}

t_psWidget og_label_create(uint8_t font, int16_t x, int16_t y)
{
	t_psWidget pWidget = og_widget_create();
	t_sLabel *pLabel;
	#ifdef OLED_DEBUG
	if (pWidget == NULL)					// 如果内存分配失败，直接返回NULL
	{
		OG_PRINTERROR("label widget create fail");
		return NULL;
	}
	if(pLabel == NULL)
	{
		free(pWidget);
		OG_PRINTERROR("label widget create fail");
		return NULL;
	}	
	#endif
	pWidget->x = x;										// 在屏幕上的横坐标
	pWidget->y = y;										// 在屏幕上的纵坐标
	pWidget->config |= LABEL_WIDGET;	// 控件类型为label
	pWidget->pFeature = malloc(sizeof(t_sLabel));
	#ifdef OLED_DEBUG
	if(pWidget->pFeature == NULL)
	{
		free(pWidget);
		OG_PRINTERROR("label create fail");
		return NULL;
	}
	#endif
	pLabel = (t_sLabel *)pWidget->pFeature;
	pLabel->font = font;
	pLabel->Str = '\0';
	og_widget_add(pWidget);					// 将控件添加到控件显示器中
	return pWidget;									// 返回控件参数的指针
}
void og_label_setText(t_psWidget pWidget, const char *str)
{
	t_sLabel *pLabel = (t_sLabel *)(pWidget->pFeature);
	uint8_t line_cnt = 0;
	uint8_t line_char_max = 0;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != LABEL_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	if(str_cmp(str, (char *)&pLabel->Str) == 1)
		return;
	else
	{
		uint8_t *ptr = (uint8_t *)str;
		uint16_t str_sum = 0;
		uint8_t line_char_cnt = 0;
		while(*ptr)
		{
			if(*ptr == '\r' || *ptr == '\n')
			{
				line_cnt++;
				if(line_char_cnt > line_char_max)
					line_char_max = line_char_cnt;
				line_char_cnt = 0;
			}
			else
			{			
				line_char_cnt++;
			}
			str_sum++;
			ptr++;
		}
		if(line_char_cnt != 0)
		{
			line_cnt++;
			if(line_char_cnt > line_char_max)
				line_char_max = line_char_cnt;
		}
		ptr = realloc((uint8_t *)pWidget->pFeature, sizeof(t_sLabel) + str_sum);
		#ifdef OLED_DEBUG
		if(ptr == NULL)
		{
			OG_PRINTERROR("label set text fail");
			return;
		}
		#endif
		pWidget->pFeature = ptr;
		pLabel = (t_sLabel *)pWidget->pFeature;
		str_cpy(str, (char *)&pLabel->Str);
	}
	pWidget->w = line_char_max * sFont[pLabel->font].w;
	pWidget->h = line_cnt * sFont[pLabel->font].h;
	#if (OLED_USE_BUILDIN_FONT == 0)
	if(pLabel->font == FONT_GB2312_15X16)
		pWidget->w >>= 1;
	#endif
}
void og_label_appendText(t_psWidget pWidget, const char *str)
{
	t_sLabel *pLabel = (t_sLabel *)(pWidget->pFeature);
	uint8_t *ptr;
	uint16_t str_sum1, str_sum2;
	uint8_t line_cnt = 0;
	uint8_t line_char_cnt = 0;
	uint8_t line_char_max = 0;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != LABEL_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	if(pLabel->Str == '\0')
	{
		og_label_setText(pWidget, str);
		return;
	}
	str_sum1 = str_len(str);
	if(str_sum1 == 0)
		return;
	str_sum2 = str_len((char *)(&pLabel->Str));
	ptr = realloc((uint8_t *)pWidget->pFeature, sizeof(t_sLabel) + str_sum1 + str_sum2);
	#ifdef OLED_DEBUG
	if(ptr == NULL)
	{
		OG_PRINTERROR("label append text fail");
		return;
	}
	#endif
	pWidget->pFeature = ptr;
	pLabel = (t_sLabel *)pWidget->pFeature;
	str_cpy(str, (char *)(&pLabel->Str) + str_sum2);
	ptr = &pLabel->Str;
	while(*ptr)
	{
		if(*ptr == '\r' || *ptr == '\n')
		{
			line_cnt++;
			if(line_char_cnt > line_char_max)
				line_char_max = line_char_cnt;
			line_char_cnt = 0;
		}
		else
		{
			line_char_cnt++;
		}
		ptr++;
	}
	if(line_char_cnt != 0)
	{
		line_cnt++;
		if(line_char_cnt > line_char_max)
			line_char_max = line_char_cnt;
	}
	pWidget->w = line_char_max * sFont[pLabel->font].w;
	pWidget->h = line_cnt * sFont[pLabel->font].h;
	#if (OLED_USE_BUILDIN_FONT == 0)
	if(pLabel->font == FONT_GB2312_15X16)
		pWidget->w >>= 1;
	#endif
}
void og_label_clear(t_psWidget pWidget)
{
	t_sLabel *pLabel;
	uint8_t *ptr;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != LABEL_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	ptr = realloc(pWidget->pFeature, sizeof(t_sLabel));
	#ifdef OLED_DEBUG
	if(ptr == NULL)
	{
		OG_PRINTERROR("label clear fail");
		return;
	}
	#endif
	pWidget->pFeature = ptr;
	pLabel = (t_sLabel *)pWidget->pFeature;
	pLabel->Str = '\0';
}
void og_label_draw(t_psWidget pWidget)
{
	t_sLabel *pLabel = (t_sLabel *)(pWidget->pFeature);
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{	
		OG_PRINTERROR("try to set a null widget");		
		return;
	}
	if((pWidget->config & 0x0f) != LABEL_WIDGET)
	{
		OG_PRINTERROR("wrong type of widget for this method");
		return;
	}
	#endif
	if(pLabel->Str == '\0')
		return;
	else
	{	
		uint8_t *pStr = (uint8_t *)&pLabel->Str;
		uint8_t *ptr;
		int16_t y = 0;
		uint16_t i = 0, ptr_size;
		#if (OLED_USE_BUILDIN_FONT == 1)
		switch(pLabel->font)
		{
			case FONT_ASCII_6X8:
			{
				ptr_size = pWidget->w;
				ptr = calloc(ptr_size, 1);
				#ifdef OLED_DEBUG
				if(ptr == NULL)
				{
					OG_PRINTERROR("label draw fail");
					return;
				}
				#endif
				while(1)
				{
					if(*pStr < ' ') 
					{
						if(((pWidget->y + y + 8) >= 0) && (i != 0))
						{
							uint8_t *ptr2 = pStr - i;
							uint16_t j;
							uint8_t *offset = ptr;
							for(j = 0; j < i; j++)
							{
								font_getCharASCII_6X8(ptr2[j], offset);
								offset += 7;	
							}
							BMPtoBuf(pWidget->x, pWidget->y + y, pWidget->w, 8, pWidget->config & WIDGET_FOCUS, ptr);
							for(j = 0; j < ptr_size; j++)
								ptr[j] = 0x00;
						}
						if(*pStr == '\0')
							break;
						i = 0;
						y += 8;
					}
					else
						i++;
					pStr++;
				}
				break;
			}
			case FONT_ASCII_8X16:
			{
				ptr_size = (pWidget->w << 1);
				ptr = calloc(ptr_size, 1);
				#ifdef OLED_DEBUG
				if(ptr == NULL)
				{
					OG_PRINTERROR("label draw fail");
					return;
				}
				#endif
				while(1)
				{
					if(*pStr < ' ') 
					{
						if(((pWidget->y + y + 16) >= 0) && (i != 0))
						{
							uint8_t *ptr2 = pStr - i;
							uint16_t j;
							uint8_t *offset = ptr;
							for(j = 0; j < i; j++)
							{
								font_getCharASCII_8X16(ptr2[j], offset, pWidget->w);
								offset += 8;	
							}
							BMPtoBuf(pWidget->x, pWidget->y + y, pWidget->w, 16, pWidget->config & WIDGET_FOCUS, ptr);
							for(j = 0; j < ptr_size; j++)
								ptr[j] = 0x00;
						}
						if(*pStr == '\0')
							break;
						i = 0;
						y += 16;
					}
					else
						i++;
					pStr++;
				}
				break;
			}
			case FONT_ASCII_16X32:
			{
				ptr_size = (pWidget->w << 2);
				ptr = calloc(ptr_size, 1);
				#ifdef OLED_DEBUG
				if(ptr == NULL)
				{
					OG_PRINTERROR("label draw fail");
					return;
				}
				#endif
				while(1)
				{
					if(*pStr < ' ') 
					{
						if(((pWidget->y + y + 32) >= 0) && (i != 0))
						{
							uint8_t *ptr2 = pStr - i;
							uint16_t j;
							uint8_t *offset = ptr;
							for(j = 0; j < i; j++)
							{
								font_getCharASCII_16X32(ptr2[j], offset, pWidget->w);
								offset += 16;	
							}
							BMPtoBuf(pWidget->x, pWidget->y + y, pWidget->w, 32, pWidget->config & WIDGET_FOCUS, ptr);
							for(j = 0; j < ptr_size; j++)
								ptr[j] = 0x00;
						}
						if(*pStr == '\0')
							break;
						i = 0;
						y += 32;
					}
					else
						i++;
					pStr++;
				}
				break;
			}
		}
		#endif
		#if (OLED_USE_BUILDIN_FONT == 0)
		switch(pLabel->font)
		{
			case FONT_ASCII_5X7:
			{
				ptr_size = pWidget->w;
				ptr = calloc(ptr_size, 1);
				#ifdef OLED_DEBUG
				if(ptr == NULL)
				{
					OG_PRINTERROR("label draw fail");
					return;
				}
				#endif
				while(1)
				{
					if(*pStr < ' ') 
					{
						if(((pWidget->y + y + 8) >= 0) && (i != 0))
						{
							uint8_t *ptr2 = pStr - i;
							uint16_t j;
							uint8_t *offset = ptr;
							for(j = 0; j < i; j++)
							{
								font_getCharASCII_5X7(ptr2[j], offset);
								offset += 6;	
							}
							BMPtoBuf(pWidget->x, pWidget->y + y, pWidget->w, 8, pWidget->config & WIDGET_FOCUS, ptr);
							for(j = 0; j < ptr_size; j++)
								ptr[j] = 0x00;
						}
						if(*pStr == '\0')
							break;
						i = 0;
						y += 8;
					}
					else
						i++;
					pStr++;
				}
				break;
			}
			case FONT_ASCII_7X8:
			{
				ptr_size = pWidget->w;
				ptr = calloc(ptr_size, 1);
				#ifdef OLED_DEBUG
				if(ptr == NULL)
				{
					OG_PRINTERROR("label draw fail");
					return;
				}
				#endif
				while(1)
				{
					if(*pStr < ' ') 
					{
						if(((pWidget->y + y + 8) >= 0) && (i != 0))
						{
							uint8_t *ptr2 = pStr - i;
							uint16_t j;
							uint8_t *offset = ptr;
							for(j = 0; j < i; j++)
							{
								font_getCharASCII_7X8(ptr2[j], offset);
								offset += 8;	
							}
							BMPtoBuf(pWidget->x, pWidget->y + y, pWidget->w, 8, pWidget->config & WIDGET_FOCUS, ptr);
							for(j = 0; j < ptr_size; j++)
								ptr[j] = 0x00;
						}
						if(*pStr == '\0')
							break;
						i = 0;
						y += 8;
					}
					else
						i++;
					pStr++;
				}
				break;
			}
			case FONT_ASCII_8X16:
			{
				ptr_size = (pWidget->w << 1);
				ptr = calloc(ptr_size, 1);
				#ifdef OLED_DEBUG
				if(ptr == NULL)
				{
					OG_PRINTERROR("label draw fail");
					return;
				}
				#endif
				while(1)
				{
					if(*pStr < ' ') 
					{
						if(((pWidget->y + y + 16) >= 0) && (i != 0))
						{
							uint8_t *ptr2 = pStr - i;
							uint16_t j;
							uint8_t *offset = ptr;
							for(j = 0; j < i; j++)
							{
								font_getCharASCII_8X16(ptr2[j], offset, pWidget->w);
								offset += 8;	
							}
							BMPtoBuf(pWidget->x, pWidget->y + y, pWidget->w, 16, pWidget->config & WIDGET_FOCUS, ptr);
							for(j = 0; j < ptr_size; j++)
								ptr[j] = 0x00;
						}
						if(*pStr == '\0')
							break;
						i = 0;
						y += 16;
					}
					else
						i++;
					pStr++;
				}
				break;
			}
			case FONT_ASCII_8X16_BOLD:
			{
				ptr_size = (pWidget->w << 1);
				ptr = calloc(ptr_size, 1);
				#ifdef OLED_DEBUG
				if(ptr == NULL)
				{
					OG_PRINTERROR("label draw fail");
					return;
				}
				#endif
				while(1)
				{
					if(*pStr < ' ') 
					{
						if(((pWidget->y + y + 16) >= 0) && (i != 0))
						{
							uint8_t *ptr2 = pStr - i;
							uint16_t j;
							uint8_t *offset = ptr;
							for(j = 0; j < i; j++)
							{
								font_getCharASCII_8X16_BOLD(ptr2[j], offset, pWidget->w);
								offset += 8;	
							}
							BMPtoBuf(pWidget->x, pWidget->y + y, pWidget->w, 16, pWidget->config & WIDGET_FOCUS, ptr);
							for(j = 0; j < ptr_size; j++)
								ptr[j] = 0x00;
						}
						if(*pStr == '\0')
							break;
						i = 0;
						y += 16;
					}
					else
						i++;
					pStr++;
				}
				break;
			}
			case FONT_GB2312_15X16:
			{
				ptr_size = (pWidget->w << 1);
				ptr = calloc(ptr_size, 1);
				#ifdef OLED_DEBUG
				if(ptr == NULL)
				{
					OG_PRINTERROR("label draw fail");
					return;
				}
				#endif
				while(1)
				{
					if(*pStr < ' ') 
					{
						if(((pWidget->y + y + 16) >= 0) && (i != 0))
						{
							uint16_t *ptr2 = (uint16_t *)(pStr - (i << 1));
							uint16_t j;
							uint8_t *offset = ptr;
							for(j = 0; j < i; j++)
							{
								font_getCharGB2312_15X16(ptr2[j], offset, pWidget->w);
								offset += 16;	
							}
							BMPtoBuf(pWidget->x, pWidget->y + y, pWidget->w, 16, pWidget->config & WIDGET_FOCUS, ptr);
							for(j = 0; j < ptr_size; j++)
								ptr[j] = 0x00;
						}
						if(*pStr == '\0')
							break;
						i = 0;
						y += 16;
						pStr += 1;
					}
					else
					{
						i++;
						pStr += 2;
					}
				}
				break;
			}
		}
		#endif
		free(ptr);
		return;
	}
}

