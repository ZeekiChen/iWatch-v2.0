#define __OELD_GUI_C
#include "oled_gui.h"

void og_timeTick(void)
{
	og_tick ++;
}
//返回当前时间戳
uint16_t og_getTimestamp(void)
{
	return og_tick;
}
// 返回当前时间戳和给定时间戳之间的时间间隔（ms）
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
	for(i = 0; i < HEAP_SIZE; i++)		// 初始化栈			
		mem_buf[i] = 0;
	init_mempool(mem_buf, HEAP_SIZE); // 初始化内存池
	#endif
	og_widget_init();		// 初始化控件管理器
	#if OLED_USE_ANIM == 1
	og_anim_init();			// 初始化动画系统
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


