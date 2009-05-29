#pragma once

#ifdef WIN32
#include <atlbase.h>
#include <comutil.h>
#include <Mstask.h>
class ScheduledUpdateTask
{
	CComPtr<ITaskScheduler> pITS;
	CComPtr<ITask> pITask;
	CComPtr<IPersistFile> pIPersistFile;
public:
	ScheduledUpdateTask(void);
};
#else
class ScheduledUpdateTask
{
public:
	ScheduledUpdateTask(void);
};
#endif
