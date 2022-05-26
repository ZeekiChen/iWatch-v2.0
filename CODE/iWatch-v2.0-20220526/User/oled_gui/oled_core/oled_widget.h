#ifndef _OLED_WIDGET_H
#define _OLED_WIDGET_H

#include "oled_conf.h"
#if (OLED_USE_LOG == 0)
#include "oled_log.h"
#endif
#include "oled_draw.h"
#include <stdlib.h>

typedef	struct sWidget{
	int16_t x;			// �ÿؼ�������
	int16_t y;
	int16_t w;			// �ÿؼ��Ŀ��
	int16_t h;			// �ÿؼ��ĸ߶�
	uint8_t config;	// �ÿؼ��Ĳ���
	void *pFeature;	// ��ͬ���͵Ŀؼ����еĲ���
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
	BMP_WIDGET,				// ͼƬ�ؼ�
	LABEL_WIDGET,			// ��ǩ�ؼ�
	CANVAS_WIDGET,		// �����ؼ�
	CHECKBOX_WIDGET,	// ��ѡ��ؼ�
	SLIDER_WIDGET,		// ����ؼ�
	SWITCH_WIDGET			// ���ؿؼ�
};

enum WIDGET_ALIGN
{
	ALIGN_TOP,					//��������
	ALIGN_BOTTOM,				//�ײ�����
	ALIGN_LEFT,					//������
	ALIGN_RIGHT,				//�Ҳ����
	ALIGN_HORIZONMID,		//ˮƽ�м����
	ALIGN_VERTICALMID,	//��ֱ�м����
	ALIGN_TOPLEFT,			//����������
	ALIGN_TOPMID,				//�����м����
	ALIGN_TOPRIGHT,			//�����Ҳ����
	ALIGN_BOTTOMLEFT,		//�ײ�������
	ALIGN_BOTTOMMID,		//�ײ��м����
	ALIGN_BOTTOMRIGHT,	//�ײ��Ҳ����
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

