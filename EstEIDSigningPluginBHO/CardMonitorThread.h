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

class CardMonitorThread :
	public IWorkerThreadClient
{
	CWorkerThread<Win32ThreadTraits> m_thread;
public:
	CardMonitorThread(CWindow &parent);
	virtual ~CardMonitorThread(void);
private:
    HRESULT Execute(DWORD_PTR dwParam, HANDLE hObject);
	HRESULT CloseHandle(HANDLE hHandle);
};
