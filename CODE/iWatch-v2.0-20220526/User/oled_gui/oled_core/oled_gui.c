#define __OELD_GUI_C
#include "oled_gui.h"

void og_timeTick(void)
{
	og_tick ++;
}
//���ص�ǰʱ���
uint16_t og_getTimestamp(void)
{
	return og_tick;
}
// ���ص�ǰʱ����͸���ʱ���֮���ʱ������ms��
uint16_t og_tickElaps(uint16_t pre_tick)
{
  if(og_tick >= pre_tick)
	{
		pre_tick = og_tick - pre_tick;
  } 
	else
	{
		pre_tick = 0xffff - pre_tick + 1;
    pre_tick += og_tick;
  }
  return pre_tick;
}
void og_init(void)
{
	#if defined(__C251__) || defined(__C51__)	
	uint16_t i;
	for(i = 0; i < HEAP_SIZE; i++)		// ��ʼ��ջ			
		mem_buf[i] = 0;
	init_mempool(mem_buf, HEAP_SIZE); // ��ʼ���ڴ��
	#endif
	og_widget_init();		// ��ʼ���ؼ�������
	#if OLED_USE_ANIM == 1
	og_anim_init();			// ��ʼ������ϵͳ
	#endif
}
void og_run(void)
{
	static uint16_t gui_timestamp = 0;
	if(og_tickElaps(gui_timestamp) >= OLED_REFRESH_T)
	{
		gui_timestamp = og_getTimestamp();
		#if OLED_USE_ANIM == 1
		og_anim_run();
		#endif
		og_widget_draw();
	}
}


