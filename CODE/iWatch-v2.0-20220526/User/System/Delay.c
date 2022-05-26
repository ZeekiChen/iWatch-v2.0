#include "Delay.h"

void Delay1ms(unsigned int t)		//@35.000MHz
{
	unsigned char i, j;
	for(t; t>0; t--)
	{
		_nop_();
		_nop_();
		i = 46;
		j = 113;
		do
		{
			while (--j);
		} while (--i);
	}
}

void Delay1us(unsigned int t)		//@35.000MHz
{
	unsigned char i;
	for(t; t>0; t--)
	{
		_nop_();
		_nop_();
		i = 9;
		while (--i);
	}
}
