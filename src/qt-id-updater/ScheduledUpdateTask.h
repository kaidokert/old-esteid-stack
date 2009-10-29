#pragma once

#if defined(_WIN32)
#include <atlbase.h>
#include <comutil.h>
#include <Mstask.h>
#endif

#include <string>

class ScheduledUpdateTask
{
#if defined(_WIN32)
	CComPtr<ITaskScheduler> pITS;
	CComQIPtr<ITask> pITask;
	CComQIPtr<IPersistFile> pIPersistFile;
#endif
	std::wstring m_command;
	std::wstring m_name;
public:
	enum Interval {
		DAILY,
		WEEKLY,
		MONTHLY,
		NEVER
	} ;
	ScheduledUpdateTask(std::wstring command,std::wstring name);
	bool configure(Interval interval,bool autoupdate);
	bool remove();
};

