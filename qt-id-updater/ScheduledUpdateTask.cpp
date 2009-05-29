#include "ScheduledUpdateTask.h"

#pragma comment(lib,"Mstask.lib")
ScheduledUpdateTask::ScheduledUpdateTask(std::wstring name)
{
	pITS.CoCreateInstance(CLSID_CTaskScheduler,NULL);
	CComPtr<IUnknown> iUnk;
	pITS->Activate(name.c_str(),IID_ITask,&iUnk);
	if (!iUnk) {
		pITS->NewWorkItem(name.c_str(),CLSID_CTask,IID_ITask,&iUnk);
		if (!iUnk) 
			return;
		}
	pITask = iUnk;
	pIPersistFile = iUnk;
}


void ScheduledUpdateTask::configure() {
	pITask->SetComment(L"we didz it");
	pIPersistFile->Save(NULL,TRUE);
}