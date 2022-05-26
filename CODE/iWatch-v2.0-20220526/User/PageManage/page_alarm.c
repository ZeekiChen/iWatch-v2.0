#include "iWatch.h"

static unsigned char CODE Icon[] = {
	48, 48,
	0x00,0x80,0xE0,0x70,0x18,0x0C,0x0C,0x06,0x06,0x06,0x86,0xC6,0x6C,0x7C,0x18,0x10,
	0x00,0x00,0x00,0x00,0x10,0x30,0xF0,0xF0,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
	0x18,0x38,0x7C,0xEC,0xC6,0x86,0x06,0x06,0x06,0x0C,0x0C,0x18,0x70,0xE0,0x80,0x00,
	0x00,0x0F,0x3F,0x70,0xE0,0x70,0x38,0x1C,0x0E,0x07,0x83,0xC1,0xE0,0x70,0x38,0x18,
	0x0C,0x0C,0x0E,0x06,0x06,0x06,0xE7,0xE7,0x06,0x06,0x06,0x06,0x06,0x0C,0x0C,0x1C,
	0x18,0x30,0x70,0xE0,0xC1,0x03,0x07,0x0E,0x1C,0x38,0x70,0xE0,0x70,0x3F,0x0F,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFC,0x1E,0x07,0x01,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x03,0x07,0x3E,0xF8,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x0C,0x0C,0x18,0x18,0x38,0x30,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0F,0x1E,0x38,0x70,0xE0,0xC0,0x80,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
	0x80,0xC0,0xE0,0x70,0x38,0x1E,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x70,0x38,0x1C,0x0E,0x07,0x03,
	0x03,0x03,0x06,0x06,0x06,0x0E,0x0C,0x0C,0x0C,0x0C,0x06,0x06,0x06,0x06,0x03,0x03,
	0x03,0x07,0x0C,0x18,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	/* (48 X 48 )*/
};
static t_psWidget text1, text2, text3, text4, text5, text6;
static t_psGroup group1;
static char code ALARM_MODE[5][7] = 
{
	{"关闭\0"},
	{"单次\0"},
	{"每天\0"},
	{"工作日\0"},
	{"某一天\0"}
};
static char code ch1[] = "闹钟模式：";
static char code ch2[] = "时　分";
static char code ch3[] = "时　分　日";

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup(unsigned char condition)
{
	text1 = og_label_create(FONT_GB2312_15X16, 0, 0);
	og_label_setText(text1, ch1);
	text2 = og_label_create(FONT_GB2312_15X16, 80, 0);
	og_label_setText(text2, ALARM_MODE[config.alarm.mode]);
	og_widget_setPos(text2, OLED_WIDTH - text2->w, 0);		//右对齐
	text3 = og_label_create(FONT_GB2312_15X16, 48, 20);
	og_label_setText(text3, ch2);
	text4 = og_label_create(FONT_ASCII_8X16, 32, 20);
	text5 = og_label_create(FONT_ASCII_8X16, 64, 20);
	text6 = og_label_create(FONT_ASCII_8X16, 80, 20);
	if(config.alarm.mode != ALARM_SPECIFIC_DAY)
	{
		og_widget_setShow(text6, 0);
	}
	else
	{
		og_widget_setPos(text3, 32, 20);
		og_widget_setPos(text4, 16, 20);
		og_widget_setPos(text5, 48, 20);
		og_widget_setPos(text6, 80, 20);
	}
	group1 = og_group_create(6);
	og_group_addWidget(group1, text1, 0);
	og_group_addWidget(group1, text2, 1);
	og_group_addWidget(group1, text3, 0);
	og_group_addWidget(group1, text4, 1);
	og_group_addWidget(group1, text5, 1);
	og_group_addWidget(group1, text6, 1);
	og_group_setPosOffset(group1, 0, 64);
	og_group_addAnimOffset(group1, 0, -64, ANIM_TIME_NORM, ANIM_NULL_CB);
}
static void delet(void)
{
	og_group_delet(group1);
	pageSetStatus(page_alarm, PAGE_IDLE);
}
/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit(unsigned char condition)
{
	RX8130CESetAlarm(&config.alarm);
	iWatchSaveConfig(&config);
	og_group_addAnimOffset(group1, 0, 64, ANIM_TIME_NORM, delet);
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
		og_label_setText(text2, ALARM_MODE[config.alarm.mode]);
		og_widget_setPos(text2, 128 - text2->w, 0);
		lablel_printf(text4, "%02d", config.alarm.hour);
		lablel_printf(text5, "%02d", config.alarm.minute);
		lablel_printf(text6, "%2d", config.alarm.day);
	}
}
void showText6()
{
	og_widget_setShow(text6, 1);
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
		pageShift(page_menu, PAGE_RETURN);
	else if(event == KEY3_PRESSED)
	{
		og_group_focusWidget(group1, FOCUS_NEXT);
		if(og_group_getFocusIndex(group1) == 5)
		{
			if(config.alarm.mode!=ALARM_SPECIFIC_DAY)
				og_group_focusWidget(group1, FOCUS_NEXT);
		}
	}
	else if(event == KEY2_PRESSED)
	{
		unsigned char i = og_group_getFocusIndex(group1);
		if(i == 1)
		{
			if(config.alarm.mode == ALARM_SPECIFIC_DAY)
			{
				og_label_setText(text3, ch2);
				og_anim_create(text3, 48, 20, ANIM_TIME_NORM, ANIM_NULL_CB);
				og_anim_create(text4, 32, 20, ANIM_TIME_NORM, ANIM_NULL_CB);
				og_anim_create(text5, 64, 20, ANIM_TIME_NORM, ANIM_NULL_CB);
				og_widget_setShow(text6, 0);
			}
			if(++config.alarm.mode > ALARM_SPECIFIC_DAY)
				config.alarm.mode = ALARM_DISABLE;
			if(config.alarm.mode == ALARM_SPECIFIC_DAY)
			{
				og_label_setText(text3, ch3);
				og_anim_create(text3, 32, 20, ANIM_TIME_NORM, ANIM_NULL_CB);
				og_anim_create(text4, 16, 20, ANIM_TIME_NORM, ANIM_NULL_CB);
				og_anim_create(text5, 48, 20, ANIM_TIME_NORM, showText6);
			}
		}
		else if(i == 3)
		{
			if(++config.alarm.hour == 24)
				config.alarm.hour = 0;
		}
		else if(i == 4)
		{
			if(++config.alarm.minute == 60)
				config.alarm.minute = 0;
		}
		else if(i == 5)
		{
			if(++config.alarm.day == 32)
				config.alarm.day = 1;
		}
	}
	else if(event == KEY4_PRESSED)
	{
		unsigned char i = og_group_getFocusIndex(group1);
		if(i == 1)
		{
			if(config.alarm.mode == ALARM_SPECIFIC_DAY)
			{
				og_label_setText(text3, ch2);
				og_anim_create(text3, 48, 20, ANIM_TIME_NORM, ANIM_NULL_CB);
				og_anim_create(text4, 32, 20, ANIM_TIME_NORM, ANIM_NULL_CB);
				og_anim_create(text5, 64, 20, ANIM_TIME_NORM, ANIM_NULL_CB);
				og_widget_setShow(text6, 0);
			}
			if(--config.alarm.mode == 255)
				config.alarm.mode = ALARM_SPECIFIC_DAY;
			if(config.alarm.mode == ALARM_SPECIFIC_DAY)
			{
				og_label_setText(text3, ch3);
				og_anim_create(text3, 32, 20, ANIM_TIME_NORM, ANIM_NULL_CB);
				og_anim_create(text4, 16, 20, ANIM_TIME_NORM, ANIM_NULL_CB);
				og_anim_create(text5, 48, 20, ANIM_TIME_NORM, showText6);
			}
		}
		else if(i == 3)
		{
			if(--config.alarm.hour == 255)
				config.alarm.hour = 23;
		}
		else if(i == 4)
		{
			if(--config.alarm.minute == 255)
				config.alarm.minute = 59;
		}
		else if(i == 5)
		{
			if(--config.alarm.day == 0)
				config.alarm.day = 31;
		}
	}
}
void pageRegister_page_alarm(unsigned char pageID)
{
	pageRegister(pageID, "ALARM", Icon, Setup, Loop, Exit, Event);
}