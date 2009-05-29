#pragma once

#ifdef WIN32
#include <atlbase.h>
#include <comutil.h>
#include <Mstask.h>
#include <string>

class ScheduledUpdateTask
{
	CComPtr<ITaskScheduler> pITS;
	CComQIPtr<ITask> pITask;
	CComQIPtr<IPersistFile> pIPersistFile;
public:
	ScheduledUpdateTask(std::wstring name);
	void configure();
};
#else
class ScheduledUpdateTask
{
public:
	ScheduledUpdateTask(void);
};
#endif
