#ifndef _OLED_GROUP_H
#define _OLED_GROUP_H

#include "oled_conf.h"
#include "oled_widget.h"
#if (OLED_USE_ANIM == 1)
#include "oled_anim.h"
#endif

enum FOCUS{
	FOCUS_PREV,
	FOCUS_NEXT
};

typedef struct{
	t_psWidget pWidget;
	uint8_t focus_en;
}t_sGroupItem;
#define	T_SGROUPITEM_SIZE	sizeof(t_sGroupItem)
typedef struct{
	uint8_t ucSum;
	int8_t focus_index;
	t_sGroupItem item;
}t_sGroup;
#define	T_SGROUP_SIZE	sizeof(t_sGroup)
typedef t_sGroup * t_psGroup;

t_psGroup og_group_create(unsigned char i);
void og_group_addWidget(t_psGroup pGroup, t_psWidget pWidget, uint8_t focus_en);
void og_group_setPosOffset(t_psGroup pGroup, int16_t delta_x, int16_t delta_y);
#if OLED_USE_ANIM == 1
void og_group_addAnimOffset(t_psGroup pGroup, int16_t delta_x, int16_t delta_y, uint16_t duration, t_anim_end_cb anim_end_cb);
#endif
void og_group_delet(t_psGroup pGroup);
void og_group_focusWidget(t_psGroup pGroup, uint8_t direction);
t_psWidget og_group_getFocusWidget(t_psGroup pGroup);
int16_t og_group_getFocusIndex(t_psGroup pGroup);
void og_group_hideOffScreenWidget(t_psGroup pGroup);

#endif

