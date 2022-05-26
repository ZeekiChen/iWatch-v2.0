#define	__IWATCH_C
#include "iWatch.h"

/*******************iWatch工作状态相关变量**********************/
static unsigned char xdata t_inactive = 0;	//不活跃的时间，没有动作产生所持续的时长
static unsigned char xdata t_idle = 0;			//空闲状态下持续的时间
static unsigned int xdata autowake_cnt = 0;		//自动唤醒的次数
/***************************************************************/

void KeyScan(void)					
{
	static unsigned int t_key_press = 0;	
	unsigned char dat = 0x00;
	if(KEY1 == 0)
		dat |= KEY1_PRESSED;
	if(KEY2 == 0)
		dat |= KEY2_PRESSED;
	if(KEY3 == 0)
		dat |= KEY3_PRESSED;
	if(KEY4 == 0)
		dat |= KEY4_PRESSED;
	Trg = dat&(dat^Cont);
	Trg ^= key_mask;
	key_mask = 0;
	Cont = dat;
	if(Cont)
	{
		if(++t_key_press > KEY_LONGPRESS_THRESHOLD)	//判断是否长按KEY_LONGPRESS_THRESHOLD
		{
			Trg = Cont;						//重复触发	
			t_key_press = KEY_REPEAT_RATE;
		}
	}
	else
		t_key_press = 0;
}
void TM1_Isr() interrupt 3	//MCU定时器1ms中断
{
	static unsigned int t_cnt = 0;
	pageTick();
	og_timeTick();
	if(t_cnt % KEY_SCAN_T == 0)		//每20ms执行一次按键扫描
	{
		KeyScan();
		if(Trg)
			iWatch_action = IWATCH_KEYPRESSED;	//产生按键动作
		if((KEY1 == 0) && (KEY2 == 0))
		{
			MCUSoftReset();
		}
	}
	if(++t_cnt == 1000)
	{
		t_cnt = 0;
		iWatchStatusUpdate();		//每秒钟执行一次状态更新
	}
}
void INT0_Isr() interrupt 0  //电源线接入中断
{
	if(PWR_SENSE == 0)
		iWatch_action = IWATCH_CABLEPLUGININT;
	else
		iWatch_action = IWATCH_CABLEUNPLUGINT;
}
void INT1_Isr() interrupt 2		//抬腕唤醒中断
{
	iWatch_action = IWATCH_WRISTUPINT;
}
void INT2_Isr() interrupt 10	//RX8130CE中断
{
	iWatch_action = IWATCH_RTCINT;
}
void INT3_Isr() interrupt 11	//按键2中断
{
	iWatch_action = IWATCH_KEYINT;
}
void iWatchStatusUpdate(void)
{
	if(iWatchCheckStatus(IWATCH_ACTIVE))	
	{
		if(config.t_inactive_max == 0)	
		{
			iWatchSetStatus(IWATCH_ACTIVE);
			t_inactive = 0;
			iWatchClearStatus(IWATCH_IDLE);
			t_idle = 0;
		}
		else
		{
			if(++t_inactive >= config.t_inactive_max)
			{
				iWatchClearStatus(IWATCH_ACTIVE);
				t_inactive = 0;
				iWatchSetStatus(IWATCH_IDLE);
				t_idle = 0;
			}
		}
	}
	if(iWatchCheckStatus(IWATCH_IDLE))
	{
		if(++t_idle >= 5)
		{
			iWatchClearStatus(IWATCH_IDLE);
			t_idle = 0;
			iWatchSetStatus(IWATCH_BEFORESLEEP);
		}
	}
}
void iWatchStatusHandle(void)
{
	//每当有动作产生（按键被按下，抬腕），action标志位被置1
	if(iWatch_action != 0)
	{
		switch(iWatch_action)
		{
			case IWATCH_KEYINT:						//按键中断
			{
				if(iWatchCheckStatus(IWATCH_ACTIVE) == 0)
					key_mask = KEY2_PRESSED;
				break;
			}
			case IWATCH_KEYPRESSED:				//有按键被按下
			{
				if(config.key_sound)
					Bee();
				if(iWatchCheckStatus(IWATCH_ACTIVE) == 0)
				{
					Trg = 0;
				}
				if(iWatchCheckStatus(IWATCH_ALARMWENTOFF))
				{
					Trg = 0;
					BuzzerOff();
					iWatchClearStatus(IWATCH_ALARMWENTOFF);
				}
				if(Trg)
					pageEventTransmit(Trg);
				break;
			}
			case IWATCH_WRISTUPINT:				//抬腕唤醒中断
			{
				if(iWatchCheckStatus(IWATCH_ACTIVE))
				{
					pageEventTransmit(AWT);
				}
				break;
			}
			case IWATCH_RTCINT:						//外部RTC中断
			{
				unsigned char temp = RX8130CEReadIntSrc();				
				if(temp == RTC_ALARM_INT)
				{
					RX8130CEClearAlarmFlag();
					Beebeebee();			//响铃
					iWatchSetStatus(IWATCH_ALARMWENTOFF);	//置位闹钟标志位
					Delay1ms(5);
					RX8130CEAlarmHandle(&config.alarm);	//闹钟处理
					iWatchSaveConfig(&config);
					break;
				}
				if(temp == RTC_TIMER_INT)
				{
					RX8130CEClearTimerFlag();
					RX8130CEReadTime(&time);
					//battery_life = iWatchGetBatteryLife();
					if(config.lsm6dsm_cfg & 0x02)	//如果计步器功能有开启的话，记录当日步数
					{	
						if((time.hour == 22) && (time.minute == 4))		//22:04,记录当天的步数数据
						{
							int i;
							for(i = 6; i > 0; i--)
							{
								memcpy((unsigned char *)&(config.step_data[i]), (unsigned char *)&(config.step_data[i- 1]), sizeof(tsSTEP));
							}
							config.step_data[0].month = time.month;
							config.step_data[0].day = time.day;
							config.step_data[0].step = LSM6DSMGetCurrentStep();
							LSM6DSMResetStepCounter();
							iWatchSaveConfig(&config);
						}
					}
					autowake_cnt++;
					iWatch_action = 0;
					goto Sleep;
				}
			}
			case IWATCH_CABLEPLUGININT:		//充电线接入中断
				break;
			case IWATCH_CABLEUNPLUGINT:		//充电线断开中断
			{
				RX8130CEReadTime(&config.last_charge_time);		//保存最近一次结束充电的时间
				iWatchSaveConfig(&config);
				break;	
			}
		}
		iWatch_action = 0;
		//当有动作被触发后，根据手表所处的状态来进行特定的处理
		if(iWatchCheckStatus(IWATCH_ACTIVE))
		{
			t_inactive = 0;
		}
		//如果手表处于空闲状态，退出空闲状态进入活跃状态，屏幕被点亮
		else if(iWatchCheckStatus(IWATCH_IDLE))
		{
			iWatchClearStatus(IWATCH_IDLE);
			t_idle = 0;
			iWatchSetStatus(IWATCH_ACTIVE);
			OLED_Display_On();
			iWatchSetStatus(IWATCH_SCREENON);
		}
		//如果手表处于睡眠状态，退出睡眠状态进入活跃状态，单片机开始开始运转，屏幕被点亮
		else if(iWatchCheckStatus(IWATCH_SLEEP))
		{
			iWatchClearStatus(IWATCH_SLEEP);
			iWatchSetStatus(IWATCH_ACTIVE);
			OLED_Display_On();		//点亮屏幕
			iWatchSetStatus(IWATCH_SCREENON);
			pageOpenCurrentPage();//打开休眠前关闭的页面
		}
		else if(iWatchCheckStatus(IWATCH_POWERDOWN))
		{
			MCUSoftReset();
			/*
			powerdown_flag = 0;
			active_flag = 1;
			SystemWake();					// 唤醒系统
			DisplayInit();
			ScreenOnOff(ON);			// 点亮屏幕
			screen_on_flag = 1;
			SensorInit();
			Timer3Init();
			PageOpenCurrentPage();//打开休眠前关闭的页面
			*/
		}
	}
	if(iWatchCheckStatus((IWATCH_ALARMWENTOFF | IWATCH_TIMERON)))
	{
		t_idle = 0;
	}
	if(iWatchCheckStatus(IWATCH_IDLE))
	{
		if(iWatchCheckStatus(IWATCH_SCREENON))
		{
			OLED_Display_Off();
			iWatchClearStatus(IWATCH_SCREENON);
		}
	}
	if(iWatchCheckStatus(IWATCH_BEFORESLEEP))
	{
		iWatchClearStatus(IWATCH_BEFORESLEEP);
		if(iWatchCheckStatus(IWATCH_SCREENON))
		{
			OLED_Display_Off();
			iWatchClearStatus(IWATCH_SCREENON);
		}
		pageCloseCurrentPage();
		iWatchSetStatus(IWATCH_SLEEP);
	}
	Sleep:
	if(iWatchCheckStatus(IWATCH_SLEEP))
	{
		if(pageGetStatus() == PAGE_IDLE)	//等待页面关闭后，才进入休眠
		{
			SystemSleep();
			Delay1us(10);
			SystemWake();
		}
	}
	if(iWatchCheckStatus(IWATCH_POWERDOWN))		
	{
		OLED_Display_Off();
		iWatchClearStatus(IWATCH_SCREENON);
		SystemPowerDown();
	}
}
void iWatchKeepActive(void)
{
	iWatchSetStatus(IWATCH_ACTIVE);
	t_inactive = 0;
}
void iWatchSleep(void)
{
	iWatch_action = 0;
	iWatchClearStatus(IWATCH_ACTIVE);
	iWatchClearStatus(IWATCH_IDLE);
	iWatchClearStatus(IWATCH_POWERDOWN);
	t_inactive = 0;
	t_idle = 0;
	iWatchSetStatus(IWATCH_BEFORESLEEP);
}
void iWatchPowerDown(void)
{
	iWatch_action = 0;
	iWatchClearStatus(IWATCH_ACTIVE);
	iWatchClearStatus(IWATCH_IDLE);
	iWatchClearStatus(IWATCH_POWERDOWN);
	t_inactive = 0;
	t_idle = 0;
	iWatchSetStatus(IWATCH_POWERDOWN);
}
void iWatchSaveConfig(iWatch_config *config)
{
	unsigned int i = 0;
	unsigned int temp = 0;
	for(i; i < CONFIG_SIZE - 2; i++)				//对结构体中每一字节求和
		temp += ((unsigned char *)config)[i];
	config->check_sum = temp;								//将求和结果存放在结构体最后一个数字中
	EEPROM_SectorErase(EE_ADDRESS1);				
	EEPROM_write_n(EE_ADDRESS1, (unsigned char *)config, CONFIG_SIZE);
}
unsigned char iWatchReadConfig(iWatch_config *config)
{
	unsigned int i = 0;
	unsigned int temp = 0;
	EEPROM_read_n(EE_ADDRESS1, (unsigned char *)config, CONFIG_SIZE);
	for(i; i < CONFIG_SIZE - 2; i++)				//对结构体中每一字节求和
		temp += ((unsigned char *)config)[i];
	if((temp == config->check_sum) && (temp != 0))				//检验数据是否正确完整，结构体最后两字节为检验求和字节
		return 1;
	else
	{
		for(i = 0; i < CONFIG_SIZE; i++)
			((unsigned char *)config)[i] = ((unsigned char *)(&default_config))[i];
		EEPROM_SectorErase(EE_ADDRESS1);				
		EEPROM_write_n(EE_ADDRESS1, (unsigned char *)config, CONFIG_SIZE);
		return 0;
	}
}
/**
	* @brief 	测量当前电池剩余电量，测量结果已经过窗口滤波
	* @param  无
	* @retval percentage：剩余电量的百分比
	*/
#define	V_SHUTDOWN		3.4		//放电截止电压
#define	V_FULLCHARGE	4.1 	//满电电压
#define	WINDOW_WIDTH	10		//窗口滤波宽度，适当的宽度使得电压百分比显示比较稳定
float iWatchGetBatteryLife(void)
{
	static bit first_time_flag = 0;
	static float queue[WINDOW_WIDTH];		//窗口滤波队列
	float queue_average = 0;						//队列均值
	float bat_v;
	float percentage;										//电量百分比
	unsigned char i = 0;
	bat_v = GetBatVoltage();
	if(bat_v >= 4.08)
		percentage = ((bat_v - 4.08) / (V_FULLCHARGE - 4.08)) * 0.1 + 0.9;
	else if(bat_v >= 4.00)
		percentage = ((bat_v - 4.00) / (4.08 - 4.00)) * 0.1 + 0.8;
	else if(bat_v >= 3.93)
		percentage = ((bat_v - 3.93) / (4.00 - 3.93)) * 0.1 + 0.7;
	else if(bat_v >= 3.87)
		percentage = ((bat_v - 3.87) / (3.93 - 3.87)) * 0.1 + 0.6;
	else if(bat_v >= 3.82)
		percentage = ((bat_v - 3.82) / (3.87 - 3.82)) * 0.1 + 0.5;
	else if(bat_v >= 3.79)
		percentage = ((bat_v - 3.79) / (3.82 - 3.79)) * 0.1 + 0.4;
	else if(bat_v >= 3.77)
		percentage = ((bat_v - 3.77) / (3.79 - 3.77)) * 0.1 + 0.3;
	else if(bat_v >= 3.73)
		percentage = ((bat_v - 3.73) / (3.77 - 3.73)) * 0.1 + 0.2;
	else if(bat_v >= 3.70)
		percentage = ((bat_v - 3.70) / (3.73 - 3.70)) * 0.05 + 0.15;
	else if(bat_v >= 3.68)
		percentage = ((bat_v - 3.68) / (3.70 - 3.68)) * 0.05 + 0.1;
	else if(bat_v >= 3.50)
		percentage = ((bat_v - 3.50) / (3.68 - 3.50)) * 0.05 + 0.05;
	else if(bat_v >= V_SHUTDOWN)
		percentage = ((bat_v - V_SHUTDOWN) / (3.50 - V_SHUTDOWN)) * 0.05;
	if(percentage > 1)
		percentage = 1;
	else if(percentage < 0)
		percentage = 0;
	if(first_time_flag == 0)						//若队列中没数据，初始化队列
	{
		first_time_flag = 1;
		for(i = 0; i < WINDOW_WIDTH; i++)
			queue[i] = percentage;
	}
	for(i = WINDOW_WIDTH - 1; i > 0; i--)
	{
		queue[i] = queue[i - 1];
		queue_average += queue[i];
	}
	queue_average += percentage;
	percentage = queue_average / WINDOW_WIDTH;
	queue[0] = percentage;
	return percentage;
}
/**
	* @brief 	根据当前电量的百分比绘制一个电池图标
	* @param  bat：剩余电量的百分比
	* @retval 绘制的图标的指针
	*/
unsigned char *DrawBatteryIcon(float bat)
{
	unsigned char i, m;
	if(PWR_SENSE == 0 && CHRG_SENSE == 0)	//如果电源线接入且正在充电
		return BATTERY_CHARGING;						//直接返回一个正在充电的电池图标
	else
	{
		//根据当前电量百分比绘制一个电池图标
		for(i = 0; i < 24; i++)
			battery[i + 2] = BATTERY_LIFE_ICON[i];
		m = 18 * bat;
		for(i = 2; i < 2 + m; i++)
			battery[i + 2] |= 0x3c;
		return battery;
	}
}
void SensorInit(void)
{
	sensor_status = 0;
	if(LSM6DSMInit() != 0)
	{
		sensor_status |= LSM6DSM_ISPRESENTED;
		if(config.lsm6dsm_cfg & 0x01)
			LSM6DSMEnableAWT(10, 100);				//20 degree, 150ms
		if(config.lsm6dsm_cfg & 0x02)
			LSM6DSMEnablePedometer(1040, 6);	//debounce time = 1040ms, debounce step = 6 steps
	}
	#if (MAG_SENSOR_SELECT == 1)
	if(QMC5883LInit() != 0)
	{
		QMC5883LSetCaliValue(&config.mag_cal_Data);
		sensor_status |= QMC5883L_ISPRESENTED;
	}
	#endif
	#if (TH_SENSOR_SELECT == 1)
	if(HDC2080Init() != 0)
		sensor_status |= HDC2080_ISPRESENTED;
	#elif (TH_SENSOR_SELECT == 2)
	if(HDC1080Init() != 0)
		sensor_status |= HDC1080_ISPRESENTED;
	#elif (TH_SENSOR_SELECT == 3)
	#endif
	#if (P_SENSOR_SELECT == 1)
	if(BMP390Init() != 0)
		sensor_status |= BMP390_ISPRESENTED;
	#endif
}
void DisplayInit()
{
	OLED_Init();
	OLED_Inverse(config.screen_inverse);
	OLED_Set_Brightness((config.screen_brightness - 1) * 50 + 1);
}
void iWatchInit(void)
{
	SystemInit();						//系统初始化
	iWatchReadConfig(&config);
	DisplayInit();					//显示初始化
	RX8130CEInit();					//实时时钟初始化
	SensorInit();
	og_init();
	pageInit();
}