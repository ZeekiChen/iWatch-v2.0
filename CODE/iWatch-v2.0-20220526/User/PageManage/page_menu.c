#include "iWatch.h"

extern t_sPageList pagelist[PAGE_MAX];
#define ICON_SUM (PAGE_MENU_MAX - PAGE_MENU_MIN - 1)
#define	ICON_GAP	72
static unsigned char page_index = PAGE_MENU_MIN + 1;
static t_psWidget text1, bmp1, title;
static t_psWidget icon[ICON_SUM];
static t_psGroup group1, group2;

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup(unsigned char condition)
{
	unsigned char i;
	group2 = og_group_create(ICON_SUM);
	//页面图标
	for(i = 0; i < ICON_SUM; i++)
	{
		icon[i] = og_bmp_create((t_psBMP)pagelist[i + PAGE_MENU_MIN + 1].Icon, i * ICON_GAP + 40, 8);
		og_group_addWidget(group2, icon[i], 0);
	}
	og_group_setPosOffset(group2, 0, condition == PAGE_ENTER?64:-64);
	og_group_addAnimOffset(group2, -ICON_GAP * (page_index - PAGE_MENU_MIN - 1), condition == PAGE_ENTER?-64:64, ANIM_TIME_NORM, ANIM_NULL_CB);
	//页面标题
	title = og_label_create(FONT_ASCII_5X7, 0, 56);
	og_label_setText(title, pagelist[page_index].Title);
	//令页面标题居中
	og_widget_setPos(title, 64 - title->w/2, title->y);
	//显示时间
	text1 = og_label_create(FONT_ASCII_5X7, 0, 0);
	lablel_printf(text1, "%02d:%02d", (int)time.hour, (int)time.minute)
	//显示电池电量图标
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
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit(unsigned char condition)
{
	og_group_addAnimOffset(group1, 0, condition == PAGE_ENTER?-64:64, ANIM_TIME_NORM, ANIM_NULL_CB);
	og_group_addAnimOffset(group2, ICON_GAP * (page_index - PAGE_MENU_MAX + 1), condition == PAGE_ENTER?-64:64, ANIM_TIME_NORM, delet);
}
/**
  * @brief  页面循环执行的内容
  * @param  无
  * @retval 无
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
  * @brief  页面事件
  * @param  btn:发出事件的按键
  * @param  event:事件编号
  * @retval 无
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