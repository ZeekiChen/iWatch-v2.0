#include "iWatch.h"
#include "Bluetooth.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>

static unsigned char xdata mode = 0;
static unsigned char xdata data_mode = 0;

#define	RX1_BUF_SIZE		64	//���ڽ��ջ�����64�ֽ�
static unsigned char xdata uart1_busy_flag = 0;
static unsigned char xdata rx1_cnt = 0;
static unsigned char xdata rx1_buf[RX1_BUF_SIZE];		//����1���ջ�����
extern unsigned char xdata rx4_cnt;
extern unsigned char xdata rx4_buf[64];							//����4���ջ�����
static unsigned char temp[64];											//����ת��������

#define MAX_VA_LIST		10  //ÿ��ָ�����10������
#define	COM_BUF_SIZE	64	//ָ����ջ�����64�ֽ�
static unsigned char xdata com_buf[COM_BUF_SIZE]; //ָ����ջ�����
static unsigned char xdata com_index = 0;
static unsigned char *rx_buf;
static unsigned char *rx_tail;
static unsigned char xdata rx_head = 0;
char *pVaList[MAX_VA_LIST];
char *code CommandList[] =
{
	"setTime",
	"setConfig",
	"setAlarm",
	"setDataMode",
	"getConfig",
	"getAlarm",
	"getStep"
};
#define	COMMNAND_LIST_SUM	7

void Uart1Reset(void)
{
	unsigned char i;
	com_index = 0;
	rx_head = 0;
	for(i = 0; i < COM_BUF_SIZE; i++)
		com_buf[i] = 0;
	for(i = 0; i < RX1_BUF_SIZE; i++)
	{
		rx1_buf[i] = 0;
		rx4_buf[i] = 0;
	}
	rx1_cnt = 0;
	rx4_cnt = 0;
}
void UART1_Isr() interrupt 4
{
	if(TI)
	{
		TI = 0;
		uart1_busy_flag = 0;
	}
	if(RI)
  {
		RI = 0;			//���жϱ�־
		rx1_buf[rx1_cnt] = SBUF;
		if(++rx1_cnt == RX1_BUF_SIZE)
			rx1_cnt = 0;
  }
}
void Uart1SendString(const char *str)
{
	while(*str)
	{
		while(uart1_busy_flag);
		uart1_busy_flag = 1;
		SBUF = *str;
		str++;
	}
}
void MuxSendString(const char *str)
{
	if(mode == 0)
		Uart1SendString(str);
	else
		BtSendString(str);
}

//����Ӵ��ڽ�������ʶ��ָ�����ָ��Ͳ���
void DecVaList(char *pInput, char **argv)
{
	unsigned char i;
  unsigned char j;
  //�����ָ������洢��
	for (i = 0; i < MAX_VA_LIST; i++)
	{
		argv[i] = NULL;
  }
	j = 0;
	while (j < COM_BUF_SIZE)                            // Խ���ײ��Ŀհ��ַ�;
  {
		if (*pInput == '\0')
			break;
		if (*pInput >= '0' && *pInput <= '9' ||
				*pInput >= 'A' && *pInput <= 'Z' ||
				*pInput >= 'a' && *pInput <= 'z' ||
				*pInput == '-' || *pInput == '.')
		{
			break;
		}
		*pInput = '\0';
		pInput++;
		j++;
	}
	for (i = 0; i < MAX_VA_LIST; i++)                   // ��ദ�� MAX_VA_LIST ������;
  {
		while (j < COM_BUF_SIZE)                        // �����ҵ���ָ���롢����ֵ��
    {
			if (*pInput == '\0')
				break;
			if (*pInput >= '0' && *pInput <= '9' ||
          *pInput >= 'A' && *pInput <= 'Z' ||
          *pInput >= 'a' && *pInput <= 'z' ||
          *pInput == '-' || *pInput == '.')
      {
				if (argv[i] == NULL)
        {
					argv[i] = pInput;
        }
			}
			else
			{
				break;
			}
			pInput++;
			j++;
		}
		while (j < COM_BUF_SIZE)                        // Խ���ָ���;
    {
			if (*pInput == '\0')
				break;
			if (*pInput >= '0' && *pInput <= '9' ||
          *pInput >= 'A' && *pInput <= 'Z' ||
          *pInput >= 'a' && *pInput <= 'z' ||
          *pInput == '-' || *pInput == '.')
      {
				break;
			}
			*pInput = '\0';
			pInput++;
			j++;
		}
	}
	return;
}
void CommandProc(void)
{
	char ucRecByte;
	while(rx_head != *rx_tail)
	{
		ucRecByte = rx_buf[rx_head++];
		if(rx_head >= *rx_tail)
			rx_head = *rx_tail = 0;
		if(ucRecByte == '\r' || ucRecByte == '\n') //ÿ����⵽���ڽ��յ����з���س�����ִ��ָ��ʶ��
		{
			unsigned char i;
			com_buf[com_index] = '\0';
			DecVaList(com_buf, pVaList); 		//����ָ��洢��pVaList��
			com_index = 0;
			if(NULL == pVaList[0])
				continue;
			for (i = 0; i < COMMNAND_LIST_SUM; i++)      // �����յ���ָ����ָ���б��е�ָ����һ�Ƚ�
      {
				if(strcmp(pVaList[0], CommandList[i]) == 0)
        {
					break;      //���յ�ָ���������ָ���б�֮�У�����ѭ��
        }
			}
      if (i >= COMMNAND_LIST_SUM)			// ���յ�ָ�����
			{
        continue;
			}
			switch(i)
			{
				case 0:		//setTime
				{
					time.year = atoi(pVaList[1]) - 2000;
					time.month = atoi(pVaList[2]);
					time.day = atoi(pVaList[3]);
					time.hour = atoi(pVaList[4]);
					time.minute = atoi(pVaList[5]);
					time.second = atoi(pVaList[6]);
					time.weekday = atoi(pVaList[7]) + 1;
					RX8130CEWriteTime(&time);
					MuxSendString("OK\r\n");
					break;
				}
				case 1:		//setConfig
				{
					config.t_inactive_max = atoi(pVaList[1]);
					config.screen_brightness = atoi(pVaList[2]);
					config.screen_inverse = atoi(pVaList[3]);
					config.lsm6dsm_cfg = atoi(pVaList[4]);
					config.key_sound = atoi(pVaList[5]);
					OLED_Set_Brightness((config.screen_brightness - 1) * 50 + 1);
					OLED_Inverse(config.screen_inverse);
					MuxSendString("OK\r\n");
					break;
				}
				case 2:  	//setAlarm
				{
					config.alarm.mode = atoi(pVaList[1]);
					config.alarm.hour = atoi(pVaList[2]);
					config.alarm.minute = atoi(pVaList[3]);
					config.alarm.day = atoi(pVaList[4]);
					RX8130CESetAlarm(&config.alarm);
					MuxSendString("OK\r\n");
					break;
				}
				case 3:  	//setDataMode
				{
					data_mode = atoi(pVaList[1]);
					MuxSendString("OK\r\n");
					break;
				}
				case 4:		//getConfig
				{
					sprintf(temp, "CF#%u#%u#%u#%u#%u\r\n", config.t_inactive_max, config.screen_brightness, 
																					config.screen_inverse, config.lsm6dsm_cfg, config.key_sound);
					MuxSendString(temp);
					break;
				}
				case 5:		//getAlarm
				{
					sprintf(temp, "AL#%u#%u#%u#%u\r\n", config.alarm.mode, config.alarm.hour, config.alarm.minute, config.alarm.day);
					MuxSendString(temp);
					break;
				}
				case 6:		//getStep
				{
					tsSTEP *pStep = (tsSTEP *)config.step_data;
					MuxSendString("ST");
					for(i = 0; i < 7; i++)
					{
						sprintf(temp, "#%u/%u %u", pStep[i].month, pStep[i].day, pStep[i].step);
						MuxSendString(temp);
					}
					MuxSendString("\r\n");
					break;
				}
			}
		}
		else
		{
			com_buf[com_index++] = ucRecByte;
			if(com_index >= COM_BUF_SIZE)
				com_index = 0;
		}
	}
}

static unsigned char CODE Icon[] = {
	48, 48,
	0xE0,0xF0,0x30,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
	0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
	0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x30,0xF0,0xE0,
	0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,
	0x60,0x70,0xF8,0xFC,0xF8,0x70,0x60,0x40,0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
	0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x1F,0x1F,0x1F,0x00,0x00,0x00,0x20,0x60,0xE0,0xFF,0xFF,0xFF,0xE0,0x60,
	0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,
	0xFF,0xFF,0xC0,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x81,0x83,0x81,0x80,0x80,
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xC0,0xFF,0xFF,
	0x0F,0x1F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
	0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0xFF,0xFF,0xFF,0xFF,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
	0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x1F,0x0F,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x1C,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,
	0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1F,0x1F,0x1F,0x1F,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,
	0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1C,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	/* (48 X 48 )*/
};
static t_psWidget text1, text2;
static t_psGroup group1;
static unsigned char code MODE[3][9] = 
{
	{"��������\0"},
	{"��������\0"},
	{"��������\0"}
};

/**
  * @brief  ҳ���ʼ���¼�
  * @param  ��
  * @retval ��
  */
static void Setup(unsigned char condition)
{
	text1 = og_label_create(FONT_GB2312_15X16, 0, 0);
	text2 = og_label_create(FONT_GB2312_15X16, 32, 16);
	og_label_setText(text1, "ѡ��ģʽ��");
	og_label_setText(text2, MODE[mode]);
	group1 = og_group_create(2);
	og_group_addWidget(group1, text1, 0);
	og_group_addWidget(group1, text2, 0);
	og_group_setPosOffset(group1, 0, 64);
	og_group_addAnimOffset(group1, 0, -64, ANIM_TIME_NORM, ANIM_NULL_CB);
	#if (TH_SENSOR_SELECT == 1)
	HDC2080SetMode(7);
	HDC2080StartConversion();
	#elif (TH_SENSOR_SELECT == 2)
	HDC1080Start();
	#elif (TH_SENSOR_SELECT == 3)
	HTU21DStart();
	#endif
	#if (P_SENSOR_SELECT == 1)
	BMP390SetMode(NORMAL_MODE);
	#endif
	#if (MAG_SENSOR_SELECT == 1)
	QMC5883LSetMode(1);
	#endif
	LSM6DSMConfigAcc(ACC_ODR_208_HZ, ACC_SCALE_4_G);
	LSM6DSMConfigGyr(GYR_ODR_208_HZ, GYR_SCALE_500_DPS);
	
	Uart1Reset();
	ES = 1;				//������1�ж�
	P3 |= 0x03;		//����1IO������
	BtOn();
	mode = 0;
	rx_buf = rx1_buf;
	rx_tail = &rx1_cnt;
}
static void delet(void)
{
	og_group_delet(group1);
	pageSetStatus(page_ilink, PAGE_IDLE);
}
/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit(unsigned char condition)
{
	iWatchSaveConfig(&config);
	#if (TH_SENSOR_SELECT == 1)
	HDC2080SetMode(0);
	#elif (TH_SENSOR_SELECT == 2)
	#elif (TH_SENSOR_SELECT == 3)
	#endif
	#if (P_SENSOR_SELECT == 1)
	BMP390SetMode(SLEEP_MODE);
	#endif
	#if (MAG_SENSOR_SELECT == 1)
	QMC5883LSetMode(0);
	#endif
	if(config.lsm6dsm_cfg == 0)
		LSM6DSMConfigAcc(ACC_POWER_DOWN, ACC_SCALE_4_G);
	else
		LSM6DSMConfigAcc(ACC_ODR_26_HZ, ACC_SCALE_4_G);
	LSM6DSMConfigGyr(GYR_POWER_DOWN, GYR_SCALE_500_DPS);
	ES = 0;				//�ش���1�ж�
	P3 &= ~0x03;	//ȡ������1IO������
	BtOff();			//�ر�����
	og_group_addAnimOffset(group1, 0, 64, ANIM_TIME_NORM, delet);
}
/**
  * @brief  ҳ��ѭ��ִ�е�����
  * @param  ��
  * @retval ��
  */
static void Loop()
{
	iWatchKeepActive();
	if(mode < 2)	// ���ݴ���ģʽ��USB���ڻ�����������
	{
		CommandProc();
		if(data_mode == 1)	//�����¶ȣ�ʪ�ȣ���ѹ����
		{
			if(pageExecuteRate(&Rate10Hz))
			{
				#if (TH_SENSOR_SELECT == 0)
				sprintf(temp, "TH#%.2f#%.2f\r\n", 0, 0);
				MuxSendString(temp);
				#elif (TH_SENSOR_SELECT == 1)
				HDC2080GetTH(&HDC2080);
				sprintf(temp, "TH#%.2f#%.2f\r\n", HDC2080.temperature + 0.005, HDC2080.humidity + 0.005);
				MuxSendString(temp);
				#elif (TH_SENSOR_SELECT == 2)
				if(HDC1080GetTH(&HDC1080))
				{
					sprintf(temp, "TH#%.2f#%.2f\r\n", HDC1080.temperature + 0.005, HDC1080.humidity + 0.005);
					MuxSendString(temp);
				}
				#elif (TH_SENSOR_SELECT == 3)
				static unsigned char step = 0;
				if(step == 0)
				{
					if(HTU21DGetTemperature(&HTU21D))
						step = 1;
				}
				else if(step == 1)
				{
					if(HTU21DGetHumidity(&HTU21D))
					{
						step = 0;
						sprintf(temp, "TH#%.2f#%.2f\r\n", HTU21D.temperature + 0.005, HTU21D.humidity + 0.005);
						MuxSendString(temp);
					}
				}
				#endif
			}
			if(pageExecuteRate(&Rate20Hz))
			{
				#if (P_SENSOR_SELECT == 0)
				sprintf(temp, "PA#%.1f\r\n", 0);
				#elif (P_SENSOR_SELECT == 1)
				BMP390GetSensorData(&BMP390);
				sprintf(temp, "PA#%.1f\r\n", BMP390.pressure + 0.05);
				#endif
				MuxSendString(temp);
			}
		}
		else if(data_mode == 2)	//���ͼ��ٶȣ����ٶ�����
		{
			if(pageExecuteRate(&Rate50Hz))
			{
				LSM6DSMReadGYRAndACC(&LSM6DSM);
				sprintf(temp, "AG#%d#%d#%d#%d#%d#%d\r\n", LSM6DSM.acc_x, LSM6DSM.acc_y, LSM6DSM.acc_z
																								, LSM6DSM.gyr_x, LSM6DSM.gyr_y, LSM6DSM.gyr_z);
				MuxSendString(temp);
			}
		}
		else if(data_mode == 3)	//���ʹų�����
		{
			if(pageExecuteRate(&Rate50Hz))
			{
				#if (MAG_SENSOR_SELECT == 0)
				sprintf(temp, "MG#%d#%d#%d\r\n", 0, 0, 0);
				#elif (MAG_SENSOR_SELECT == 1)
				QMC5883LRead(mag);
				sprintf(temp, "MG#%d#%d#%d\r\n", mag[0], mag[1], mag[2]);
				#endif
				MuxSendString(temp);
			}
		}
	}
	else	//��������ģʽ����USB���ڽ��յ�������ת�����������ڣ��������ڽ��յ�������ת����USB����
	{
		if(pageExecuteRate(&Rate10Hz))
		{
			if(BtReceiveString(temp))
			{
				Uart1SendString(temp);
			}
			if(rx1_cnt != 0)
			{
				unsigned char i;
				for(i = 0; i < rx1_cnt; i++)
				{
					temp[i] = rx1_buf[i];
				}
				temp[i] = '\0';
				rx1_cnt = 0;
				BtSendString(temp);
			}
		}
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
	if(event == KEY1_PRESSED)
		pageShift(page_menu, PAGE_RETURN);
	else 
	{
		if(event == KEY2_PRESSED)
		{
			if(++mode == 3)
				mode = 0;
		}
		else if(event == KEY4_PRESSED)
		{
			if(--mode == 255)
				mode = 2;
		}
		og_label_setText(text2, MODE[mode]);
		if(mode == 0)
		{
			rx_buf = rx1_buf;
			rx_tail = &rx1_cnt;
		}
		else if(mode == 1)
		{
			rx_buf = rx4_buf;
			rx_tail = &rx4_cnt;
		}
		Uart1Reset();
	}
}
void pageRegister_page_ilink(unsigned char pageID)
{
	pageRegister(pageID, "iLink", Icon, Setup, Loop, Exit, Event);
}