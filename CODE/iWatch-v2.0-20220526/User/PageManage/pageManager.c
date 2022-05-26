#define	__PAGE_C
#include "pageManager.h"


/**
  * @brief  �ṩҳ��ʱ�ӣ�ÿ1�������һ��
  * @param  ��
  * @retval ��
  */
void pageTick(void)
{
	page_timestamp ++;
}
/**
  * @brief  ��ҳ��ѭ����ͨ����ѯ�˺����Ľ�������ָ����ִ��Ƶ��
  * @param  ִ��Ƶ����صĽṹ��ָ��
  * @retval ��
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
  * @brief  ע��һ������ҳ�棬����һ����ʼ��������ѭ���������˳��������¼�����
  * @param  pageID: ҳ����
	*	@param	pageText: ҳ������ָ��
	*	@param	pageIcon: ҳ��ͼ���ָ��
  * @param  setupCallback: ��ʼ�������ص�
  * @param  loopCallback: ѭ�������ص�
  * @param  exitCallback: �˳������ص�
  * @param  eventCallback: �¼������ص�
  * @retval ��
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
  * @brief  ҳ���¼�����
  * @param  event: �¼����
  * @retval ��
  */
void pageEventTransmit(unsigned char event)
{
	/*���¼����ݵ���ǰҳ��*/
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
  * @brief  ҳ���л�
  * @param  pageID��ҳ���
	* @retval ��
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