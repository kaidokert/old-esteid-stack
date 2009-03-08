// SmartCardSigner.cpp : Implementation of CSmartCardSigner

#include "stdafx.h"
#include "SmartCardSigner.h"
#include <comutil.h>

#include <cardlib/common.h>
#include <cardlib/EstEidCard.h>

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

STDMETHODIMP CSmartCardSigner::get_lastName(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_firstName(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_middleName(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_sex(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_citizenship(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_birthDate(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_personalID(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_documentID(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_expiryDate(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_placeOfBirth(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_issuedDate(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_residencePermit(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_comment1(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_comment2(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_comment3(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardSigner::get_comment4(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}
