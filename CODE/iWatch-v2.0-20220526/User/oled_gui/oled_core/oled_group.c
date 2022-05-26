#define	_OLED_GROUP_C
#include "oled_group.h"
#include "stdio.h"

t_psGroup og_group_create(unsigned char i)
{
	t_psGroup pGroup = malloc(T_SGROUP_SIZE + (i - 1) * T_SGROUPITEM_SIZE);
	#ifdef OLED_DEBUG
	if(pGroup == NULL)
	{
		OG_PRINTERROR("group create fail");
		return NULL;
	}
	#endif
	pGroup->ucSum = 0;
	pGroup->focus_index = -1;
	return pGroup;
}
void og_group_addWidget(t_psGroup pGroup, t_psWidget pWidget, uint8_t focus_en)
{
	t_sGroupItem *pItem = &pGroup->item;
	#ifdef OLED_DEBUG
	if(pWidget == NULL)
	{
		OG_PRINTERROR("try to add null widget to a group");
		return;
	}
	#endif
	pItem[pGroup->ucSum].pWidget = pWidget;
	pItem[pGroup->ucSum].focus_en = focus_en;
	if(focus_en && (pGroup->focus_index == -1))
	{
		pItem[pGroup->ucSum].pWidget->config |= WIDGET_FOCUS;
		pGroup->focus_index = pGroup->ucSum;
	}
	pGroup->ucSum++;
}
void og_group_setPosOffset(t_psGroup pGroup, int16_t delta_x, int16_t delta_y)
{
	uint8_t i;
	t_sGroupItem *pItem = &pGroup->item;
	for(i = 0; i < pGroup->ucSum; i++)
	{
		og_widget_setPosOffset(pItem[i].pWidget, delta_x, delta_y);
	}
}
#if (OLED_USE_ANIM == 1)
void og_group_addAnimOffset(t_psGroup pGroup, int16_t delta_x, int16_t delta_y, uint16_t duration, t_anim_end_cb anim_end_cb)
{
	uint8_t i;
	t_sGroupItem *pItem = &pGroup->item;
	for(i = 0; i < pGroup->ucSum; i++)
	{
		og_anim_create_offset(pItem[i].pWidget, delta_x, delta_y, duration, (i==(pGroup->ucSum-1)?anim_end_cb:ANIM_NULL_CB));
	}
}
#endif
void og_group_delet(t_psGroup pGroup)
{
	uint8_t i;
	t_sGroupItem *pItem = &pGroup->item;
	for(i = 0; i < pGroup->ucSum; i++)
	{
		og_widget_delet(pItem[i].pWidget);
	}
	free(pGroup);
}
void og_group_focusWidget(t_psGroup pGroup, uint8_t direction)
{
	t_psWidget pWidget;
	t_sGroupItem *pItem = &pGroup->item;
	int16_t i;
	int16_t bias_x, bias_y;
	if(pGroup == NULL || pGroup->ucSum == 0 || pGroup->focus_index == -1)
		return;
	i = pGroup->focus_index;
	pItem[i].pWidget->config &= ~WIDGET_FOCUS;
	if(direction == 1)
	{
		while(1)
		{
			if(pItem[++i].focus_en == 1)
			{
				pItem[i].pWidget->config |= WIDGET_FOCUS;
				break;
			}
			if(i == pGroup->ucSum)
				i = -1;
		}
	}
	else
	{
		while(1)
		{
			if(pItem[--i].focus_en == 1)
			{
				pItem[i].pWidget->config |= WIDGET_FOCUS;
				break;
			}
			if(i == -1)
				i = pGroup->ucSum;
		}
	}
	pGroup->focus_index = i;
	pWidget = pItem[i].pWidget;
	if(pWidget->x < 0)
		bias_x = -pWidget->x;
	else if((pWidget->x + pWidget->w) > OLED_WIDTH)
		bias_x = -(pWidget->x + pWidget->w - OLED_WIDTH);
	else
		bias_x = 0;
	if(pWidget->y < 0)
		bias_y = -pWidget->y;
	else if((pWidget->y + pWidget->h) > OLED_HEIGHT)
		bias_y = -(pWidget->y + pWidget->h - OLED_HEIGHT);
	else
		bias_y = 0;
	if(bias_x != 0 || bias_y != 0)
	{
		#if OLED_USE_ANIM == 1
		og_group_addAnimOffset(pGroup, bias_x , bias_y, ANIM_TIME_NORM, ANIM_NULL_CB);
		#else
		og_group_setPosOffset(pGroup, bias_x , bias_y);
		#endif
	}
}
t_psWidget og_group_getFocusWidget(t_psGroup pGroup)
{
	t_sGroupItem *pItem = &pGroup->item;
	#ifdef OLED_DEBUG
	if(pGroup == NULL || pGroup->ucSum == 0 || pGroup->focus_index == -1)
	{
		OG_PRINTERROR("no widget is being focused");
		return NULL;
	}
	#endif
	return pItem[pGroup->focus_index].pWidget;
}
int16_t og_group_getFocusIndex(t_psGroup pGroup)
{
	return pGroup->focus_index;
}
void og_group_hideOffScreenWidget(t_psGroup pGroup)
{
	uint8_t i;
	t_sGroupItem *pItem = &pGroup->item;
	t_psWidget pWidget;
	if(pGroup == NULL || pGroup->ucSum == 0)
		return;
	for(i = 0; i < pGroup->ucSum; i++)
	{
		pWidget = pItem[i].pWidget;
		if((pWidget->x >= OLED_WIDTH) || (pWidget->y >= OLED_HEIGHT))
			og_widget_setShow(pWidget, 0);
		else if((pWidget->x + pWidget->w) <= 0 || (pWidget->y + pWidget->h) <= 0)
			og_widget_setShow(pWidget, 0);
	}
}