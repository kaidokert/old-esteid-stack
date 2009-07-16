// card.h : Declaration of the Ccard

#pragma once
#include "resource.h"       // main symbols

#include "idutil2.h"
#include <atlwin.h>
#include <objsafe.h>
#include <atlctl.h>
#include "_IcardEvents_CP.h"
#include "WorkerClass.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


#define WM_CARD_INSERTED  (WM_USER + 101)
#define WM_CARD_REMOVED  (WM_USER + 102)
// Ccard

class ATL_NO_VTABLE Ccard :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Ccard, &CLSID_card>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<Ccard>,
	public CProxy_IcardEvents<Ccard>,
	public IObjectWithSiteImpl<Ccard>,
	public IObjectSafetyImpl<Ccard, INTERFACESAFE_FOR_UNTRUSTED_CALLER |
         INTERFACESAFE_FOR_UNTRUSTED_DATA >,
	public IProvideClassInfo2Impl<&CLSID_card,&IID_Icard>,
	public CWindowImpl<Ccard>, //window to receive 2nd thread notifications
	public IDispatchImpl<Icard, &IID_Icard, &LIBID_idutil2Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
	WorkerClass workThread;
	long m_selectedReader;
	VARIANT readField(int );
public:
	Ccard() : workThread(this), m_selectedReader(-1)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CARD)

/*BEGIN_CATEGORY_MAP(Ccard)
	IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
END_CATEGORY_MAP()
*/
BEGIN_COM_MAP(Ccard)
	COM_INTERFACE_ENTRY(Icard)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IObjectSafety)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(Ccard)
	CONNECTION_POINT_ENTRY(__uuidof(_IcardEvents))
END_CONNECTION_POINT_MAP()
BEGIN_MSG_MAP(Ccard)
	MESSAGE_HANDLER(WM_CARD_INSERTED, OnCardInserted)
	MESSAGE_HANDLER(WM_CARD_REMOVED, OnCardRemoved)
END_MSG_MAP()
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	LRESULT OnCardInserted(UINT uMsg, WPARAM wParam,
		LPARAM lParam, BOOL& bHandled)
	{
		Fire_CardWasInserted((int)wParam);
		return 0;
	}
	LRESULT OnCardRemoved(UINT uMsg, WPARAM wParam,
		LPARAM lParam, BOOL& bHandled)
	{
		Fire_CardWasRemoved((int)wParam);
		return 0;
	}
	HRESULT FinalConstruct()
	{
		RECT rect = {0,10,0,10};
		HWND hwnd = Create( NULL, rect, _T("CcardAtlWindow"), WS_POPUP);
		if (!hwnd) return HRESULT_FROM_WIN32(GetLastError());
		return S_OK;
	}

	void FinalRelease()
	{
		if (m_hWnd != NULL) DestroyWindow();
	}

public:

	STDMETHOD(get_familyName)(VARIANT* pVal);
	STDMETHOD(get_firstName1)(VARIANT* pVal);
	STDMETHOD(get_firstName2)(VARIANT* pVal);
	STDMETHOD(get_sex)(VARIANT* pVal);
	STDMETHOD(ReadCard)(void);
	STDMETHOD(get_nationality)(VARIANT* pVal);
	STDMETHOD(get_birthDate)(VARIANT* pVal);
	STDMETHOD(get_personalCode)(VARIANT* pVal);
	STDMETHOD(get_documentNumber)(VARIANT* pVal);
	STDMETHOD(get_expiry)(VARIANT* pVal);
	STDMETHOD(get_birthPlace)(VARIANT* pVal);
	STDMETHOD(get_issued)(VARIANT* pVal);
	STDMETHOD(get_residencePermitType)(VARIANT* pVal);
	STDMETHOD(get_comment1)(VARIANT* pVal);
	STDMETHOD(get_comment2)(VARIANT* pVal);
	STDMETHOD(get_comment3)(VARIANT* pVal);
	STDMETHOD(get_comment4)(VARIANT* pVal);
	STDMETHOD(get_selectReader)(LONG* pVal);
	STDMETHOD(put_selectReader)(LONG newVal);
	STDMETHOD(getReaders)(VARIANT* arrReaders);
};

OBJECT_ENTRY_AUTO(__uuidof(card), Ccard)
