/*!
	\file		CardMonitorThread.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#pragma once

#include <atlutil.h> //IWorkerThreadClient

struct WorkerPriv;

#define WM_CARD_INSERTED   (WM_USER + 101)
#define WM_CARD_REMOVED    (WM_USER + 102)
#define WM_READERS_CHANGED (WM_USER + 103)

class CardMonitorThread :
	public IWorkerThreadClient
{
	HANDLE m_hEvent;
	CWorkerThread<Win32ThreadTraits> m_thread;
	CComCriticalSection &m_criticalSection;
	WorkerPriv* d;
public:
	CardMonitorThread(CWindow &parent,CComCriticalSection &criticalSection);
	virtual ~CardMonitorThread(void);
private:
    HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject);
	HRESULT CloseHandle(HANDLE hHandle);
};
