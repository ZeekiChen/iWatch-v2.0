#define	_SYSTEM_C
#include "System.h"

void GPIOInit(void)
{
	//P03:BTTX, P02:BTRX, P00:BTSTA
	//P07:KEY4, P06:KEY3
	P0M0 = 0x00;
	P0M1 = 0x01;
	P0 = 0x00;		
	//P13:CHAR_SENSE, P11:RES, P10:CS1
	//P17:3V3_EN, P15:SCL, P14:SDA
	P1M0 = 0x30;
	P1M1 = 0x30;
	P_SW2 |= 0x80;
	P1PU = 0x30;
	P_SW2 &= 0x7f;
	P1 = 0x01;
	//P23:MOSI, P20:BUZZER
	//P25:SCLK, P24:MISO
	P2M0 = 0x29;
	P2M1 = 0x00;
	P2 = 0x00;
	//P33:INT1, P32:PWR_SENSE, P31:TXD, P30:RXD
	//P37:KEY2, P36:INT2, P34:T0
	P3M0 = 0x00;
	P3M1 = 0x04;
	P3 = 0x50;
	//P43:KEY1
	//P47:DC, P45:BTEN, P44:CS2
	P4M0 = 0x00;
	P4M1 = 0x00;
	P4 = 0x10;

	P5M0 = 0x00;
	P5M1 = 0x00;
	P5 = 0x00;

	KEY1 = 1;
	KEY2 = 1;
	KEY3 = 1;
	KEY4 = 1;	
	EN_3V3 = 1;
	CHRG_SENSE = 1;//���IC�ڲ���©������MCU�������
	BTSTA = 0;
	BTEN = 1;
	BUZZER = 0;
}
void UartInit(void)		//115200bps@35.0MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xB4;		//���ö�ʱ��ʼֵ
	T2H = 0xFF;		//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	TI = 1;
}
void Uart4Init(void)		//115200bps@35.0MHz
{
	//P_SW2 |= 0x04;  //RXD4_2/P5.2, TXD4_2/P5.3
	S4CON = 0x10;		//8λ����,�ɱ䲨����
	S4CON |= 0x40;	//����4ѡ��ʱ��4Ϊ�����ʷ�����
	T4T3M |= 0x20;	//��ʱ��ʱ��1Tģʽ
	T4L = 0xB4;			//���ö�ʱ��ʼֵ
	T4H = 0xFF;			//���ö�ʱ��ʼֵ
	T4T3M |= 0x80;	//��ʱ��4��ʼ��ʱ
}
void Timer1Init(void)		//1����@35.0MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x48;		//���ö�ʱ��ʼֵ
	TH1 = 0x77;		//���ö�ʱ��ʼֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}
void Timer0Init(void)
{
	TMOD |= 0x04;    //�ⲿ����ģʽ
  TL0 = 0x00;
  TH0 = 0x00;
  TR0 = 0;       	//�رն�ʱ��
  ET0 = 1;       	//ʹ�ܶ�ʱ���ж�
}
void IntInit(void)
{
	//��Դ�߲������ж�
	IT0 = 0;            //ʹ��INT0�����غ��½����ж�
  EX0 = 1;            //ʹ��INT0�ж�
	IT1 = 1;            //ʹ��INT1�½����ж�
  EX1 = 1;            //ʹ��INT1�жϣ�LSM6DSM�ж�Դ
	INTCLKO |= 0x10;    //ʹ��INT2�жϣ�RCT�ж�Դ
	INTCLKO |= 0x20;    //ʹ��INT3�жϣ�KEY2�ж�
	//INTCLKO |= 0x40; 	//ʹ��INT4�ж�
}
/*
void INT4_int (void) interrupt 16
{
	MCUSoftReset();
}
*/
void MCUSoftReset(void)
{
	P3 |= 0x03;
	Delay1ms(50);
	IAP_CONTR = 0x60;
}
void EnableWatchDog(void)
{
	WDT_CONTR = 0x27;			//���������Լ1.43sûι����MCU�Զ���λ
}
void FeedWatchDog(void)
{
	WDT_CONTR |= 0x10;		//ι����������Ź�������
}
void ADC_Init(void)
{
	P_SW2 |= 0x80;
  ADCTIM = 0x3f;		
  P_SW2 &= 0x7f;
  ADCCFG = 0x20 | 0x08;  //ADC_RES��������4λ��ADC_RESL��������8λ
												 //ADCʱ�ӣ�SYSclk/2/(0x08+1)
  ADC_CONTR = 0x80; //0x80:��ADC��Դ
}
void ADC_Deinit(void)
{
	ADC_CONTR = 0x00;
}
unsigned int Get_ADC12bitResult(unsigned char channel)  //channel = 0~15
{
	ADC_RES = 0;
	ADC_RESL = 0;

	ADC_CONTR = (ADC_CONTR & 0xF0) | 0x40 | channel;
  _nop_();
  _nop_();
  _nop_();
  _nop_();

	while((ADC_CONTR & 0x20) == 0);   //wait for ADC finish
  ADC_CONTR &= ~0x20;     					

	return (((unsigned int)ADC_RES << 8) | ADC_RESL);
}
float GetBatVoltage(void)
{
	unsigned int temp;
	temp = Get_ADC12bitResult(15);	//1.2575414 
	return (4096.0 * 1.1787277 / (float)temp);
}
void SystemSleep(void)
{
	KEY1 = 0;
	KEY3 = 0;
	KEY4 = 0;
	BuzzerOff();
	ET0 = 0;
	ET1 = 0;
	
	//IIC_Deinit();
	//SPI_Deinit();
	ADC_Deinit();
	/*
	P1M0 = 0x00;
	P1M1 = 0x00;
	P_SW2 |= 0x80;
	P1PU = 0x00;
	P_SW2 &= 0x7f;
	*/
	P14 = 1;
	P15 = 1;
	//CHRG_SENSE = 0;
	PCON = 0x02;		//MCU�������ģʽ
	_nop_();
	_nop_();
}
void SystemPowerDown(void)
{
	KEY1 = 0;
	KEY3 = 0;
	KEY4 = 0;
	BuzzerOff();
	ET0 = 0;
	ET1 = 0;

	IIC_Deinit();
	SPI_Deinit();
	//ADC_Deinit();
	/*
	P1M0 = 0x00;
	P1M1 = 0x00;
	*/
	P_SW2 |= 0x80;
	P1PU = 0x00;
	P_SW2 &= 0x7f;
	
	P14 = 0;
	P15 = 0;
	/*
	P23 = 0;
	P24 = 0;
	P25 = 0;
	
	P11 = 0;
	P47 = 0;
	
	P23 = 0;
	P24 = 0;
	P25 = 0;

	P10 = 0;
	//P44 = 0;

	//P33:INT1, P32:PWR_SENSE, P31:TXD, P30:RXD
	//P37:KEY2,P36:INT2

	P3M0 = 0x00;
	P3M1 = 0x00;
	PWR_SENSE = 1;
	//P3 = 0x00;

	//P36 = 0;
	//P5 = 0;
	//P4 = 0x10;
	//BTEN = 0;
	*/
	EN_3V3 = 0;
	PCON = 0x02;
}
void SystemWake(void)
{
	//GPIOInit();
	KEY1 = 1;
	KEY3 = 1;
	KEY4 = 1;
	ET0 = 1;
	ET1 = 1;
	//IIC_Init();
	//SPI_Init();
	ADC_Init();
}
void SystemInit(void)
{
	WTST = 0;
	GPIOInit();
	UartInit();			//��ʼ������1
	Uart4Init();		//��ʼ������4����������
	IIC_Init();
	SPI_Init();
	Timer1Init();
	Timer0Init();
	BuzzerInit();		//��������ʼ��
	IntInit();			//�ⲿ�жϳ�ʼ��
	ADC_Init();
	Delay1ms(50);
	//printf("Hello iWatch-v2.1\n");
	EA = 1;					//��ȫ���ж�
	EnableWatchDog();	//ʹ�ܿ��Ź�
}