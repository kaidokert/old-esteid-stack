// card.cpp : Implementation of Ccard

#include "precompiled.h"
#include "card.h"
#include <comdef.h>
#include <comutil.h> //for _bstr_t
#include <atlsafe.h> //bring in CComSafeArray
#include "cardlib/PCSCManager.h"
#include "cardlib/EstEidCard.h"

// Ccard

STDMETHODIMP Ccard::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_Icard
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP Ccard::ReadCard(void)
{
	return S_OK;
}

VARIANT Ccard::readField(int rec) {
	_variant_t var("");
	try {
		PCSCManager mgr;
		EstEidCard card(mgr);
		if (m_selectedReader == -1 )
			for (unsigned int i = 0 ; i < mgr.getReaderCount(); i++ ) 
				if (card.isInReader(i)) { m_selectedReader = i;break; }
		card.connect(m_selectedReader);
		std::vector<std::string> data;
		card.readPersonalData(data,rec,rec);
		var = _bstr_t(data[rec].c_str());
	} catch (std::runtime_error &) {
	}
	return var.Detach();
	}

STDMETHODIMP Ccard::get_familyName(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::SURNAME);
	return S_OK;
}

STDMETHODIMP Ccard::get_firstName1(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::FIRSTNAME);
	return S_OK;
}

STDMETHODIMP Ccard::get_firstName2(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::MIDDLENAME);
	return S_OK;
}

STDMETHODIMP Ccard::get_sex(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::SEX);
	return S_OK;
}

STDMETHODIMP Ccard::get_nationality(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::CITIZEN);
	return S_OK;
}

STDMETHODIMP Ccard::get_birthDate(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::BIRTHDATE);
	return S_OK;
}

STDMETHODIMP Ccard::get_personalCode(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::ID);
	return S_OK;
}

STDMETHODIMP Ccard::get_documentNumber(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::DOCUMENTID);
	return S_OK;
}

STDMETHODIMP Ccard::get_expiry(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::EXPIRY);
	return S_OK;
}

STDMETHODIMP Ccard::get_birthPlace(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::BIRTHPLACE);
	return S_OK;
}

STDMETHODIMP Ccard::get_issued(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::ISSUEDATE);
	return S_OK;
}

STDMETHODIMP Ccard::get_residencePermitType(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::RESIDENCEPERMIT);
	return S_OK;
}

STDMETHODIMP Ccard::get_comment1(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::COMMENT1);
	return S_OK;
}

STDMETHODIMP Ccard::get_comment2(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::COMMENT2);
	return S_OK;
}

STDMETHODIMP Ccard::get_comment3(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::COMMENT3);
	return S_OK;
}

STDMETHODIMP Ccard::get_comment4(VARIANT* pVal)
{
	*pVal = readField(EstEidCard::COMMENT4);
	return S_OK;
}

STDMETHODIMP Ccard::get_selectReader(LONG* pVal)
{
	*pVal = m_selectedReader;
	return S_OK;
}

STDMETHODIMP Ccard::put_selectReader(LONG newVal)
{
	m_selectedReader = newVal;
	return S_OK;
}

STDMETHODIMP Ccard::getReaders(VARIANT* arrReaders)
{
	CComSafeArray<VARIANT > saOut;
	PCSCManager mgr;
	for(unsigned int i = 0;i < mgr.getReaderCount(); i++ ) {
		CComBSTR tmp(mgr.getReaderName(i).c_str());
		saOut.Add(CComVariant(tmp));
		}

	VariantInit(arrReaders);
	V_VT(arrReaders) = VT_ARRAY | VT_VARIANT;
	V_ARRAY(arrReaders)= saOut.Detach();
	return S_OK;
}
