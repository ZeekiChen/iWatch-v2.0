#ifndef __OLED_H
#define __OLED_H

//-----------------OLED端口定义----------------  	
#define OLED_RST	P11		//复位
#define OLED_DC		P47		//数据/命令控制
#define OLED_CS		P10 		//片选
#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_CS_Clr() OLED_CS=0
#define OLED_CS_Set() OLED_CS=1
//--------------------------------------------  						  		   

//OLED控制用函数
void OLED_Display_On(void);
void OLED_Display_Off(void);	
void OLED_Set_Brightness(unsigned char brightness);
void OLED_setBreathing(unsigned char k, unsigned char interval);
//void OLED_Horizental_Reverse(unsigned char reverse);
//void OLED_Vertical_Reverse(unsigned char reverse);
void OLED_Inverse(unsigned char k);
void OLED_Clear(void);
//void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Refresh(unsigned char *buf);
void OLED_Init(void);

#endif  
	 



