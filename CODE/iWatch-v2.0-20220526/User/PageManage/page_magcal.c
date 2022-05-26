#include "iWatch.h"

static t_psWidget text1;
static t_psWidget canvas1;
static unsigned int i = 0;

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup(unsigned char condition)
{
	text1 = og_label_create(FONT_GB2312_15X16, 0, 0);
	og_label_setText(text1, "请在１５秒内\n控制手表\n画出几个８字");
	canvas1 = og_canvas_create(128, 64, 0, 0);
	#if (MAG_SENSOR_SELECT == 1)
	QMC5883LClearCaliValue();
	QMC5883LSetMode(1);
	ResetMatrix();
	#endif
	LSM6DSMConfigAcc(ACC_ODR_208_HZ, ACC_SCALE_4_G);
	LSM6DSMConfigGyr(GYR_ODR_208_HZ, GYR_SCALE_500_DPS);
	i = 0;
}
/*
static void delet(void)
{
	//og_group_delet(group1);
	pageSetStatus(PAGE_IDLE);
}
*/
/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit(unsigned char condition)
{
	#if (MAG_SENSOR_SELECT == 1)
	Ellipsoid_fitting_Process(&config.mag_cal_Data);		//椭球校准算法
	QMC5883LSetCaliValue(&config.mag_cal_Data);					//设置校准参数
	QMC5883LSetMode(0);
	#endif
	iWatchSaveConfig(&config);
	if(config.lsm6dsm_cfg == 0)
		LSM6DSMConfigAcc(ACC_POWER_DOWN, ACC_SCALE_4_G);
	else
		LSM6DSMConfigAcc(ACC_ODR_26_HZ, ACC_SCALE_4_G);
	LSM6DSMConfigGyr(GYR_POWER_DOWN, GYR_SCALE_500_DPS);
	og_widget_delet(text1);
	og_widget_delet(canvas1);
	pageSetStatus(page_magcal, PAGE_IDLE);
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
	if(pageExecuteRate(&Rate20Hz))
	{
		i++;
		if(i == 30)
		{
			og_widget_setShow(text1, 0);
		}
		if(i > 30 && i < 300)
		{
			og_canvas_drawDot(canvas1, 64 + LSM6DSM.AngleX, 32 + LSM6DSM.AngleY);
			og_canvas_drawDot(canvas1, 64 + LSM6DSM.AngleX, 31 + LSM6DSM.AngleY);
			og_canvas_drawDot(canvas1, 63 + LSM6DSM.AngleX, 32 + LSM6DSM.AngleY);
			og_canvas_drawDot(canvas1, 63 + LSM6DSM.AngleX, 31 + LSM6DSM.AngleY);
			/*
			if(mag[0] == 0 && mag[1] == 0 && mag[2] == 0)
				return;
			*/
			#if (MAG_SENSOR_SELECT == 1)
			QMC5883LRead(mag);
			CalcData_Input(mag[0], mag[1], mag[2]);
			#endif
		}
		else if(i >= 300)
		{
			pageShift(page_compass, PAGE_RETURN);
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
		pageShift(page_compass, PAGE_RETURN);
}
void pageRegister_page_magcal(unsigned char pageID)
{
	pageRegister(pageID, NULL, NULL, Setup, Loop, Exit, Event);
}