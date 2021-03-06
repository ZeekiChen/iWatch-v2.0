#include "iWatch.h"

static unsigned char CODE Icon[] = {
	48, 48,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x30,0x18,0x18,0x0C,0x0C,
	0x0C,0x06,0x06,0x06,0x06,0x06,0x03,0xF3,0xF3,0x03,0x06,0x06,0x06,0x06,0x06,0x0C,
	0x0C,0x1C,0x18,0x38,0x30,0x70,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xC0,0xF0,0x3C,0x0E,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x80,0x80,0xC0,0xC0,
	0xE0,0xE0,0xF0,0x30,0x00,0x00,0x00,0x01,0x03,0x07,0x1E,0x7C,0xF0,0xC0,0x00,0x00,
	0xF0,0xFF,0x1F,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x80,0xC0,0xF0,0x70,0x78,0xFC,0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,0x7F,0x3F,
	0x0F,0x03,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x01,0x3F,0xFE,0xC0,
	0x0F,0xFF,0xF8,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0xC0,0xF0,
	0x7C,0x1E,0x8F,0xC3,0xC0,0x60,0x70,0x30,0x39,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x80,0xFC,0x7F,0x0F,
	0x00,0x00,0x03,0x0F,0x3C,0x70,0xE0,0xC0,0x80,0x00,0x00,0x08,0x0C,0x0F,0x07,0x07,
	0x03,0x03,0x01,0x01,0x00,0x00,0x00,0xE0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0x78,0x3E,0x0F,0x03,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0E,0x0C,0x18,0x18,0x30,0x30,
	0x70,0x60,0x60,0x60,0x60,0x60,0xC0,0xCF,0xCF,0xC0,0x60,0x60,0x60,0x60,0x60,0x30,
	0x30,0x30,0x18,0x1C,0x0C,0x0E,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	/* (48 X 48 )*/
};
static t_psWidget text1;
static t_psWidget bmp1;
static t_psWidget canvas1;
static t_psWidget pp[8];
static t_psGroup group1;

static char *code CH[8] = {
	"北",
	"东",
	"南",
	"西",
	"东北",
	"东南",
	"西南",
	"西北"
};
static unsigned char CODE	POINTER[] = {
	9, 32,
	0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x07,0xF8,0x00,
	0x00,0x00,0x00,0x00,0xF8,0x07,0x00,0x07,0xFC,0x00,0x00,0x00,0xF8,0x07,0x00,0x00,
	0x00,0x07,0xF8,0x00
	/* (9 X 32 )*/
};

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup(unsigned char condition)
{
	unsigned char i;
	group1 = og_group_create(11);
	for(i = 0; i < 8; i++)
	{
		pp[i] = og_label_create(FONT_GB2312_15X16, 128, 64);
		og_label_setText(pp[i], CH[i]);
		og_group_addWidget(group1, pp[i], 0);
	}
	text1 = og_label_create(FONT_ASCII_8X16, 64, 0);
	bmp1 = og_bmp_create((t_psBMP)POINTER, 60, 32);
	canvas1 = og_canvas_create(128, 32, 0, 16);
	og_group_addWidget(group1, text1, 0);
	og_group_addWidget(group1, bmp1, 0);
	og_group_addWidget(group1, canvas1, 0);
	og_group_setPosOffset(group1, 0, 64);
	og_group_addAnimOffset(group1, 0, -64, ANIM_TIME_NORM, ANIM_NULL_CB);
	#if (MAG_SENSOR_SELECT == 1)
	QMC5883LSetMode(1);
	#endif
	LSM6DSMConfigAcc(ACC_ODR_208_HZ, ACC_SCALE_4_G);
	LSM6DSMConfigGyr(GYR_ODR_208_HZ, GYR_SCALE_500_DPS);
}
static void delet(void)
{
	og_group_delet(group1);
	pageSetStatus(page_compass, PAGE_IDLE);
}
/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit(unsigned char condition)
{
	#if (MAG_SENSOR_SELECT == 1)
	QMC5883LSetMode(0);
	#endif
	if(config.lsm6dsm_cfg == 0)
		LSM6DSMConfigAcc(ACC_POWER_DOWN, ACC_SCALE_4_G);
	else
		LSM6DSMConfigAcc(ACC_ODR_26_HZ, ACC_SCALE_4_G);
	LSM6DSMConfigGyr(GYR_POWER_DOWN, GYR_SCALE_500_DPS);
	og_group_addAnimOffset(group1, 0, 64, ANIM_TIME_NORM, delet);
}
/**
  * @brief  页面循环执行的内容
  * @param  无
  * @retval 无
  */
static void Loop()
{
	if(pageExecuteRate(&Rate100Hz))
	{
		LSM6DSMReadGYRAndACC(&LSM6DSM);
		IMUupdate(&LSM6DSM);
	}
	if(pageExecuteRate(&Rate50Hz))
	{
		float temp;
		static float last_temp = 0;
		int angle_from_north;
		int cosA, sinA;
		unsigned char i, j;
		#if (MAG_SENSOR_SELECT == 1)
		QMC5883LRead(mag);
		temp = QMC5883LCalYaw(mag, LSM6DSM.AngleX, LSM6DSM.AngleY);
		#endif
		if((temp > 360.0) || (temp < 0.0))
			return;
		if((temp - last_temp < -250.0) || (temp - last_temp > 250.0)) 
			last_temp = temp;
		else
			last_temp = temp * 0.4 + last_temp * 0.6;
		angle_from_north = last_temp + 0.5;
		angle_from_north -= 90;
		if(angle_from_north < 0)
			angle_from_north += 360;
		#if (MAG_SENSOR_SELECT == 0)
		og_label_setText(text1, NULL);
		#elif (MAG_SENSOR_SELECT == 1)
		lablel_printf(text1, "%d", angle_from_north);
		#endif
		og_widget_setAlignment(text1, ALIGN_HORIZONMID);
		cosA = cos(angle_from_north*0.017453292)*72;
		sinA = sin(angle_from_north*0.017453292)*72;
		og_widget_setPos(pp[0], 64 - sinA - 8, 96 - cosA - 8);
		og_widget_setPos(pp[1], 64 + cosA - 8, 96 - sinA - 8);
		og_widget_setPos(pp[2], 64 + sinA - 8, 96 + cosA - 8);
		og_widget_setPos(pp[3], 64 - cosA - 8, 96 + sinA - 8);
		og_canvas_clear(canvas1);
		for(i = 0; i < 5; i++)
		{
			angle_from_north -= 15;
			for(j = 0; j < 5; j++)
			{
				cosA = cos(angle_from_north * 0.017453292) * (68 + j * 2);
				sinA = sin(angle_from_north * 0.017453292) * (68 + j * 2);
				og_canvas_drawDot(canvas1, 64 - sinA, 80 - cosA);
				og_canvas_drawDot(canvas1, 64 +	cosA, 80 - sinA);
				og_canvas_drawDot(canvas1, 64 + sinA, 80 + cosA);
				og_canvas_drawDot(canvas1, 64 - cosA, 80 + sinA);
			}
		}
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
	if(event == KEY1_PRESSED)
		pageShift(page_menu, PAGE_RETURN);
	else if(event == KEY2_PRESSED)
		pageShift(page_magcal, PAGE_ENTER);
}
void pageRegister_page_compass(unsigned char pageID)
{
	pageRegister(pageID, "COMPASS", Icon, Setup, Loop, Exit, Event);
}