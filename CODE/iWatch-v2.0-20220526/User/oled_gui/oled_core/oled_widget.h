#ifndef _OLED_WIDGET_H
#define _OLED_WIDGET_H

#include "oled_conf.h"
#if (OLED_USE_LOG == 0)
#include "oled_log.h"
#endif
#include "oled_draw.h"
#include <stdlib.h>

typedef	struct sWidget{
	int16_t x;			// 该控件的坐标
	int16_t y;
	int16_t w;			// 该控件的宽度
	int16_t h;			// 该控件的高度
	uint8_t config;	// 该控件的参数
	void *pFeature;	// 不同类型的控件特有的参数
	void *pPrev;
	void *pNext;
}t_sWidget;
#define	T_SWIDGET_SIZE	sizeof(t_sWidget)
typedef	t_sWidget * t_psWidget;

//sWidget.config[3:0]:widget_type
//sWidget.config[5:4]:widget_show,widget_focus
//sWidget.config[7:6]:not used
#define	WIDGET_SHOW		0x10
#define	WIDGET_FOCUS	0x20
enum WIDGET_TYPE
{
	BMP_WIDGET,				// 图片控件
	LABEL_WIDGET,			// 标签控件
	CANVAS_WIDGET,		// 画布控件
	CHECKBOX_WIDGET,	// 复选框控件
	SLIDER_WIDGET,		// 滑块控件
	SWITCH_WIDGET			// 开关控件
};

enum WIDGET_ALIGN
{
	ALIGN_TOP,					//顶部对齐
	ALIGN_BOTTOM,				//底部对齐
	ALIGN_LEFT,					//左侧对齐
	ALIGN_RIGHT,				//右侧对齐
	ALIGN_HORIZONMID,		//水平中间对齐
	ALIGN_VERTICALMID,	//垂直中间对齐
	ALIGN_TOPLEFT,			//顶部左侧对齐
	ALIGN_TOPMID,				//顶部中间对齐
	ALIGN_TOPRIGHT,			//顶部右侧对齐
	ALIGN_BOTTOMLEFT,		//底部左侧对齐
	ALIGN_BOTTOMMID,		//底部中间对齐
	ALIGN_BOTTOMRIGHT,	//底部右侧对齐
};

/**************************widget_manage***********************************/
void og_widget_init(void);
t_psWidget og_widget_create(void);
void og_widget_delet(t_psWidget pWidget);
void og_widget_add(t_psWidget pWidget);
void og_widget_setShow(t_psWidget pWidget, uint8_t k);
void og_widget_setPos(t_psWidget pWidget, int16_t x, int16_t y);
void og_widget_setPosOffset(t_psWidget pWidget, int16_t x, int16_t y);
void og_widget_setAlignment(t_psWidget pWidget, uint8_t alignment);
void og_widget_draw(void);

#endif

