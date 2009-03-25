/*!
	\file		SmartCardSigner.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

// SmartCardSigner.cpp : Implementation of CSmartCardSigner

#include "precompiled.h"
#include "SmartCardSigner.h"
#include "utility/pinDialog.h"

#include <algorithm>

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

BSTR CSmartCardSigner::readField(EstEidCard::RecordNames rec  ) {
	if (!m_cardData.size()) {
		try {
			EstEidCard card(m_mgr,0U);
			card.readPersonalData(m_cardData,EstEidCard::SURNAME,EstEidCard::COMMENT4);
		} catch (std::runtime_error &err) {
			return _bstr_t("").Detach();
			}
		}
	return _bstr_t(m_cardData[rec].c_str()).Detach();
	}

#define GET_METHOD(a,b) \
STDMETHODIMP CSmartCardSigner::get_##a(BSTR* pVal) {\
	*pVal = readField(EstEidCard::b); \
	return S_OK; \
}

GET_METHOD(lastName,SURNAME);
GET_METHOD(firstName,FIRSTNAME);
GET_METHOD(middleName,MIDDLENAME);
GET_METHOD(sex,SEX);
GET_METHOD(citizenship,CITIZEN);
GET_METHOD(birthDate,BIRTHDATE);
GET_METHOD(personalID,ID);
GET_METHOD(documentID,DOCUMENTID);
GET_METHOD(expiryDate,EXPIRY);
GET_METHOD(placeOfBirth,BIRTHPLACE);
GET_METHOD(issuedDate,ISSUEDATE);
GET_METHOD(residencePermit,RESIDENCEPERMIT);
GET_METHOD(comment1,COMMENT1);
GET_METHOD(comment2,COMMENT2);
GET_METHOD(comment3,COMMENT3);
GET_METHOD(comment4,COMMENT4);


inline std::string narrow(std::wstring source)
{
    std::string result(source.size(), char(0));
    typedef std::ctype<wchar_t> ctype_t;
    const ctype_t& ct = std::use_facet<ctype_t>(std::locale());
    ct.narrow(source.data(), source.data() + source.size(), '\u00B6',&(*result.begin()));
    return result;

} 
class widener {
public:
	WCHAR operator ( ) ( char& elem ) const 
	{ 
		return elem;
	}
	char operator() (WCHAR &elem) const
	{
		return (char)elem;
	}
};

STDMETHODIMP CSmartCardSigner::errMsg(LPCOLESTR err) {
	return Error(err,__uuidof(ISmartCardSigner));;
	}

struct pinDialogPriv_l {
	HINSTANCE m_hInst;
	WORD m_resourceID;
};

STDMETHODIMP CSmartCardSigner::sign(BSTR hashToBeSigned,VARIANT pCert)
{
	if (!pCert.uintVal )
		return errMsg(L"Parameter needs to be a cert object");

	_variant_t inputVar;
	if (V_ISBYREF(&pCert)) inputVar = pCert.pvarVal;
	else inputVar = &pCert;
	CComQIPtr<ISmartCardCertificate> iCert(inputVar);

	BSTR tmp;
	iCert->get_privateKeyContainer(&tmp);
	std::wstring keyContainer(tmp);
	SysFreeString(tmp);
	int method = 0;
	if (0 == keyContainer.compare(0,5,L"CARD:")) method = 1;
	if (0 == keyContainer.compare(0,4,L"CSP:"))  method = 2;
	if (0 == keyContainer.compare(0,7,L"PKCS11:"))method = 3;
	if (!method)
		return errMsg(L"Unknown method specified");
	if(0 == keyContainer.compare(6,7,L"EstEID:"))
		return errMsg(L"Unknown card specified");

/*	CREDUI_INFO ui_info = {
		sizeof(CREDUI_INFO),
		NULL,
		_T("pls enter auth"),
		_T("idcard authentication"),
		NULL
		};
	BOOL bSave = FALSE;
	TCHAR passPrompt[12] = {L'\0'};
	TCHAR uName[100] = _T("idCard User");
	DWORD ret = CredUIPromptForCredentials(&ui_info,_T("tgt_esteidcard"),
		NULL,0,
		uName,sizeof(uName),
		passPrompt,12,
		&bSave,
		CREDUI_FLAGS_EXCLUDE_CERTIFICATES | CREDUI_FLAGS_DO_NOT_PERSIST |
		CREDUI_FLAGS_GENERIC_CREDENTIALS | CREDUI_FLAGS_PASSWORD_ONLY_OK |
		CREDUI_FLAGS_KEEP_USERNAME
		);
*/
	
	pinDialogPriv_l params = { ATL::_AtlBaseModule.GetResourceInstance(),
		IDD_PIN_DIALOG_ENG
		};
	pinDialog dlg(&params,"Enter signature PIN");
	if (!dlg.doDialog())
		return errMsg(L"User cancelled");

	std::string pin = dlg.getPin();
	try {
		EstEidCard card(m_mgr,0U);
		if(0 == keyContainer.compare(11,2,L":0"))
			card.calcSignSHA1(ByteVec(0,0),EstEidCard::AUTH,pin);
		if(0 == keyContainer.compare(11,2,L":1"))
			card.calcSignSHA1(ByteVec(0,0),EstEidCard::SIGN,pin);
	} catch(std::runtime_error &e) {
		std::string errx(e.what());
		std::wstring errStr(errx.length() + 1,L' ');
		std::transform(errx.begin(),errx.end(),errStr.begin(),widener());
		return Error(errStr.c_str(),__uuidof(ISmartCardSigner));
		}

	return S_OK;
}

void CSmartCardSigner::getEstEIDCerts(CInterfaceList<ISmartCardCertificate> &list) {
	CComPtr<ISmartCardCertificate> cert0,cert1;
	cert0.CoCreateInstance(CLSID_SmartCardCertificate);
	cert1.CoCreateInstance(CLSID_SmartCardCertificate);
	if (!m_authCert.size()) {
		try {
		EstEidCard card(m_mgr,0U);
		m_authCert = card.getAuthCert();
		m_signCert = card.getSignCert();
		} catch (std::runtime_error &err) {return;}
		}
	cert0->_loadArrayFrom(L"CARD:EstEID:0", m_authCert.size(), &m_authCert[0] );
	cert1->_loadArrayFrom(L"CARD:EstEID:1", m_signCert.size(), &m_signCert[0] );
	list.AddTail(cert0);
	list.AddTail(cert1);
	}

STDMETHODIMP CSmartCardSigner::getCertificateList(BSTR* retVal)
{
	_bstr_t returnList;
	CInterfaceList<ISmartCardCertificate> certs;
	getEstEIDCerts(certs);
	// optionally load certs from other sources
	while( certs.GetCount() > 0 ) {
		BSTR val;
		certs.GetTail()->get_thumbPrint(&val);
		returnList += val + _bstr_t(",");
		SysFreeString(val);
		certs.RemoveTail();
		}

	*retVal = returnList.Detach();
	return S_OK;
}

STDMETHODIMP CSmartCardSigner::getCertificateByThumbprint(BSTR thumbPrint, VARIANT* retVal)
{
	_bstr_t thumb(thumbPrint);
	CInterfaceList<ISmartCardCertificate> certs;
	getEstEIDCerts(certs);
	while( certs.GetCount() > 0 ) {
		BSTR val;
		certs.GetTail()->get_thumbPrint(&val);
		if (thumb == _bstr_t(val)) {
			VariantInit(retVal);
			*retVal = _variant_t(certs.GetTail().Detach()).Detach();
			return S_OK;
			}
		certs.RemoveTail();
		}
	return S_OK;
}
