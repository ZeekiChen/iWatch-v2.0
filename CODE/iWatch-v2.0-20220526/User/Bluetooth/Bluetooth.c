#include "Bluetooth.h"
#include "System.h"

#define	RX4_BUF_SIZE	64
unsigned char xdata uart4_busy_flag = 0;
unsigned char xdata rx4_cnt = 0;
unsigned char xdata rx4_buf[RX4_BUF_SIZE];	//����4���ջ�����

void Uart4Isr() interrupt 18
{
	if (S4CON & 0x02)		//�����ж�
  {
		S4CON &= ~0x02;
    uart4_busy_flag = 0;
  }
  if (S4CON & 0x01)		//�����ж�
  {
		S4CON &= ~0x01;
    rx4_buf[rx4_cnt] = S4BUF;
		if(++rx4_cnt == RX4_BUF_SIZE)
			rx4_cnt = 0;
  }
}
void Uart4SendChar(char c)
{
	while(uart4_busy_flag);
	uart4_busy_flag = 1;
	S4BUF = c;
}

void BtOn(void)
{
	unsigned char i;
	for(i = 0; i < RX4_BUF_SIZE; i++)
		rx4_buf[i] = 0;
	rx4_cnt = 0;
	IE2 |= 0x10;	//������4�ж�
	P0 |= 0x0c;
	BTEN = 0;			//������
}
void BtOff(void)
{
	BTEN = 1;			//�ر�����
	IE2 &= ~0x10;	//�ش���4�ж�
	P0 &= ~0x0c;	//ȡ������4IO����
}
unsigned char BtCheckConnection(void)
{
	return BTSTA;
}
unsigned char BtReceiveString(char *dptr)
{
	if(rx4_cnt != 0)
	{
		unsigned char i;
		unsigned char temp = rx4_cnt;
		rx4_cnt = 0;
		for(i = 0; i < temp; i++)
		{
			dptr[i] = rx4_buf[i];
		}
		dptr[i] = '\0';
		return temp;
	}
	else
		return 0;
}
void BtSendString(const char *str)
{
	while(*str)
	{
		Uart4SendChar(*str);
		str++;
	}
}