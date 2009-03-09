// SmartCardCertificate.cpp : Implementation of CSmartCardCertificate

#include "precompiled.h"
#include "SmartCardCertificate.h"


// CSmartCardCertificate

STDMETHODIMP CSmartCardCertificate::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ISmartCardCertificate
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CSmartCardCertificate::get_CN(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardCertificate::get_validFrom(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardCertificate::get_validTo(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardCertificate::get_issuerCN(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardCertificate::get_keyUsage(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardCertificate::get_cert(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSmartCardCertificate::get_certID(BSTR* pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}
