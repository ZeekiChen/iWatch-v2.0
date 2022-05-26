#define	_OLED_ANIM_C
#include "oled_anim.h"
#include "oled_gui.h"
#include "math.h"

static t_psAnim pFirst, pLast;
static uint8_t ucSum;
static uint16_t anim_timestamp = 0;

void og_anim_init(void)
{
	pFirst = NULL;
	pLast = NULL;
	ucSum = 0;
}
#ifdef ANIM_BOUNCE
float CODE anim_bounce_table[32] = {
1.025757715,1.083824649,1.134734632,1.176754659,
1.208646412,1.22969821,1.239731069,1.239079506,
1.228548931,1.209352456,1.183030912,1.151360499,
1.116252999,1.07965368,1.043441987,1.009339863,
0.978831999,0.953101659,0.932984822,0.918944418,
0.911065396,0.909070257,0.912353713,0.920034112,
0.931018519,0.944077655,0.957926451,0.971305756,
0.983060726,0.992211633,0.998013307,1.0
};
#endif
float animate_fx(float t)
{
	#if ANIM_TYPE == 0	//ANIM_EASE_IN_EASE_OUT
	if(t <= 0.5)
		return(4*t*t*t);
	else
		return(4*(t-1)*(t-1)*(t-1)+1);
	#elif ANIM_TYPE == 1	//ANIM_EASE_IN
	return(t*t*t);
	#elif ANIM_TYPE == 2	//ANIM_EASE_OUT
	return((t-1)*(t-1)*(t-1)+1);
	#elif ANIM_TYPE == 3	//ANIM_LINEAR
	return(t);
	#elif ANIM_TYPE == 4	//ANIM_BOUNCE
	if(t <= 0.68584)
		return(3.099775*t*t*t);
	else
	{
		return(-(t-1)*sin(20*(t-1))+1);
		//return(anim_bounce_table[(unsigned char)((t-0.68584)*100)]);
	}
	#endif
}
void og_anim_create(t_psWidget pWidget, int16_t end_x, int16_t end_y, uint16_t duration, t_anim_end_cb anim_end_cb)
{
	t_psAnim pAnim = pFirst;;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to set a null widget");	
		return;
	}
	#endif
	while(pAnim != NULL)	//遍历动画列表检查当前控件是否在动画列表里
	{
		if(pAnim->pWidget == pWidget)	//当前控件在动画列表里
			goto anim_overwrite;				//直接重写动画参数，不用重新申请内存
		pAnim = (t_psAnim)pAnim->pNext;
	}
	pAnim = malloc(T_ANIM_SIZE);		//为动画参数申请一部分内存
	#ifdef OLED_DEBUG
	if(pAnim == NULL)								//内存分配失败，直接返回
	{
		OG_PRINTERROR("animation create fail");
		return;
	}
	#endif
	if(ucSum == 0)
	{
		pAnim->pPrev = NULL;
		pAnim->pNext = NULL;
		pFirst = pAnim;
		pLast = pAnim;
	}
	else
	{
		pAnim->pPrev = pLast;
		pAnim->pNext = NULL;
		pLast->pNext = pAnim;
		pLast = pAnim;
	}
	ucSum++;
	
	pAnim->pWidget = pWidget;
anim_overwrite:
	pAnim->start_x = pWidget->x;
	pAnim->start_y = pWidget->y;
	pAnim->distance_x = end_x - pWidget->x;
	pAnim->distance_y = end_y - pWidget->y;
	pAnim->act_time = 0;
	pAnim->duration = duration;
	pAnim->anim_end_cb = anim_end_cb;
}

void og_anim_create_offset(t_psWidget pWidget, int16_t delta_x, int16_t delta_y, uint16_t duration, t_anim_end_cb anim_end_cb)
{
	t_psAnim pAnim;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to add animation to a NULL widget");
		return;
	}
	#endif
	pAnim = pFirst;
	while(pAnim != NULL)	//遍历动画列表检查当前控件是否在动画列表里
	{
		if(pAnim->pWidget == pWidget)	//当前控件在动画列表里
			goto anim_overwrite;				//直接重写动画参数，不用重新申请内存
		pAnim = (t_psAnim)pAnim->pNext;
	}
	pAnim = malloc(T_ANIM_SIZE);		//为动画申请一部分内存
	#ifdef OLED_DEBUG
	if(pAnim == NULL)								//内存分配失败，直接返回
	{
		OG_PRINTERROR("animation create fail");
		return;
	}
	#endif
	if(ucSum == 0)
	{
		pAnim->pPrev = NULL;
		pAnim->pNext = NULL;
		pFirst = pAnim;
		pLast = pAnim;
	}
	else
	{
		pAnim->pPrev = pLast;
		pAnim->pNext = NULL;
		pLast->pNext = pAnim;
		pLast = pAnim;
	}
	ucSum++;
	
	pAnim->pWidget = pWidget;
	pAnim->distance_x = delta_x;
	pAnim->distance_y = delta_y;
	goto anim_end;
	anim_overwrite:
	pAnim->distance_x = delta_x + pAnim->distance_x - (pWidget->x - pAnim->start_x);
	pAnim->distance_y = delta_y + pAnim->distance_y - (pWidget->y - pAnim->start_y);
	anim_end:
	pAnim->start_x = pWidget->x;
	pAnim->start_y = pWidget->y;
	pAnim->act_time = 0;
	pAnim->duration = duration;
	pAnim->anim_end_cb = anim_end_cb;
}
void anim_delet(t_psAnim pAnim)
{
	if(pAnim == pFirst)
	{
		if(pAnim->pNext != NULL)
		{
			((t_psAnim)pAnim->pNext)->pPrev = NULL;
			pFirst = (t_psAnim)pAnim->pNext;
		}
		else
		{
			pFirst = NULL;
			pLast = NULL;
		}
	}
	else if(pAnim == pLast)
	{
		((t_psAnim)pAnim->pPrev)->pNext = NULL; 
		pLast = (t_psAnim)pAnim->pPrev;
	}
	else
	{
		((t_psAnim)pAnim->pPrev)->pNext = pAnim->pNext;
		((t_psAnim)pAnim->pNext)->pPrev = pAnim->pPrev;
	}
	ucSum--;
	free(pAnim);
}
void og_anim_run()
{
	t_psAnim pAnim = pFirst; //获取动画链表中的第一个元素
	
	uint16_t elaps = og_tickElaps(anim_timestamp);//计算两次更新动画之间的时间
	anim_timestamp = og_getTimestamp();
	while(pAnim != NULL)
	{
		float p;
		pAnim->act_time += elaps;
		if(pAnim->act_time > pAnim->duration)
			pAnim->act_time = pAnim->duration;
		p = animate_fx((float)pAnim->act_time / pAnim->duration);
		pAnim->pWidget->x = pAnim->start_x + pAnim->distance_x * p;
		pAnim->pWidget->y = pAnim->start_y + pAnim->distance_y * p;
		if(pAnim->act_time == pAnim->duration)
		{
			t_anim_end_cb anim_end_cb = pAnim->anim_end_cb;
			anim_delet(pAnim);
			if(pAnim->anim_end_cb != ANIM_NULL_CB)
				anim_end_cb();
		}
		pAnim = (t_psAnim)pAnim->pNext;
	}
}
