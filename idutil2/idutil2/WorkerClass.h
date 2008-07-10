#pragma once
#include <atlsiface.h>
#include <atlutil.h>
#include <atlwin.h>

#define WM_CARD_INSERTED  (WM_USER + 101)
#define WM_CARD_REMOVED   (WM_USER + 102)

struct WorkerPriv;

class WorkerClass : public IWorkerThreadClient
{
	HANDLE m_hEvent;
	CWorkerThread<Win32ThreadTraits> m_thread;
	WorkerPriv* pimpl;
public:
	WorkerClass(CWindow *parent);
	~WorkerClass(void);
private:
    HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject);
	HRESULT CloseHandle(HANDLE hHandle);
};
