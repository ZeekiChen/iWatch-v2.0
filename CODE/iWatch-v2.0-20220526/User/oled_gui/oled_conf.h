#ifndef _OLED_CONF_H
#define _OLED_CONF_H

#if defined(__C251__) || defined(__C51__)		// ʹ��STC32/16/8/15/12ϵ�е�Ƭ��
typedef	signed char				int8_t;
typedef	unsigned char			uint8_t;
typedef	signed int				int16_t;
typedef	unsigned int			uint16_t;
typedef	signed long int 	int32_t;
typedef	unsigned long int uint32_t;
#define	CODE	code
#define	DATA	data
#define	XDATA xdata
#else																				// ʹ��32λ�ں˻��������ں˵ĵ�Ƭ��
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

/********ջ�ռ��С***********/
#if defined(__C251__) || defined(__C51__)	
#define	HEAP_SIZE			4096			// ���ڿؼ��������Դ�ķ��䣬������Ҫ������
																// ��ʹ��MSC51/MSC251�ں˵ĵ�Ƭ��ʱ����Ҫ����init_mempool(mem_buf, HEAP_SIZE)��������ڴ�ռ�
																// ��stm32ƽ̨������ʹ��init_mempool()���ѿռ�Ĵ�С�������ļ����޸�
static uint8_t XDATA mem_buf[HEAP_SIZE];	// ���ڴ�أ����ڷ���ؼ�����
#endif
#endif
/********ѡ����Ҫ�õĹ���***********/
#define	OLED_USE_LOG			0					// Ҫʹ�ô��ڴ�ӡLOG���ܣ����ʼ�����ڲ�����putChar()�������ض���
#define	OLED_USE_ANIM			1
#define	OLED_USE_GROUP		1
/********ѡ����Ҫ�õĿؼ�***********/
#define	OLED_USE_BMP			1		//ͼƬ�ؼ�
#define	OLED_USE_LABEL		1		//��ǩ�ؼ�
#define	OLED_USE_CHECKBOX	0		//��ѡ��ؼ�
#define	OLED_USE_SLIDER		1		//����ؼ�
#define	OLED_USE_SWITCH		1		//���ؿؼ�
#define	OLED_USE_CANVAS		1		//�����ؼ�
/**************����*****************/
#define	OLED_USE_BUILDIN_FONT		0		//0��ʹ���ⲿ�ֿ⣨ǰ����Ӳ����֧������ֲ��������
																		//1��ʹ���Դ��ֿ⣬Ŀǰ֧�����ֳߴ��Ӣ���ַ�
#if (OLED_USE_BUILDIN_FONT == 1)		//ѡ��Ҫ�õ�����������
	#define	USE_FONT_ASIIC_6X8		1
	#define	USE_FONT_ASIIC_8X16		1
	#define	USE_FONT_ASIIC_16X32	1
#endif

/**************����*****************/
//#define OLED_DEBUG								//�ڵ��Խ׶ο�ȡ����ע��
																		//ȷ���������ڴ�й©������Խ��������
																		//����ע�͵���Σ����ٴ��������������ٶ�
#if (OLED_USE_LOG == 0)
#define	OG_PRINTERROR(str)
#define	OG_LOG(str)
#endif

#define	OLED_WIDTH	128
#define	OLED_HEIGHT	64
/*****��Ļˢ��ʱ����********/
#define	OLED_REFRESH_T		10  //ms
/*******��������ʱ��**********/
#if OLED_USE_ANIM == 1
#define	ANIM_TIME_FAST		300	//ms
#define	ANIM_TIME_NORM		450	//ms
#define	ANIM_TIME_SLOW		700	//ms
/*******����������**********/
#define ANIM_TYPE	2
//0:	ANIM_EASE_IN_EASE_OUT		//�ȼ��ٺ����
//1:	ANIM_EASE_IN						//����	
//2:	ANIM_EASE_OUT						//����
//3:	ANIM_LINEAR							//����
//4:	ANIM_BOUNCE							//�ص�
#endif

#endif


