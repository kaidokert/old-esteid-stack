#include "precompiled.h"
#include "CardMonitorThread.h"

CardMonitorThread::CardMonitorThread(CWindow &parent)
{
	m_thread.Initialize();
}

CardMonitorThread::~CardMonitorThread(void)
{
	m_thread.Shutdown();
}

HRESULT CardMonitorThread::Execute(DWORD_PTR dwParam, HANDLE hObject) {
	Sleep(500);
	return S_OK;
	}

HRESULT CardMonitorThread::CloseHandle(HANDLE hHandle) {
	::CloseHandle(hHandle);
	return S_OK;
}
