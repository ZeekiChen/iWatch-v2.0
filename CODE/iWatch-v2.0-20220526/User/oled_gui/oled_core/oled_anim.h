#ifndef _OLED_ANIM_H
#define _OLED_ANIM_H

typedef void(*t_anim_end_cb)(void);
#define	ANIM_NULL_CB	(t_anim_end_cb)0

#include "oled_widget.h"

typedef struct sAmin{
	t_psWidget pWidget;
	int16_t start_x;
	int16_t start_y;
	int16_t distance_x;
	int16_t distance_y;
	uint16_t duration;
	uint16_t act_time;
	t_anim_end_cb anim_end_cb;
	void *pPrev;
	void *pNext;
}t_sAnim;
#define	T_ANIM_SIZE	sizeof(t_sAnim)
typedef	t_sAnim * t_psAnim;

void og_anim_init(void);
void og_anim_run(void);
void og_anim_create(t_psWidget pWidget, int16_t end_x, int16_t end_y, uint16_t duration, t_anim_end_cb anim_end_cb);
void og_anim_create_offset(t_psWidget pWidget, int16_t delta_x, int16_t delta_y, uint16_t duration, t_anim_end_cb anim_end_cb);

#endif

