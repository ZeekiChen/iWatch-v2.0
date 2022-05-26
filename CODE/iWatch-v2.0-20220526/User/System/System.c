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
	CHRG_SENSE = 1;//充电IC内部开漏，所以MCU这端上拉
	BTSTA = 0;
	BTEN = 1;
	BUZZER = 0;
}
void UartInit(void)		//115200bps@35.0MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xB4;		//设置定时初始值
	T2H = 0xFF;		//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
	TI = 1;
}
void Uart4Init(void)		//115200bps@35.0MHz
{
	//P_SW2 |= 0x04;  //RXD4_2/P5.2, TXD4_2/P5.3
	S4CON = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;	//串口4选择定时器4为波特率发生器
	T4T3M |= 0x20;	//定时器时钟1T模式
	T4L = 0xB4;			//设置定时初始值
	T4H = 0xFF;			//设置定时初始值
	T4T3M |= 0x80;	//定时器4开始计时
}
void Timer1Init(void)		//1毫秒@35.0MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x48;		//设置定时初始值
	TH1 = 0x77;		//设置定时初始值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;
}
void Timer0Init(void)
{
	TMOD |= 0x04;    //外部计数模式
  TL0 = 0x00;
  TH0 = 0x00;
  TR0 = 0;       	//关闭定时器
  ET0 = 1;       	//使能定时器中断
}
void IntInit(void)
{
	//电源线插入检测中断
	IT0 = 0;            //使能INT0上升沿和下降沿中断
  EX0 = 1;            //使能INT0中断
	IT1 = 1;            //使能INT1下降沿中断
  EX1 = 1;            //使能INT1中断，LSM6DSM中断源
	INTCLKO |= 0x10;    //使能INT2中断，RCT中断源
	INTCLKO |= 0x20;    //使能INT3中断，KEY2中断
	//INTCLKO |= 0x40; 	//使能INT4中断
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
	WDT_CONTR = 0x27;			//如果超过大约1.43s没喂狗，MCU自动复位
}
void FeedWatchDog(void)
{
	WDT_CONTR |= 0x10;		//喂狗，清除看门狗计数器
}
void ADC_Init(void)
{
	P_SW2 |= 0x80;
  ADCTIM = 0x3f;		
  P_SW2 &= 0x7f;
  ADCCFG = 0x20 | 0x08;  //ADC_RES保存结果高4位，ADC_RESL保存结果后8位
												 //ADC时钟：SYSclk/2/(0x08+1)
  ADC_CONTR = 0x80; //0x80:开ADC电源
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
	PCON = 0x02;		//MCU进入掉电模式
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
	UartInit();			//初始化串口1
	Uart4Init();		//初始化串口4，用于蓝牙
	IIC_Init();
	SPI_Init();
	Timer1Init();
	Timer0Init();
	BuzzerInit();		//蜂鸣器初始化
	IntInit();			//外部中断初始化
	ADC_Init();
	Delay1ms(50);
	//printf("Hello iWatch-v2.1\n");
	EA = 1;					//开全局中断
	EnableWatchDog();	//使能看门狗
}