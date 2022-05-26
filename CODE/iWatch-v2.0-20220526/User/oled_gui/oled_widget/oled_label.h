#ifndef _OLED_LABEL_H
#define _OLED_LABEL_H

#include "oled_font.h"

#ifdef _OLED_LABEL_C
#include "oled_widget.h"
typedef struct{
	uint8_t font;
	uint8_t Str;
}t_sLabel;
#define	LABEL_EXTERN
#else
#define	LABEL_EXTERN	extern
#endif

LABEL_EXTERN uint8_t label_str[64];

t_psWidget og_label_create(uint8_t font, int16_t x, int16_t y);
void og_label_setText(t_psWidget label, const char *str);
void og_label_appendText(t_psWidget pWidget, const char *str);
void og_label_clear(t_psWidget pWidget);
void og_label_draw(t_psWidget pWidget);
#define	lablel_printf(pWidget, ...)	\
{																							\
	sprintf(label_str, ##__VA_ARGS__);			\
	og_label_setText(pWidget, label_str);		\
}

#endif
