#ifndef _OLED_CONF_H
#define _OLED_CONF_H

#if defined(__C251__) || defined(__C51__)		// 使用STC32/16/8/15/12系列单片机
typedef	signed char				int8_t;
typedef	unsigned char			uint8_t;
typedef	signed int				int16_t;
typedef	unsigned int			uint16_t;
typedef	signed long int 	int32_t;
typedef	unsigned long int uint32_t;
#define	CODE	code
#define	DATA	data
#define	XDATA xdata
#else																				// 使用32位内核或者其他内核的单片机
typedef	signed char				int8_t;
typedef	unsigned char			uint8_t;
typedef	signed short			int16_t;
typedef	unsigned short		uint16_t;
typedef	signed int 				int32_t;
typedef	unsigned int 			uint32_t;
//typedef	signed long int 	int64_t;
//typedef	unsigned long int uint64_t;
#define	CODE	const
#define	DATA
#define	XDATA
#endif

#ifdef __OELD_GUI_C

/********栈空间大小***********/
#if defined(__C251__) || defined(__C51__)	
#define	HEAP_SIZE			4096			// 用于控件参数和显存的分配，根据需要来调整
																// 在使用MSC51/MSC251内核的单片机时，需要调用init_mempool(mem_buf, HEAP_SIZE)来分配堆内存空间
																// 在stm32平台中无需使用init_mempool()，堆空间的大小在启动文件中修改
static uint8_t XDATA mem_buf[HEAP_SIZE];	// 堆内存池，用于分配控件参数
#endif
#endif
/********选择想要用的功能***********/
#define	OLED_USE_LOG			0					// 要使用串口打印LOG功能，请初始化串口并做好putChar()函数的重定向
#define	OLED_USE_ANIM			1
#define	OLED_USE_GROUP		1
/********选择想要用的控件***********/
#define	OLED_USE_BMP			1		//图片控件
#define	OLED_USE_LABEL		1		//标签控件
#define	OLED_USE_CHECKBOX	0		//复选框控件
#define	OLED_USE_SLIDER		1		//滑块控件
#define	OLED_USE_SWITCH		1		//开关控件
#define	OLED_USE_CANVAS		1		//画布控件
/**************字体*****************/
#define	OLED_USE_BUILDIN_FONT		0		//0：使用外部字库（前提是硬件上支持且移植好驱动）
																		//1：使用自带字库，目前支持三种尺寸的英文字符
#if (OLED_USE_BUILDIN_FONT == 1)		//选择要用到的内置字体
	#define	USE_FONT_ASIIC_6X8		1
	#define	USE_FONT_ASIIC_8X16		1
	#define	USE_FONT_ASIIC_16X32	1
#endif

/**************调试*****************/
//#define OLED_DEBUG								//在调试阶段可取消此注释
																		//确保程序无内存泄漏，访问越界等问题后
																		//可以注释掉这段，减少代码量提升运行速度
#if (OLED_USE_LOG == 0)
#define	OG_PRINTERROR(str)
#define	OG_LOG(str)
#endif

#define	OLED_WIDTH	128
#define	OLED_HEIGHT	64
/*****屏幕刷新时间间隔********/
#define	OLED_REFRESH_T		10  //ms
/*******动画持续时间**********/
#if OLED_USE_ANIM == 1
#define	ANIM_TIME_FAST		300	//ms
#define	ANIM_TIME_NORM		450	//ms
#define	ANIM_TIME_SLOW		700	//ms
/*******动画的类型**********/
#define ANIM_TYPE	2
//0:	ANIM_EASE_IN_EASE_OUT		//先加速后减速
//1:	ANIM_EASE_IN						//加速	
//2:	ANIM_EASE_OUT						//减速
//3:	ANIM_LINEAR							//匀速
//4:	ANIM_BOUNCE							//回弹
#endif

#endif


