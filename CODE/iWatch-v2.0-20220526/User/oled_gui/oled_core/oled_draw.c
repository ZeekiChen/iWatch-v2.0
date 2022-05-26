#define __OLED_DRAW_C
#include "oled_draw.h"

void og_clearDisplayBuf(void)
{
	uint16_t DATA i;
	for(i = 0; i < OLED_BUF_SIZE; i++)
		display_buf[i] = 0x00;
}
void og_screenRefresh(void)
{
	ScreenRefresh();
}
void BMPtoBuf(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t inverse, const uint8_t *buf1)
{
	uint8_t DATA temp;
	uint8_t DATA y_b, w_b, h_b, offset1, offset2;
	uint16_t DATA i, j;
	uint16_t DATA num2 = 0, num3, num4, num5;
	if(x > 127 || y > 63)
		return;
	if(inverse)
		temp = 0xff;
	else
		temp = 0x00;
	if(x < 0)
	{
		if(x + width < 0)
			return;
		else 
		{
			if(x + width < 128)
				w_b = width + x;
			else
				w_b = 128;
			num2 = -x;
			x = 0;
		}
	}
	else if(x < 128)
		w_b = (x+width)>128?(128-x):width;
	if(y < 0)
	{
		if((y + height) > 0)
		{
			h_b = (height + y - 1) >> 3;		//bmp图的行数（8个像素为一行）
			offset2 = (-y) % 8;							//y坐标的偏移量
			offset1 = 8 - offset2;
			num4 = ((-y) >> 3) * width + num2;
			for(j = 0; j < w_b; j++)
				display_buf[x + j] |= (buf1[num4 + j] ^ temp) >> offset2;
			for(i = 0; i < h_b; i++)		//当前所在行数
			{
				num3 = (i << 7) + x;
				num5 = num3 + 128;
				num4 = (i + ((-y) >> 3) + 1) * width + num2;
				for(j = 0; j < w_b; j++)
				{
					display_buf[num3 + j] |= ((buf1[num4 + j] ^ temp) << offset1);
					display_buf[num5 + j] |= ((buf1[num4 + j] ^ temp) >> offset2);
				}
			}
		}
		else 
			return;
	}
	else if(y < 64)
	{
		y_b = y >>  3;					//y坐标对应的行数（8个像素为一行）
		h_b = (y + height) > 63 ? (8 - y_b) : (height >> 3);		//bmp图的行数（8个像素为一行）
		offset1 = y % 8;				//y坐标的偏移量
		offset2 = 8 - offset1;
		for(i = 0; i < h_b; i++)		//当前所在行数
		{
			num3 = ((y_b + i) << 7) + x;	//num3 = (y_b + i) * 128;
			if(y_b + i == 7)
			{
				num4 = i * width + num2;
				for(j = 0; j < w_b; j++)
					display_buf[num3 + j] |= (buf1[num4 + j] ^ temp) << offset1;
			}
			else
			{
				num5 = num3 + 128;
				num4 = i * width + num2;
				for(j = 0; j < w_b; j++)
				{
					display_buf[num3 + j] |= (buf1[num4 + j] ^ temp) << offset1;
					display_buf[num5 + j] |= (buf1[num4 + j] ^ temp) >> offset2;
				}
			}
		}
	}
}
