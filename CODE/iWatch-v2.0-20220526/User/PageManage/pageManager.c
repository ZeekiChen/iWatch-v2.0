#define	__PAGE_C
#include "pageManager.h"


/**
  * @brief  提供页面时钟，每1毫秒调用一次
  * @param  无
  * @retval 无
  */
void pageTick(void)
{
	page_timestamp ++;
}
/**
  * @brief  在页面循环中通过查询此函数的结果来获得指定的执行频率
  * @param  执行频率相关的结构体指针
  * @retval 无
  */
unsigned char pageExecuteRate(t_sPageRate *er)
{
	if(page_timestamp - er->last_timestamp >= er->executeT)
	{
		er->last_timestamp = page_timestamp;
		return 1;
	}
	else
		return 0;
}
/**
  * @brief  注册一个基本页面，包含一个初始化函数，循环函数，退出函数，事件函数
  * @param  pageID: 页面编号
	*	@param	pageText: 页面标题的指针
	*	@param	pageIcon: 页面图标的指针
  * @param  setupCallback: 初始化函数回调
  * @param  loopCallback: 循环函数回调
  * @param  exitCallback: 退出函数回调
  * @param  eventCallback: 事件函数回调
  * @retval 无
  */
void pageRegister(
    unsigned char pageID,
		unsigned char *pageTitle,
		unsigned char *pageIcon,
    t_fSteupCallback setupCallback,
    t_fLoopCallback loopCallback,
    t_fExitCallback exitCallback,
    t_fEventCallback eventCallback
)
{
		pagelist[pageID].Title = pageTitle;
		pagelist[pageID].Icon = pageIcon;
		pagelist[pageID].status = PAGE_IDLE;
    pagelist[pageID].SetupCallback = setupCallback;
    pagelist[pageID].LoopCallback = loopCallback;
    pagelist[pageID].ExitCallback = exitCallback;
    pagelist[pageID].EventCallback = eventCallback;
}
/**
  * @brief  页面事件传递
  * @param  event: 事件编号
  * @retval 无
  */
void pageEventTransmit(unsigned char event)
{
	/*将事件传递到当前页面*/
  if((pagelist[now_page].EventCallback != 0) && (pagelist[now_page].status == PAGE_RUN))
		pagelist[now_page].EventCallback(event);
}
void pageOpenCurrentPage()
{
	if(pagelist[now_page].status == PAGE_IDLE)
	{
		pagelist[now_page].SetupCallback(PAGE_ENTER);
		pagelist[now_page].status = PAGE_RUN;
	}
}
void pageCloseCurrentPage()
{
	if(pagelist[now_page].status == PAGE_RUN)
		pagelist[now_page].ExitCallback(PAGE_RETURN);
}
/**
  * @brief  页面切换
  * @param  pageID：页面号
	* @retval 无
  */
void pageShift(unsigned char pageID, unsigned char condition)
{
	if((now_page != pageID) && (pagelist[pageID].status == PAGE_IDLE))
	{
		if(pagelist[now_page].ExitCallback != 0)
		{
			pagelist[now_page].ExitCallback(condition);
		}
		now_page = pageID;
		if(pagelist[now_page].SetupCallback != 0)
		{
			pagelist[now_page].SetupCallback(condition);
			pagelist[now_page].status = PAGE_RUN;
		}
	}
}
void pageRun(void)
{
	pagelist[now_page].LoopCallback();
}
void pageSetStatus(unsigned char pageID, unsigned char status)
{
	pagelist[pageID].status = status;
}
unsigned char pageGetStatus(void)
{
	return pagelist[now_page].status;
}