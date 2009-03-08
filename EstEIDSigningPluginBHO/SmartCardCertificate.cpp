// SmartCardCertificate.cpp : Implementation of CSmartCardCertificate

#include "stdafx.h"
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
