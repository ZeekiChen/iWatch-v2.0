#include "iWatch.h"

void main()
{
	iWatchInit();
	while(1)
	{
		FeedWatchDog();
		og_run();
		pageRun();
		iWatchStatusHandle();	//×´Ì¬´¦Àí
	}
}
