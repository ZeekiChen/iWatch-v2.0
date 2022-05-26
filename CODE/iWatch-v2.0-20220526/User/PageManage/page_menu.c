#include "iWatch.h"

extern t_sPageList pagelist[PAGE_MAX];
#define ICON_SUM (PAGE_MENU_MAX - PAGE_MENU_MIN - 1)
#define	ICON_GAP	72
static unsigned char page_index = PAGE_MENU_MIN + 1;
static t_psWidget text1, bmp1, title;
static t_psWidget icon[ICON_SUM];
static t_psGroup group1, group2;

/**
  * @brief  ҳ���ʼ���¼�
  * @param  ��
  * @retval ��
  */
static void Setup(unsigned char condition)
{
	unsigned char i;
	group2 = og_group_create(ICON_SUM);
	//ҳ��ͼ��
	for(i = 0; i < ICON_SUM; i++)
	{
		icon[i] = og_bmp_create((t_psBMP)pagelist[i + PAGE_MENU_MIN + 1].Icon, i * ICON_GAP + 40, 8);
		og_group_addWidget(group2, icon[i], 0);
	}
	og_group_setPosOffset(group2, 0, condition == PAGE_ENTER?64:-64);
	og_group_addAnimOffset(group2, -ICON_GAP * (page_index - PAGE_MENU_MIN - 1), condition == PAGE_ENTER?-64:64, ANIM_TIME_NORM, ANIM_NULL_CB);
	//ҳ�����
	title = og_label_create(FONT_ASCII_5X7, 0, 56);
	og_label_setText(title, pagelist[page_index].Title);
	//��ҳ��������
	og_widget_setPos(title, 64 - title->w/2, title->y);
	//��ʾʱ��
	text1 = og_label_create(FONT_ASCII_5X7, 0, 0);
	lablel_printf(text1, "%02d:%02d", (int)time.hour, (int)time.minute)
	//��ʾ��ص���ͼ��
	bmp1 = og_bmp_create((t_psBMP)battery, 104, 0);
	
	group1 = og_group_create(3);
	og_group_addWidget(group1, title, 0);
	og_group_addWidget(group1, text1, 0);
	og_group_addWidget(group1, bmp1, 0);
	og_group_setPosOffset(group1, 0, condition == PAGE_ENTER?64:-64);
	og_group_addAnimOffset(group1, 0, condition == PAGE_ENTER?-64:64, ANIM_TIME_NORM, ANIM_NULL_CB);
}
static void delet(void)
{
	og_group_delet(group1);
	og_group_delet(group2);
	pageSetStatus(page_menu, PAGE_IDLE);
}
/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit(unsigned char condition)
{
	og_group_addAnimOffset(group1, 0, condition == PAGE_ENTER?-64:64, ANIM_TIME_NORM, ANIM_NULL_CB);
	og_group_addAnimOffset(group2, ICON_GAP * (page_index - PAGE_MENU_MAX + 1), condition == PAGE_ENTER?-64:64, ANIM_TIME_NORM, delet);
}
/**
  * @brief  ҳ��ѭ��ִ�е�����
  * @param  ��
  * @retval ��
  */
static void Loop()
{
	if(pageExecuteRate(&Rate10Hz))
	{
		RX8130CEReadTime(&time);
		battery_life = iWatchGetBatteryLife();
		lablel_printf(text1, "%02d:%02d", (int)time.hour, (int)time.minute);
		battery_life = iWatchGetBatteryLife();
		og_bmp_setBmp(bmp1, (t_psBMP)DrawBatteryIcon(battery_life));
	}
}
/**
  * @brief  ҳ���¼�
  * @param  btn:�����¼��İ���
  * @param  event:�¼����
  * @retval ��
  */
static void Event(unsigned char event)
{
	if(event == KEY1_PRESSED || event == AWT)
	{
		pageShift(page_watch, PAGE_RETURN);
	}
	else if(event == KEY3_PRESSED)
	{
		pageShift(page_index, PAGE_ENTER);
	}
	else 
	{
		unsigned char i = 0;
		int icon_move;
		if(event == KEY2_PRESSED)
		{
			if(++page_index == PAGE_MENU_MAX)
			{
				page_index = PAGE_MENU_MIN + 1;
				icon_move = ICON_GAP * (ICON_SUM - 1);
			}
			else
				icon_move = -ICON_GAP;
		}
		else if(event == KEY4_PRESSED)
		{
			if(--page_index == PAGE_MENU_MIN)
			{
				page_index = PAGE_MENU_MAX - 1;
				icon_move = -ICON_GAP * (ICON_SUM - 1);
			}
			else
				icon_move = ICON_GAP;
		}
		og_group_addAnimOffset(group2, icon_move, 0, ANIM_TIME_FAST, ANIM_NULL_CB);
		lablel_printf(title, "%s", pagelist[page_index].Title);
		og_widget_setPos(title, 64 - title->w/2, 56);
	}
}
void pageRegister_page_menu(unsigned char pageID)
{
	pageRegister(pageID, NULL, NULL, Setup, Loop, Exit, Event);
}