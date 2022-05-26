#define	__IWATCH_C
#include "iWatch.h"

/*******************iWatch����״̬��ر���**********************/
static unsigned char xdata t_inactive = 0;	//����Ծ��ʱ�䣬û�ж���������������ʱ��
static unsigned char xdata t_idle = 0;			//����״̬�³�����ʱ��
static unsigned int xdata autowake_cnt = 0;		//�Զ����ѵĴ���
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
		if(++t_key_press > KEY_LONGPRESS_THRESHOLD)	//�ж��Ƿ񳤰�KEY_LONGPRESS_THRESHOLD
		{
			Trg = Cont;						//�ظ�����	
			t_key_press = KEY_REPEAT_RATE;
		}
	}
	else
		t_key_press = 0;
}
void TM1_Isr() interrupt 3	//MCU��ʱ��1ms�ж�
{
	static unsigned int t_cnt = 0;
	pageTick();
	og_timeTick();
	if(t_cnt % KEY_SCAN_T == 0)		//ÿ20msִ��һ�ΰ���ɨ��
	{
		KeyScan();
		if(Trg)
			iWatch_action = IWATCH_KEYPRESSED;	//������������
		if((KEY1 == 0) && (KEY2 == 0))
		{
			MCUSoftReset();
		}
	}
	if(++t_cnt == 1000)
	{
		t_cnt = 0;
		iWatchStatusUpdate();		//ÿ����ִ��һ��״̬����
	}
}
void INT0_Isr() interrupt 0  //��Դ�߽����ж�
{
	if(PWR_SENSE == 0)
		iWatch_action = IWATCH_CABLEPLUGININT;
	else
		iWatch_action = IWATCH_CABLEUNPLUGINT;
}
void INT1_Isr() interrupt 2		//̧�����ж�
{
	iWatch_action = IWATCH_WRISTUPINT;
}
void INT2_Isr() interrupt 10	//RX8130CE�ж�
{
	iWatch_action = IWATCH_RTCINT;
}
void INT3_Isr() interrupt 11	//����2�ж�
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
	//ÿ���ж������������������£�̧�󣩣�action��־λ����1
	if(iWatch_action != 0)
	{
		switch(iWatch_action)
		{
			case IWATCH_KEYINT:						//�����ж�
			{
				if(iWatchCheckStatus(IWATCH_ACTIVE) == 0)
					key_mask = KEY2_PRESSED;
				break;
			}
			case IWATCH_KEYPRESSED:				//�а���������
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
			case IWATCH_WRISTUPINT:				//̧�����ж�
			{
				if(iWatchCheckStatus(IWATCH_ACTIVE))
				{
					pageEventTransmit(AWT);
				}
				break;
			}
			case IWATCH_RTCINT:						//�ⲿRTC�ж�
			{
				unsigned char temp = RX8130CEReadIntSrc();				
				if(temp == RTC_ALARM_INT)
				{
					RX8130CEClearAlarmFlag();
					Beebeebee();			//����
					iWatchSetStatus(IWATCH_ALARMWENTOFF);	//��λ���ӱ�־λ
					Delay1ms(5);
					RX8130CEAlarmHandle(&config.alarm);	//���Ӵ���
					iWatchSaveConfig(&config);
					break;
				}
				if(temp == RTC_TIMER_INT)
				{
					RX8130CEClearTimerFlag();
					RX8130CEReadTime(&time);
					//battery_life = iWatchGetBatteryLife();
					if(config.lsm6dsm_cfg & 0x02)	//����Ʋ��������п����Ļ�����¼���ղ���
					{	
						if((time.hour == 22) && (time.minute == 4))		//22:04,��¼����Ĳ�������
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
			case IWATCH_CABLEPLUGININT:		//����߽����ж�
				break;
			case IWATCH_CABLEUNPLUGINT:		//����߶Ͽ��ж�
			{
				RX8130CEReadTime(&config.last_charge_time);		//�������һ�ν�������ʱ��
				iWatchSaveConfig(&config);
				break;	
			}
		}
		iWatch_action = 0;
		//���ж����������󣬸����ֱ�������״̬�������ض��Ĵ���
		if(iWatchCheckStatus(IWATCH_ACTIVE))
		{
			t_inactive = 0;
		}
		//����ֱ��ڿ���״̬���˳�����״̬�����Ծ״̬����Ļ������
		else if(iWatchCheckStatus(IWATCH_IDLE))
		{
			iWatchClearStatus(IWATCH_IDLE);
			t_idle = 0;
			iWatchSetStatus(IWATCH_ACTIVE);
			OLED_Display_On();
			iWatchSetStatus(IWATCH_SCREENON);
		}
		//����ֱ���˯��״̬���˳�˯��״̬�����Ծ״̬����Ƭ����ʼ��ʼ��ת����Ļ������
		else if(iWatchCheckStatus(IWATCH_SLEEP))
		{
			iWatchClearStatus(IWATCH_SLEEP);
			iWatchSetStatus(IWATCH_ACTIVE);
			OLED_Display_On();		//������Ļ
			iWatchSetStatus(IWATCH_SCREENON);
			pageOpenCurrentPage();//������ǰ�رյ�ҳ��
		}
		else if(iWatchCheckStatus(IWATCH_POWERDOWN))
		{
			MCUSoftReset();
			/*
			powerdown_flag = 0;
			active_flag = 1;
			SystemWake();					// ����ϵͳ
			DisplayInit();
			ScreenOnOff(ON);			// ������Ļ
			screen_on_flag = 1;
			SensorInit();
			Timer3Init();
			PageOpenCurrentPage();//������ǰ�رյ�ҳ��
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
		if(pageGetStatus() == PAGE_IDLE)	//�ȴ�ҳ��رպ󣬲Ž�������
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
	for(i; i < CONFIG_SIZE - 2; i++)				//�Խṹ����ÿһ�ֽ����
		temp += ((unsigned char *)config)[i];
	config->check_sum = temp;								//����ͽ������ڽṹ�����һ��������
	EEPROM_SectorErase(EE_ADDRESS1);				
	EEPROM_write_n(EE_ADDRESS1, (unsigned char *)config, CONFIG_SIZE);
}
unsigned char iWatchReadConfig(iWatch_config *config)
{
	unsigned int i = 0;
	unsigned int temp = 0;
	EEPROM_read_n(EE_ADDRESS1, (unsigned char *)config, CONFIG_SIZE);
	for(i; i < CONFIG_SIZE - 2; i++)				//�Խṹ����ÿһ�ֽ����
		temp += ((unsigned char *)config)[i];
	if((temp == config->check_sum) && (temp != 0))				//���������Ƿ���ȷ�������ṹ��������ֽ�Ϊ��������ֽ�
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
	* @brief 	������ǰ���ʣ���������������Ѿ��������˲�
	* @param  ��
	* @retval percentage��ʣ������İٷֱ�
	*/
#define	V_SHUTDOWN		3.4		//�ŵ��ֹ��ѹ
#define	V_FULLCHARGE	4.1 	//�����ѹ
#define	WINDOW_WIDTH	10		//�����˲���ȣ��ʵ��Ŀ��ʹ�õ�ѹ�ٷֱ���ʾ�Ƚ��ȶ�
float iWatchGetBatteryLife(void)
{
	static bit first_time_flag = 0;
	static float queue[WINDOW_WIDTH];		//�����˲�����
	float queue_average = 0;						//���о�ֵ
	float bat_v;
	float percentage;										//�����ٷֱ�
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
	if(first_time_flag == 0)						//��������û���ݣ���ʼ������
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
	* @brief 	���ݵ�ǰ�����İٷֱȻ���һ�����ͼ��
	* @param  bat��ʣ������İٷֱ�
	* @retval ���Ƶ�ͼ���ָ��
	*/
unsigned char *DrawBatteryIcon(float bat)
{
	unsigned char i, m;
	if(PWR_SENSE == 0 && CHRG_SENSE == 0)	//�����Դ�߽��������ڳ��
		return BATTERY_CHARGING;						//ֱ�ӷ���һ�����ڳ��ĵ��ͼ��
	else
	{
		//���ݵ�ǰ�����ٷֱȻ���һ�����ͼ��
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
	SystemInit();						//ϵͳ��ʼ��
	iWatchReadConfig(&config);
	DisplayInit();					//��ʾ��ʼ��
	RX8130CEInit();					//ʵʱʱ�ӳ�ʼ��
	SensorInit();
	og_init();
	pageInit();
}