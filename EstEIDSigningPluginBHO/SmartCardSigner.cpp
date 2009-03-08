// SmartCardSigner.cpp : Implementation of CSmartCardSigner

#include "stdafx.h"
#include "SmartCardSigner.h"
#include <comutil.h>

#pragma comment(lib,"comsuppw")

// CSmartCardSigner

STDMETHODIMP CSmartCardSigner::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ISmartCardSigner
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CSmartCardSigner::sayHello(BSTR helloTag)
{
	// TODO: Add your implementation code here
	_bstr_t hello(helloTag);
	::MessageBox(GetForegroundWindow(),helloTag,L"CSmartCardSigner caption",MB_OK);
	return S_OK;
}
