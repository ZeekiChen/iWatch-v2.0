#include "iWatch.h"

static t_psWidget text1, text2, text3, text4;
static t_psWidget text_time[7];
static t_psGroup group1;

static t_sRX8130CE xdata set_time;
static unsigned char CODE WEEKDAY[7][8] = {
	{"星期一\0"},
	{"星期二\0"},
	{0xd0, 0xc7, 0xc6, 0xda, 0xc8, 0xfd, '\0'},  //为什么这里要这样？因为不知道什么原因，编译器无法正确转译中文“三”到GB2312编码
	{"星期四\0"},
	{"星期五\0"},
	{"星期六\0"},
	{"星期日\0"}
};

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup(unsigned char condition)
{
	unsigned char xdata i;
	RX8130CEReadTime(&set_time);
	group1 = og_group_create(11);
	text_time[0] = og_label_create(FONT_ASCII_8X16, 0, 16);
	text_time[1] = og_label_create(FONT_ASCII_8X16, 32, 16);
	text_time[2] = og_label_create(FONT_ASCII_8X16, 64, 16);
	text_time[3] = og_label_create(FONT_ASCII_8X16, 0, 48);
	text_time[4] = og_label_create(FONT_ASCII_8X16, 48, 48);
	text_time[5] = og_label_create(FONT_ASCII_8X16, 80, 48);
	text_time[6] = og_label_create(FONT_GB2312_15X16, 0, 64);
	for(i = 0; i < 7; i++)
	{
		og_group_addWidget(group1, text_time[i], 1);
	}
	text1 = og_label_create(FONT_GB2312_15X16, 0, 0);
	text2 = og_label_create(FONT_GB2312_15X16, 16, 16);
	text3 = og_label_create(FONT_GB2312_15X16, 0, 32);
	text4 = og_label_create(FONT_GB2312_15X16, 32, 48);
	og_label_setText(text1, "时间：");
	og_label_setText(text2, "时　分　秒");
	og_label_setText(text3, "日期：");
	og_label_setText(text4, "年　月　日");
	og_group_addWidget(group1, text1, 0);
	og_group_addWidget(group1, text2, 0);
	og_group_addWidget(group1, text3, 0);
	og_group_addWidget(group1, text4, 0);
	og_group_setPosOffset(group1, 128, 0);
	og_group_addAnimOffset(group1, -128, 0, ANIM_TIME_NORM, ANIM_NULL_CB);
}
static void delet(void)
{
	og_group_delet(group1);
	pageSetStatus(page_settime, PAGE_IDLE);
}
/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit(unsigned char condition)
{
	RX8130CEWriteTime(&set_time);
	og_group_addAnimOffset(group1, 128, 0, ANIM_TIME_NORM, delet);
}
/**
  * @brief  页面循环执行的内容
  * @param  无
  * @retval 无
  */
static void Loop()
{
	if(pageExecuteRate(&Rate20Hz))
	{
		lablel_printf(text_time[0], "%02d", set_time.hour);
		lablel_printf(text_time[1], "%02d", set_time.minute);
		lablel_printf(text_time[2], "%02d", set_time.second);
		lablel_printf(text_time[3], "%02d", set_time.year + 2000);
		lablel_printf(text_time[4], "%02d", set_time.month);
		lablel_printf(text_time[5], "%02d", set_time.day);
		og_label_setText(text_time[6], WEEKDAY[set_time.weekday - 1]);
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
		pageShift(page_setting, PAGE_RETURN);
	if(event == KEY2_PRESSED)//++
	{
		switch(og_group_getFocusIndex(group1))
		{
			case 0:
				if(++set_time.hour == 24)
					set_time.hour = 0;
				break;
			case 1:
				if(++set_time.minute == 60)
					set_time.minute = 0;
				break;
			case 2:
				if(++set_time.second == 60)
					set_time.second = 0;
				break;
			case 3:
				if(++set_time.year == 100)
					set_time.year = 0;
				break;
			case 4:
				if(++set_time.month == 13)
					set_time.month = 1;
				break;
			case 5:
				if(++set_time.day == 32)
					set_time.day = 1;
				break;
			case 6:
				if(++set_time.weekday == 8)
					set_time.weekday = 1;
				break;
		}
	}
	if(event == KEY4_PRESSED)//--
	{
		switch(og_group_getFocusIndex(group1))
		{
			case 0:
				if(--set_time.hour == 255) //向下溢出了
					set_time.hour = 23;
				break;
			case 1:
				if(--set_time.minute == 255)
					set_time.minute = 59;
				break;
			case 2:
				if(--set_time.second == 255)
					set_time.second = 59;
				break;
			case 3:
				if(--set_time.year == 255)
					set_time.year = 0;
				break;
			case 4:
				if(--set_time.month == 0)
					set_time.month = 12;
				break;
			case 5:
				if(--set_time.day == 0)
					set_time.day = 31;
				break;
			case 6:
				if(--set_time.weekday == 0)
					set_time.weekday = 7;
				break;
		}
	}
	if(event == KEY3_PRESSED)
	{
		og_group_focusWidget(group1, FOCUS_NEXT);
	}
}
void pageRegister_page_settime(unsigned char pageID)
{
	pageRegister(pageID, NULL, NULL, Setup, Loop, Exit, Event);
}