#include "iWatch.h"

static unsigned char CODE Icon[] = {
	48, 48,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xC0,0x80,0x00,0x00,
	0x00,0x00,0x00,0xE0,0xFC,0xFC,0x1C,0x0C,0x0C,0x1C,0xFC,0xFC,0xC0,0x00,0x00,0x00,
	0x00,0x00,0x80,0xC0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x1E,0x7F,0xF7,0xE3,0x81,0x03,0x03,0x07,0x0E,
	0x0E,0x07,0x07,0x03,0x83,0x81,0x80,0x80,0x80,0x80,0x81,0x83,0x03,0x07,0x07,0x0E,
	0x0E,0x07,0x03,0x03,0x81,0xE3,0xF7,0x7F,0x1E,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xF0,0xF0,0x70,0x38,0x38,0x38,0x3E,0x3F,0x0F,0x01,0x00,0x00,0x00,0xF0,
	0xFC,0x7E,0x0F,0x07,0xC3,0xE3,0xF1,0x71,0x71,0xF1,0xE3,0xC3,0x07,0x0F,0x7E,0xFC,
	0xF0,0x00,0x00,0x00,0x01,0x0F,0x3F,0x3E,0x38,0x38,0x38,0x70,0xF0,0xF0,0x00,0x00,
	0x00,0x00,0x0F,0x0F,0x0E,0x0C,0x0C,0x1C,0x7C,0xFC,0xF0,0xC0,0x00,0x00,0x00,0x07,
	0x1F,0x3E,0x78,0xF0,0xE1,0xC7,0xC7,0xCE,0xCE,0xC7,0xC7,0xE1,0xE0,0x78,0x3E,0x1F,
	0x07,0x00,0x00,0x00,0xC0,0xF0,0xFC,0x7C,0x1C,0x0C,0x0C,0x0E,0x0F,0x0F,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x7C,0xFE,0xEF,0xC7,0x81,0xC0,0xE0,0xE0,0x70,
	0x70,0xF0,0xE0,0xE0,0xC0,0xC1,0x01,0x01,0x01,0x01,0xC1,0xC0,0xE0,0xE0,0xF0,0x70,
	0x70,0xE0,0xE0,0xC0,0x81,0xC7,0xEF,0xFE,0x7C,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x01,0x01,0x00,0x00,
	0x00,0x00,0x00,0x03,0x3F,0x3F,0x38,0x30,0x30,0x38,0x3F,0x3F,0x03,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	/* (48 X 48 )*/
};

static t_psWidget text[7];
static t_psWidget slider1, slider2;
static t_psWidget sw1, sw2, sw3, sw4;
static t_psGroup group1;
static unsigned char xdata nfocus = 0;

char * code item[7] = 
{
	{"设置时间\0"},
	{"息屏时间\0"},
	{"屏幕亮度\0"},
	{"颜色反转\0"},
	{"抬腕唤醒\0"},
	{"计步器\0"},
	{"按键音\0"},
};

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup(unsigned char condition)
{
	unsigned char i;
	iWatchReadConfig(&config);
	group1 = og_group_create(14);
	for(i = 0; i < 7; i++)
	{
		text[i] = og_label_create(FONT_GB2312_15X16, 0, i * 16);
		og_label_setText(text[i], item[i]);
		og_group_addWidget(group1, text[i], 1);
	}
	slider1 = og_slider_create(0, 30, config.t_inactive_max, 58, 70, 20);
	slider2 = og_slider_create(1, 5, config.screen_brightness, 58, 70, 36);
	sw1 = og_switch_create(16, config.screen_inverse, 104, 48);
	sw2 = og_switch_create(16, (config.lsm6dsm_cfg & 0x01)?1:0, 104, 64);
	sw3 = og_switch_create(16, (config.lsm6dsm_cfg & 0x02)?1:0, 104, 80);
	sw4 = og_switch_create(16, config.key_sound, 104, 96);
	og_group_addWidget(group1, slider1, 0);
	og_group_addWidget(group1, slider2, 0);
	og_group_addWidget(group1, sw1, 0);
	og_group_addWidget(group1, sw2, 0);
	og_group_addWidget(group1, sw3, 0);
	og_group_addWidget(group1, sw4, 0);
	og_group_setPosOffset(group1, condition==PAGE_ENTER?0:-128, condition==PAGE_ENTER?64:0);
	og_group_addAnimOffset(group1, condition==PAGE_ENTER?0:128, condition==PAGE_ENTER?-64:0, ANIM_TIME_NORM, ANIM_NULL_CB);
	nfocus = 0;
}
static void delet(void)
{
	og_group_delet(group1);
	pageSetStatus(page_setting, PAGE_IDLE);
}
/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit(unsigned char condition)
{
	iWatchSaveConfig(&config);
	og_group_hideOffScreenWidget(group1);
	og_group_addAnimOffset(group1, condition==PAGE_ENTER?-128:0, condition==PAGE_ENTER?0:64, ANIM_TIME_NORM, delet);
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
		if(nfocus != 0)
		{
			t_psWidget pWidget = og_group_getFocusWidget(group1);
			pWidget->config |= WIDGET_FOCUS;
			if(nfocus == 1)
				slider1->config &= ~WIDGET_FOCUS;
			else
				slider2->config &= ~WIDGET_FOCUS;
			nfocus = 0;
		}
		else
			pageShift(page_menu, PAGE_RETURN);
	}
	if(event == KEY3_PRESSED)	
	{
		unsigned char i = og_group_getFocusIndex(group1);
		if(i == 0)
			pageShift(page_settime, PAGE_ENTER);
		else if(i == 1)
		{
			t_psWidget pWidget = og_group_getFocusWidget(group1);
			if(nfocus == 0)
			{
				nfocus = 1;
				pWidget->config &= ~WIDGET_FOCUS;
				slider1->config |= WIDGET_FOCUS;
			}
			else
			{
				nfocus = 0;
				pWidget->config |= WIDGET_FOCUS;
				slider1->config &= ~WIDGET_FOCUS;
			}
		}
		else if(i == 2)
		{
			t_psWidget pWidget = og_group_getFocusWidget(group1);
			if(nfocus == 0)
			{
				nfocus = 2;
				pWidget->config &= ~WIDGET_FOCUS;
				slider2->config |= WIDGET_FOCUS;
			}
			else
			{
				nfocus = 0;
				pWidget->config |= WIDGET_FOCUS;
				slider2->config &= ~WIDGET_FOCUS;
			}
		}
		else if(i == 3)
		{
			if(config.screen_inverse)
				config.screen_inverse = 0;
			else
				config.screen_inverse = 1;
			og_switch_setStatus(sw1, config.screen_inverse);
			OLED_Inverse(config.screen_inverse);
		}
		else if(i == 4)
		{
			if((config.lsm6dsm_cfg & 0x01) != 0)
			{
				config.lsm6dsm_cfg &= ~0x01;
				LSM6DSMDisableAWT();
				og_switch_setStatus(sw2, 0);
			}
			else
			{				
				config.lsm6dsm_cfg |= 0x01;
				LSM6DSMEnableAWT(10, 100);				//10 degree, 100ms
				og_switch_setStatus(sw2, 1);
			}
		}
		else if(i == 5)
		{
			if((config.lsm6dsm_cfg & 0x02) != 0)
			{
				config.lsm6dsm_cfg &= ~0x02;
				LSM6DSMDisablePedometer();
				og_switch_setStatus(sw3, 0);
			}
			else
			{
				config.lsm6dsm_cfg |= 0x02;
				LSM6DSMEnablePedometer(1040, 6);				//debounce time = 1040ms, debounce step = 6 steps
				og_switch_setStatus(sw3, 1);
			}
			
		}
		else if(i == 6)
		{
			if(config.key_sound)
			{
				config.key_sound = 0;
			}
			else
			{
				config.key_sound = 1;
				Bee();
			}
			og_switch_setStatus(sw4, config.key_sound);
		}
		/*
		else if(i == 7)
		{
			pageShift(page_devinfo, PAGE_ENTER);
		}
		*/
	}
	if(event == KEY2_PRESSED)
	{
		if(nfocus == 0)
		{
			og_group_focusWidget(group1, FOCUS_PREV);
		}
		else if(nfocus == 1)
		{
			if(++config.t_inactive_max > 30)
				config.t_inactive_max = 30;
			og_slider_setValue(slider1, config.t_inactive_max);
		}
		else if(nfocus == 2)
		{
			if(++config.screen_brightness == 6)
				config.screen_brightness = 5;
			og_slider_setValue(slider2, config.screen_brightness);
			OLED_Set_Brightness((config.screen_brightness - 1) * 50 + 1);
		}
	}
	if(event == KEY4_PRESSED)
	{
		if(nfocus == 0)
		{
			og_group_focusWidget(group1, FOCUS_NEXT);
		}
		else if(nfocus == 1)
		{
			if(--config.t_inactive_max < 3)
				config.t_inactive_max = 3;
			og_slider_setValue(slider1, config.t_inactive_max);
		}
		else if(nfocus == 2)
		{
			if(--config.screen_brightness == 0)
				config.screen_brightness = 1;
			og_slider_setValue(slider2, config.screen_brightness);
			OLED_Set_Brightness((config.screen_brightness - 1) * 50 + 1);
		}
	}
}
void pageRegister_page_setting(unsigned char pageID)
{
	pageRegister(pageID, "SETTING", Icon, Setup, Loop, Exit, Event);
}