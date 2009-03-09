// SmartCardSigner.h : Declaration of the CSmartCardSigner

#pragma once
#include "resource.h"       // main symbols

#include "EstEIDSigningPluginBHO_i.h"
#include "_ISmartCardSignerEvents_CP.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#include <atlwin.h> //CWindowImpl
#include <objsafe.h> //IObjectSafetyImpl
#include "CardMonitorThread.h"
#include <cardlib/common.h>
#include <cardlib/EstEidCard.h>
#include <cardlib/SmartCardManager.h>

// CSmartCardSigner

class ATL_NO_VTABLE CSmartCardSigner :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSmartCardSigner, &CLSID_SmartCardSigner>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CSmartCardSigner>,
	public CProxy_ISmartCardSignerEvents<CSmartCardSigner>,
	public IObjectWithSiteImpl<CSmartCardSigner>,
	public IObjectSafetyImpl<CSmartCardSigner, INTERFACESAFE_FOR_UNTRUSTED_CALLER |
         INTERFACESAFE_FOR_UNTRUSTED_DATA >,
	public IProvideClassInfo2Impl<&CLSID_SmartCardSigner,&IID_ISmartCardSigner>,
	public CWindowImpl<CSmartCardSigner>, //window to receive 2nd thread notifications
	public IDispatchImpl<ISmartCardSigner, &IID_ISmartCardSigner, &LIBID_EstEIDSigningPluginBHOLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSmartCardSigner() : m_monitorThread(*this)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SMARTCARDSIGNER)


BEGIN_COM_MAP(CSmartCardSigner)
	COM_INTERFACE_ENTRY(ISmartCardSigner)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IObjectSafety)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CSmartCardSigner)
	CONNECTION_POINT_ENTRY(__uuidof(_ISmartCardSignerEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CSmartCardSigner)
END_MSG_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		RECT rect = {0,0,1,1};
		HWND hwnd = Create( NULL, rect, _T("CSmartCardSignerAtlWindow"), WS_POPUP);
		if (!hwnd) return HRESULT_FROM_WIN32(GetLastError());
		return S_OK;
	}

	void FinalRelease()
	{
		if (m_hWnd != NULL) DestroyWindow();
	}

public:

	STDMETHOD(sayHello)(BSTR helloTag);
private:
	CardMonitorThread m_monitorThread;
	BSTR readField(EstEidCard::RecordNames rec);
	SmartCardManager m_mgr;
	std::vector<std::string> cardData;
public:
	STDMETHOD(get_lastName)(BSTR* pVal);
	STDMETHOD(get_firstName)(BSTR* pVal);
	STDMETHOD(get_middleName)(BSTR* pVal);
	STDMETHOD(get_sex)(BSTR* pVal);
	STDMETHOD(get_citizenship)(BSTR* pVal);
	STDMETHOD(get_birthDate)(BSTR* pVal);
	STDMETHOD(get_personalID)(BSTR* pVal);
	STDMETHOD(get_documentID)(BSTR* pVal);
	STDMETHOD(get_expiryDate)(BSTR* pVal);
	STDMETHOD(get_placeOfBirth)(BSTR* pVal);
	STDMETHOD(get_issuedDate)(BSTR* pVal);
	STDMETHOD(get_residencePermit)(BSTR* pVal);
	STDMETHOD(get_comment1)(BSTR* pVal);
	STDMETHOD(get_comment2)(BSTR* pVal);
	STDMETHOD(get_comment3)(BSTR* pVal);
	STDMETHOD(get_comment4)(BSTR* pVal);
	STDMETHOD(sign)(BSTR hashToBeSigned);
};

OBJECT_ENTRY_AUTO(__uuidof(SmartCardSigner), CSmartCardSigner)
