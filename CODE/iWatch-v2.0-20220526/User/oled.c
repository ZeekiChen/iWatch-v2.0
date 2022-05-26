#include "oled.h"
#include "System.h"
		   

void OLED_WR_CMD(unsigned char cmd)
{
	OLED_DC_Clr();
	SPI_WriteByte(cmd);
}
void OLED_WR_DAT(unsigned char dat)
{
	OLED_DC_Set();
	SPI_WriteByte(dat);
}
/*
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_CMD(0xb0 + y);
	OLED_WR_CMD(((x & 0xf0) >> 4) | 0x10);
	OLED_WR_CMD((x & 0x0f)); 
}   
*/
//开启OLED显示    
void OLED_Display_On(void)		//开启显示
{
	OLED_CS_Clr();
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON
	OLED_CS_Set();
}
//关闭OLED显示     
void OLED_Display_Off(void)		//关闭显示
{
	OLED_CS_Clr();
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF
	OLED_CS_Set();
}		
void OLED_Set_Brightness(unsigned char brightness)	//设置对比度（亮度）
{
	OLED_CS_Clr();
	OLED_WR_CMD(0x81);				//--set contrast control register
	OLED_WR_CMD(brightness);	// Set SEG Output Current Brightness
	OLED_CS_Set();
}
void OLED_setBreathing(unsigned char k, unsigned char interval)
{
	OLED_CS_Clr();
	OLED_WR_CMD(0x23);
	if(k)
		OLED_WR_CMD(0x30 | (interval & 0x0f));
	else
		OLED_WR_CMD(0x00);
	OLED_CS_Set();
}
/*
void OLED_Horizental_Reverse(unsigned char reverse)	//水平翻转
{
	if(reverse)
		OLED_WR_Byte(0xA0, OLED_CMD);
	else
		OLED_WR_Byte(0xA1, OLED_CMD);
}
void OLED_Vertical_Reverse(unsigned char reverse)		//垂直翻转
{
	if(reverse)
		OLED_WR_Byte(0xC0, OLED_CMD);
	else
		OLED_WR_Byte(0xC8, OLED_CMD);
}
*/
void OLED_Inverse(unsigned char k)									//反色
{
	OLED_CS_Clr();
	if(k)
		OLED_WR_CMD(0xA7);
	else
		OLED_WR_CMD(0xA6);
	OLED_CS_Set();
}
void OLED_Clear(void)		//清屏
{  
	unsigned int n;		    
	//OLED_WR_CMD(0xb0);    //设置页地址（0~7）
	//OLED_WR_CMD(0x00);    //设置显示位置—列低地址
	//OLED_WR_CMD(0x10);    //设置显示位置—列高地址   
	OLED_CS_Clr();
	for(n = 0; n < 1024; n++)
		OLED_WR_DAT(0); 
	OLED_CS_Set();
}
void OLED_Refresh(unsigned char *buf)
{
	//unsigned int data j; 
	//OLED_WR_CMD(0xb0);    //设置页地址（0~7）
	//OLED_WR_CMD(0x00);      //设置显示位置—列低地址
	//OLED_WR_CMD(0x10);      //设置显示位置—列高地址
	//OLED_WR_CMD(0x21);
	//OLED_WR_CMD(0x00);
	//OLED_WR_CMD(127);
	//OLED_WR_CMD(0x22);
	//OLED_WR_CMD(0);
	//OLED_WR_CMD(7);
	OLED_CS_Clr();
	OLED_DC_Set();
	/*
	for(j = 0; j < 1024; j++)
	{ 
		SPI_WriteByte(buf[j]);
	}
	*/
	SPI_WriteMultiBytes(buf, 1024);
	OLED_CS_Set();
}
//初始化SSD1315					    
void OLED_Init()
{
	OLED_RST_Set();
	Delay1ms(20);
	OLED_RST_Clr();
	Delay1ms(10);
	OLED_RST_Set(); 
	OLED_CS_Clr();
	
	OLED_WR_CMD(0xAE);//--turn off oled panel
	OLED_WR_CMD(0x02);//---set low column address
	OLED_WR_CMD(0x10);//---set high column address
	OLED_WR_CMD(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_CMD(0x81);//--set contrast control register
	OLED_WR_CMD(0x7f);// Set SEG Output Current Brightness
	OLED_WR_CMD(0xC8);
	OLED_WR_CMD(0xA1);
	OLED_WR_CMD(0xA8);//--set multiplex ratio(1 to 64)
	OLED_WR_CMD(0x3F);//--1/64 duty
	OLED_WR_CMD(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_CMD(0x00);//-not offset
	OLED_WR_CMD(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WR_CMD(0xf0);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_CMD(0xD9);//--set pre-charge period
	OLED_WR_CMD(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_CMD(0xDA);//--set com pins hardware configuration
	OLED_WR_CMD(0x12);
	OLED_WR_CMD(0xDB);//--set vcomh
	OLED_WR_CMD(0x40);//Set VCOM Deselect Level
	OLED_WR_CMD(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_CMD(0x00);//
	OLED_WR_CMD(0x21);
	OLED_WR_CMD(0);
	OLED_WR_CMD(127);
	OLED_WR_CMD(0x22);
	OLED_WR_CMD(0);
	OLED_WR_CMD(7);
	
	OLED_WR_CMD(0x8D);//--set Charge Pump enable/disable
	OLED_WR_CMD(0x14);//--set(0x10) disable
	OLED_WR_CMD(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_CMD(0xA6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_CMD(0xAF);//--turn on oled panel
	//OLED_WR_Byte(0xD6,OLED_CMD);
	//OLED_WR_Byte(0x01,OLED_CMD);
	//OLED_WR_Byte(0x23,OLED_CMD);
	//OLED_WR_Byte(0x30,OLED_CMD);
	
	OLED_CS_Clr();
	OLED_Clear();
	//OLED_Set_Pos(0,0); 	
}  

