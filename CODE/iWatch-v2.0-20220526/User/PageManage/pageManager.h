#ifndef _PAGEMANAGER_H
#define _PAGEMANAGER_H

typedef void(*t_fSteupCallback)(unsigned char);
typedef void(*t_fExitCallback)(unsigned char);
typedef void(*t_fLoopCallback)(void);
typedef void(*t_fEventCallback)(unsigned char);
typedef struct {
	unsigned char *Title;
	unsigned char *Icon;
	unsigned char status;
	t_fSteupCallback SetupCallback;
  t_fLoopCallback LoopCallback;
  t_fExitCallback ExitCallback;
  t_fEventCallback EventCallback;
}t_sPageList;
typedef struct {
	unsigned int executeT;
	unsigned int last_timestamp;
}t_sPageRate;
void PageRegister(
    unsigned char pageID,
		unsigned char *pageTitle,
		unsigned char *pageIcon,
    t_fSteupCallback setupCallback,
    t_fLoopCallback loopCallback,
    t_fExitCallback exitCallback,
    t_fEventCallback eventCallback
);
#define PAGE_REG(name)\
do{\
    extern void pageRegister_##name(unsigned char pageID);\
    pageRegister_##name(##name);\
}while(0)

enum page_condtion
{
	PAGE_ENTER,
	PAGE_RETURN
};
enum page_status
{
	PAGE_IDLE,
	PAGE_INIT,
	PAGE_RUN,
	PAGE_EXIT
};

		
enum PageID{
	PAGE_NULL = -1,
	page_watch,
	page_menu,
	PAGE_MENU_MIN,
	page_timer,
	page_alarm,
	page_flashlight,
	page_lsm6dsl,
	page_hdc2080,
	page_compass,
	page_pedometer,
	page_trexrunner,
	page_snake,
	page_bluetooth_serial,
	page_ilink,
	page_devinfo,
	page_setting,
	PAGE_MENU_MAX,
	page_settime,
	page_magcal,
	PAGE_MAX
};

#ifdef	__PAGE_C
#define	PAGE_EXTERN	
#else
#define	PAGE_EXTERN	extern
#endif

/********************初始化一些全局变量*************************/
#ifdef	__PAGE_C
t_sPageRate Rate5Hz = {200, 0};
t_sPageRate Rate10Hz = {100, 0};
t_sPageRate Rate20Hz = {50, 0};
t_sPageRate Rate50Hz = {20, 0};
t_sPageRate Rate100Hz = {10, 0};
t_sPageRate Rate125Hz = {8, 0};
t_sPageRate Rate1000Hz = {1, 0};
t_sPageList pagelist[PAGE_MAX];
static unsigned int page_timestamp = 0;
static int now_page;
#endif
/********************声明这些全局变量*************************/
PAGE_EXTERN t_sPageRate Rate5Hz;
PAGE_EXTERN t_sPageRate Rate10Hz;
PAGE_EXTERN t_sPageRate Rate20Hz;
PAGE_EXTERN t_sPageRate Rate50Hz;
PAGE_EXTERN t_sPageRate Rate100Hz;
PAGE_EXTERN t_sPageRate Rate125Hz;
PAGE_EXTERN t_sPageRate Rate1000Hz;

void pageTick(void);
unsigned char pageExecuteRate(t_sPageRate *er);
void pageRegister(
    unsigned char pageID,
		unsigned char *pageTitle,
		unsigned char *pageIcon,
    t_fSteupCallback setupCallback,
    t_fLoopCallback loopCallback,
    t_fExitCallback exitCallback,
    t_fEventCallback eventCallback
);
void pageEventTransmit(unsigned char event);
void pageCloseCurrentPage(void);
void pageOpenCurrentPage(void);
void pageShift(unsigned char pageID, unsigned char condition);
void pageRun(void);
void pageSetStatus(unsigned char pageID, unsigned char status);
unsigned char pageGetStatus(void);
void pageInit(void);
#ifdef	__PAGE_C
void pageInit(void)
{
	PAGE_REG(page_watch);
	PAGE_REG(page_menu);
	PAGE_REG(page_timer);
	PAGE_REG(page_lsm6dsl);
	PAGE_REG(page_hdc2080);
	PAGE_REG(page_pedometer);
	PAGE_REG(page_bluetooth_serial);
	PAGE_REG(page_compass);
	PAGE_REG(page_setting);
	PAGE_REG(page_settime);
	PAGE_REG(page_alarm);
	PAGE_REG(page_flashlight);
	PAGE_REG(page_devinfo);
	PAGE_REG(page_ilink);
	PAGE_REG(page_trexrunner);
	PAGE_REG(page_snake);
	PAGE_REG(page_magcal);
	now_page = page_watch;
	pageOpenCurrentPage();
};
#endif

#endif

