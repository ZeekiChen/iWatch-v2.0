#include "GUI_PAGE.h"
#include "stdio.h"

extern lv_obj_t * scr;				//��Ļ����
static lv_obj_t * appWindow;	//���ڶ���Ϊ��Ļ������Ӷ���
static void setup_anim_ready_cb(lv_anim_t * a);
static void exit_anim_ready_cb(lv_anim_t * a);
static void btn_event_cb(lv_obj_t * btn, lv_event_t event);
static lv_obj_t * label_time;
static lv_obj_t * label_date;
static RTC_TimeTypeDef RTC_TimeStructure;
static RTC_DateTypeDef RTC_DateStructure;
static char time_str[9] = "  :  :  \0";
static char date_str[11] = "    .  .  \0";
/**
  * @brief  ҳ���ʼ���¼�
  * @param  ��
  * @retval ��
  */
static void Setup()
{
	/*����ҳ��*/
	appWindow = lv_cont_create(scr, NULL);
	lv_obj_set_pos(appWindow, 240, 0);
	lv_obj_set_size(appWindow, PAGE_HOR_SIZE, PAGE_VER_SIZE);
	//lv_obj_set_drag(appWindow, true);
	//lv_obj_set_drag_dir(appWindow, LV_DRAG_DIR_VER);
	/*����ҳ���Ա*/
	lv_obj_t * label1 = lv_label_create(appWindow, NULL);
	lv_obj_set_size(label1, 70, 40);
	lv_obj_align(label1, appWindow, LV_ALIGN_IN_TOP_LEFT, 20, 20);
	lv_label_set_text(label1, "NMSL");
	
	lv_obj_t * btn1 = lv_btn_create(appWindow, NULL);
	lv_obj_set_size(btn1, 70, 40);
	lv_obj_align(btn1, label1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
	lv_obj_set_click(btn1, true);
	lv_obj_set_event_cb(btn1, btn_event_cb);
	
	lv_obj_t * sw1 = lv_sw_create(appWindow, NULL);
	lv_obj_set_size(sw1, 60, 30);
	lv_obj_align(sw1, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
	
	lv_obj_t * slider1 = lv_slider_create(appWindow, NULL);
	lv_obj_set_size(slider1, 140, 30);
	lv_obj_align(slider1, sw1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
	lv_slider_set_range(slider1, 0, 100);
	lv_slider_set_value(slider1, 50, LV_ANIM_ON);
	lv_slider_set_knob_in(slider1, false);
	
	static lv_style_t style1;
	lv_style_copy(&style1, &lv_style_pretty_color);
	style1.text.font = &lv_font_roboto_28;
	label_time = lv_label_create(appWindow, NULL);
	lv_obj_set_size(label_time, 70, 40);
	lv_obj_align(label_time, sw1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
	lv_label_set_style(label_time, LV_LABEL_STYLE_MAIN, &style1);
	lv_label_set_text(label_time, time_str);
	
	label_date = lv_label_create(appWindow, label_time);
	lv_obj_align(label_date, label_time, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
	lv_label_set_text(label_date, date_str);
	
	lv_obj_t * label2 = lv_label_create(appWindow, label_time);
	lv_obj_align(label2, label_date, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
	lv_label_set_text(label2, LV_SYMBOL_OK LV_SYMBOL_CLOSE LV_SYMBOL_AUDIO LV_SYMBOL_WARNING LV_SYMBOL_REFRESH LV_SYMBOL_CUT LV_SYMBOL_GPS LV_SYMBOL_KEYBOARD);

  /*����ҳ���Ƶ�ǰ̨*/
  lv_obj_move_foreground(appWindow);
	GUI_Add_Anim(appWindow, x, 0, 500, setup_anim_ready_cb);

}
/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit()
{
	GUI_Add_Anim(appWindow, x, -240, 500, exit_anim_ready_cb);
}
/**
  * @brief  ҳ��ѭ��ִ�е�����
  * @param  ��
  * @retval ��
  */
static void Loop()
{
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	sprintf(time_str, "%02d:%02d:%02d", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	sprintf(date_str, "20%02d.%02d.%02d", RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date);
	lv_label_set_text(label_time, time_str);
	lv_label_set_text(label_date, date_str);
}
/**
  * @brief  ҳ���¼�
  * @param  btn:�����¼��İ���
  * @param  event:�¼����
  * @retval ��
  */
static void Event(uint8_t btn, uint8_t event)
{
	if(btn == KEY0 && event == KEY_PRESSED)
		GUI_Page_Shift(page1);
}

static void setup_anim_ready_cb(lv_anim_t * a)
{
	GUI_Page_Set_Status(PAGE_IDLE);
}
static void exit_anim_ready_cb(lv_anim_t * a)
{
	GUI_Page_Del(appWindow);		//�˳�������ɺ�ɾ��ҳ��
	GUI_Page_Set_Status(PAGE_IDLE);
}
static void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
	GUI_Page_Shift(page1);
}

void PageRegister_page0(uint8_t pageID)
{
	PageRegister(pageID, Setup, Loop, Exit, Event);
}
