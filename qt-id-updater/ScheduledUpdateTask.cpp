#include "ScheduledUpdateTask.h"

#pragma comment(lib,"Mstask.lib")
ScheduledUpdateTask::ScheduledUpdateTask(void)
{
	pITS.CoCreateInstance(CLSID_CTaskScheduler,NULL);
	CComPtr<IEnumWorkItems> pp;
	pITS->Enum(&pp);
	LPWSTR *lpwszNames = 0;
	ULONG numFetched;
	pp->Next(1000,&lpwszNames,&numFetched);
	
	CComPtr<IUnknown> iUnk;
	pITS->Activate(lpwszNames[0],IID_ITask,&iUnk);
	CComQIPtr<ITask> iTask(iUnk);
	iTask->SetComment(L"we didz it");
}

