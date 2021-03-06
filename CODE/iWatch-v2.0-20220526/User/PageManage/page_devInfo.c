#include "iWatch.h"

static unsigned char CODE Icon[] = {
	48, 48,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF0,0x78,0x3C,0x3C,0x1C,
	0x1E,0x0E,0x0E,0x0E,0x0F,0x0F,0x07,0x07,0x07,0x07,0x0F,0x0F,0x0E,0x0E,0x0E,0x1E,
	0x1C,0x3C,0x3C,0x78,0xF0,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xE0,0xF0,0xFC,0x7E,0x1F,0x0F,0x07,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0xFC,0xFC,0xF8,0xF8,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0F,0x1F,0x7E,0xFC,0xF8,0xE0,0x00,0x00,
	0xF0,0xFF,0xFF,0x3F,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xC0,0xE1,0xE1,0xE1,0xE1,0xC0,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x1F,0xFF,0xFF,0xF8,
	0x0F,0xFF,0xFF,0xFC,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xF8,0xFF,0xFF,0x1F,
	0x00,0x00,0x07,0x0F,0x3F,0x7E,0xF8,0xF0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x3F,0x3F,0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xF0,0xF8,0x7E,0x3F,0x1F,0x07,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x0F,0x1E,0x3C,0x3C,0x38,
	0x78,0x70,0x70,0x70,0xF0,0xF0,0xE0,0xE0,0xE0,0xE0,0xF0,0xF0,0x70,0x70,0x70,0x78,
	0x38,0x3C,0x3C,0x1E,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	/* (48 X 48 )*/
};

char *code CH[] = {
	"设备型号：",
	"固件版本：",
	"编译时间：",
	"电池电压：",
	"上次充电："
};
char *code EN[] = {
	"iWatch-v2.0",
	"v2.0.0",
	__DATE__,
	NULL,
	NULL
};
#define	EN_TEXT_SUM 5

static t_psWidget ch_text[EN_TEXT_SUM];
static t_psWidget en_text[EN_TEXT_SUM];
static t_psGroup group1;
static uint8_t page = 0;

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup(unsigned char condition)
{
	uint8_t i;
	group1 = og_group_create(EN_TEXT_SUM * 2);
	for(i = 0; i < EN_TEXT_SUM; i++)
	{
		ch_text[i] = og_label_create(FONT_GB2312_15X16, 0, i * 32);
		og_label_setText(ch_text[i], CH[i]);
		en_text[i] = og_label_create(FONT_ASCII_8X16, 0, 16 + i * 32);
		og_label_setText(en_text[i], EN[i]);
		og_widget_setAlignment(en_text[i], ALIGN_HORIZONMID);
		og_group_addWidget(group1, ch_text[i], 0);
		og_group_addWidget(group1, en_text[i], 0);
	}
	og_group_setPosOffset(group1, 0, OLED_HEIGHT);
	og_group_addAnimOffset(group1, 0, -OLED_HEIGHT, ANIM_TIME_NORM, ANIM_NULL_CB);
	page = 0;
}
static void delet(void)
{
	og_group_delet(group1);
	pageSetStatus(page_devinfo, PAGE_IDLE);
}
/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit(unsigned char condition)
{
	og_group_hideOffScreenWidget(group1);
	og_group_addAnimOffset(group1, 0, OLED_HEIGHT, ANIM_TIME_NORM, delet);
}
/**
  * @brief  页面循环执行的内容
  * @param  无
  * @retval 无
  */
static void Loop()
{
	if(pageExecuteRate(&Rate5Hz))
	{
		t_sRX8130CE *t = &config.last_charge_time;
		lablel_printf(en_text[4], "%02d/%02d %02d:%02d", (int)t->month, (int)t->day, (int)t->hour, (int)t->minute);
		og_widget_setAlignment(en_text[4], ALIGN_HORIZONMID);
		lablel_printf(en_text[3], "%.2fv", GetBatVoltage() + 0.005);
		og_widget_setAlignment(en_text[3], ALIGN_HORIZONMID);
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
		pageShift(page_menu, PAGE_RETURN);
	else if(event == KEY2_PRESSED)
	{
		if(page != 0)
		{
			og_group_addAnimOffset(group1, 0, 64, ANIM_TIME_NORM, ANIM_NULL_CB);
			page--;
		}
	}
	else if(event == KEY4_PRESSED)
	{
		if(page != 2)
		{
			og_group_addAnimOffset(group1, 0, -64, ANIM_TIME_NORM, ANIM_NULL_CB);
			page++;
		}
	}
}
void pageRegister_page_devinfo(unsigned char pageID)
{
	pageRegister(pageID, "Device Info", Icon, Setup, Loop, Exit, Event);
}