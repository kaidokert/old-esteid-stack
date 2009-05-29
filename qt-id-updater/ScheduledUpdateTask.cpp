#include "ScheduledUpdateTask.h"

#pragma comment(lib,"Mstask.lib")
ScheduledUpdateTask::ScheduledUpdateTask(void)
{
	pITS.CoCreateInstance(CLSID_CTaskScheduler,NULL);
	CComPtr<IEnumWorkItems> pp;
	pITS->Enum(&pp);
}

