#include "STC32G.h"
#include "Buzzer.h"

unsigned char bee_mode = 0;
unsigned int bee_time = 0;

void BuzzerInit(void)
{
	P_SW2 |= 0x80;
	
	PWMB_CCER1 = 0x00; //写 CCMRx 前必须先清零 CCxE 关闭通道
  PWMB_CCER2 = 0x00;
	
  PWMB_CCMR1 = 0x60;
  PWMB_CCER1 = 0x03;	//配置通道输出使能和极性
	
	PWMB_PSCRH = 0x00;
	PWMB_PSCRL = 0x00;
  PWMB_ARRH = 0x44; //设置PWM频率为2000Hz
  PWMB_ARRL = 0x5C;
	
	PWMB_CCR1H = 0x22;//设置占空比为50%
	PWMB_CCR1L = 0x2E;
	
	
  PWMB_ENO = 0x01; //使能PWM5P输出
	PWMB_PS = 0x00; //选择PWM5输出管脚P20

  PWMB_BKR = 0x80; //使能主输出
  //PWMB_CR1 |= 0x01; //开始计时
  P_SW2 &= 0x7f;
	
	T4T3M &= 0xFD;		//定时器时钟12T模式
	T3L = 0x58;		//设置定时初始值
	T3H = 0x9E;		//设置定时初始值
	//T4T3M |= 0x08;		//定时器3开始计时
	//ET3 = 1;
	IE2 |= 0X20;
	
	BUZZER = 0;
}
void PWMOff(void)
{
	P_SW2 |= 0x80;
	PWMB_CR1 &= ~0x01;		//PWM计数器开始计数
	PWMB_ENO &= ~0x01;
	P_SW2 &= ~0x80;
}
void PWMOn(void)
{
	P_SW2 |= 0x80;
	PWMB_CR1 |= 0x01;		//PWM计数器开始计数
	PWMB_ENO |= 0x01;
	P_SW2 &= ~0x80;
}
void BuzzerOff(void)
{
	T4T3M &= ~0x08;
	PWMOff();
	BUZZER = 0;
}
void Bee(void)
{
	bee_mode = 0;
	bee_time = 0;
	
	PWMOn();
	T4T3M |= 0x08;			
}
void Beebeebee(void)
{
	bee_mode = 1;
	bee_time = 0;
	
	PWMOn();
	T4T3M |= 0x08;		
}
void TM3_Isr() interrupt 19
{
	AUXINTIF &= ~0x02;
	if(bee_mode == 0)
	{
		if(++bee_time == 10)
		{
			bee_time = 0;
			BuzzerOff();
		}
	}
	else if(bee_mode == 1)
	{
		if(bee_time < 60)
		{
			if(bee_time % 10 == 0)
			{
				if(((bee_time / 10) % 2) == 1)
				{
					PWMOff();
				}
				else
				{
					PWMOn();
				}
			}
		}
		if(++bee_time == 100)
		{
			bee_time = 0;
		}
	}
}
