#include "precompiled.h"
#include "WorkerClass.h"
#include "cardlib/PCSCManager.h"
#include "cardlib/EstEidCard.h"
#include <vector>
using std::vector;

struct WorkerPriv {
	PCSCManager mgr;
	vector<bool> cardPresent;
	int readerCount;
	CWindow *win;
};

WorkerClass::WorkerClass(CWindow *parent) : pimpl(new WorkerPriv)
{
	pimpl->win = parent;
	m_thread.Initialize();
	m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_thread.AddHandle(m_hEvent, this, NULL);

	int readers = pimpl->mgr.getReaderCount();
	pimpl->cardPresent.resize(readers);

	::SetEvent(m_hEvent);
}

WorkerClass::~WorkerClass(void) 
{
	m_thread.RemoveHandle(m_hEvent);
	m_thread.Shutdown();
	delete pimpl;
	pimpl = 0;
}

HRESULT WorkerClass::Execute(DWORD_PTR dwParam, HANDLE hObject) {
	Sleep(500);
	EstEidCard card(pimpl->mgr);
	for (unsigned int i = 0; i < pimpl->cardPresent.size() ; i++ ) {
		if (card.isInReader(i) && !pimpl->cardPresent[i]) {
			ATLTRACE("card inserted\n");
			pimpl->cardPresent[i] = true;
			if (pimpl->win->IsWindow())
				pimpl->win->PostMessage(WM_CARD_INSERTED,i);
			}
		if (!card.isInReader(i) && pimpl->cardPresent[i]) {
			ATLTRACE("card removed\n");
			pimpl->cardPresent[i] = false;
			if (pimpl->win->IsWindow())
				pimpl->win->PostMessage(WM_CARD_REMOVED,i);
			}
		}
	return S_OK;
}

HRESULT WorkerClass::CloseHandle(HANDLE hHandle) {
	::CloseHandle(hHandle);
	return S_OK;
}

