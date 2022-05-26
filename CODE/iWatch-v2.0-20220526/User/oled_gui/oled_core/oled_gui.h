#ifndef _OLED_GUI_H
#define _OLED_GUI_H

#include "oled_conf.h"
#include "oled_widget.h"

#if OLED_USE_ANIM == 1
#include "oled_anim.h"
#endif

#if OLED_USE_GROUP == 1
#include "oled_group.h"
#endif

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

#ifdef __OELD_GUI_C
static uint16_t og_tick = 0;							// 当前时间戳
#endif

void og_timeTick(void);
uint16_t og_getTimestamp(void);
uint16_t og_tickElaps(uint16_t pre_tick);
void og_init(void);
void og_run(void);

#endif
