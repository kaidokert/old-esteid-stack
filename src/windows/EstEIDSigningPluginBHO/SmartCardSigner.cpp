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
#include "utility/converters.h"
#include "cardlib/SCError.h"
#include <algorithm>
#include "Setup.h"

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

STDMETHODIMP CSmartCardSigner::getVersion(BSTR *retVal) {
	*retVal = _bstr_t(PACKAGE_VERSION).Detach();
	return S_OK;
	}

STDMETHODIMP CSmartCardSigner::readField(EstEidCard::RecordNames rec ,BSTR* pVal ) {
	if (!m_cardData.size()) {
		mutexObjLocker _lock(criticalSection);
		int retries = 3;
		while(retries--) {
			try {
				EstEidCard card(m_mgr,m_selectedReader);
				card.readPersonalData(m_cardData,EstEidCard::SURNAME,EstEidCard::COMMENT4);
				break;
#if SCDEBUG
			} catch (SCError &cErr)  {
				if (retries && cErr.error == 0xff) //CTAPI sometimes fails
					continue;
				return errMsg("error");
#endif
			} catch (std::runtime_error &err) {
				return errMsg(err.what());
				}
			}
		}
	*pVal = _bstr_t(m_cardData[rec].c_str()).Detach();
	return S_OK;
	}

#define GET_METHOD(a,b) \
STDMETHODIMP CSmartCardSigner::get_##a(BSTR* pVal) {\
	return readField(EstEidCard::b,pVal); \
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

STDMETHODIMP CSmartCardSigner::errMsg(std::string err) {
	std::wstring errStr(err.length() + 1,L' ');
	std::transform(err.begin(),err.end(),errStr.begin(),widener());
	return Error(errStr.c_str(),__uuidof(ISmartCardSigner));
	}

struct pinDialogPriv_l {
	HINSTANCE m_hInst;
	WORD m_resourceID;
};

struct sign_op : public pinOpInterface {
	ByteVec & hash,& result;
	sign_op(ByteVec &_hash,ByteVec &_result,
		EstEidCard &card,mutexObj &mutex) : 
		pinOpInterface(card,mutex),hash(_hash),result(_result) {}
	void call(EstEidCard &card,const std::string &pin,EstEidCard::KeyType key) {
		result = card.calcSignSHA1(hash,key,pin);
		}
};

STDMETHODIMP CSmartCardSigner::sign(BSTR hashToBeSigned,IDispatch * pCert,BSTR* pVal)
{
	if (!pCert) 
		return errMsg(L"Second parameter must be a certificate");
	CComDispatchDriver iDisp(pCert);
	CComQIPtr<ISmartCardCertificate> iCert(iDisp);
	if (!iCert) 
		return errMsg(L"Second parameter must be a certificate");

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

	CW2A pszConverted(hashToBeSigned);
	ByteVec hash = fromHex(std::string(pszConverted));

	//FIXME: hardcoded sha1
	if (hash.size() != 0x14)
		return errMsg(L"Not a valid SHA1 hash");

	EstEidCard::KeyType key = (EstEidCard::KeyType) -1;
	if(0 == keyContainer.compare(11,2,L":0"))
		key = EstEidCard::AUTH;
	if(0 == keyContainer.compare(11,2,L":1"))
		key = EstEidCard::SIGN;

	pinDialogPriv_l params = { ATL::_AtlBaseModule.GetResourceInstance(),
		IDD_PIN_DIALOG_ENG
		};

	pinDialog dlg(&params,key);
	ByteVec result;
	try {
		EstEidCard card(m_mgr,m_selectedReader);
		sign_op operation(hash,result,card,criticalSection);
		std::string dummyCache;
		dlg.doDialogInloop(operation,dummyCache);
	} catch(std::exception &e) {
		return errMsg(e.what());
		}

	std::string strHex = toHex(result);
	*pVal = _bstr_t(strHex.c_str()).Detach();
	return S_OK;
}

void CSmartCardSigner::clearCaches() {
	m_authCert.clear();
	m_signCert.clear();
	m_cardData.clear();
	}

void CSmartCardSigner::getEstEIDCerts(CInterfaceList<ISmartCardCertificate> &list) {
	CComPtr<ISmartCardCertificate> cert0,cert1;
	cert0.CoCreateInstance(CLSID_SmartCardCertificate);
	cert1.CoCreateInstance(CLSID_SmartCardCertificate);
	if (!m_authCert.size()) {
		try {
			mutexObjLocker _lock(criticalSection);
			EstEidCard card(m_mgr,m_selectedReader);
			m_authCert = card.getAuthCert();
			m_signCert = card.getSignCert();
		} catch (std::exception &err) {
			std::string doh = err.what();
			throw;
			}
		}
	cert0->_loadArrayFrom(L"CARD:EstEID:0", (ULONG)m_authCert.size(), &m_authCert[0] );
	cert1->_loadArrayFrom(L"CARD:EstEID:1", (ULONG)m_signCert.size(), &m_signCert[0] );
	list.AddTail(cert0);
	list.AddTail(cert1);
	}

STDMETHODIMP CSmartCardSigner::getCertificateList(BSTR* retVal)
{
	_bstr_t returnList("");
	CInterfaceList<ISmartCardCertificate> certs;
	try {
		getEstEIDCerts(certs);
	} catch (std::exception &err) {
		return errMsg(err.what());
		}
	// optionally load certs from other sources
	while( certs.GetCount() > 0 ) {
		CComBSTR val("");
		certs.GetTail()->get_thumbPrint(&val);
		returnList += val + _bstr_t(",");
		SysFreeString(val);
		certs.RemoveTail();
		}

	*retVal = returnList.Detach();
	return S_OK;
}

STDMETHODIMP CSmartCardSigner::getCertificateByThumbprint(BSTR thumbPrint, IDispatch** retVal)
{
	_bstr_t thumb(thumbPrint);
	CInterfaceList<ISmartCardCertificate> certs;
	try {
		getEstEIDCerts(certs);
	} catch(std::exception &err) {
		return errMsg(err.what());
		}
	while( certs.GetCount() > 0 ) {
		BSTR val;
		certs.GetTail()->get_thumbPrint(&val);
		if (thumb == _bstr_t(val)) {
			*retVal = certs.GetTail().Detach();
			return S_OK;
			}
		certs.RemoveTail();
		}
	return S_OK;
}

STDMETHODIMP CSmartCardSigner::addEventListener(BSTR eventName,IDispatch *unk)
{
	CComQIPtr<IDispatch> disp(unk);
	if (S_OK != disp.GetIDOfName(L"call",NULL) )
		return errMsg(L"Second parameter is expected to be a function");
	if (CComBSTR(eventName) == "OnCardInserted")
		notifyCardInsert.AddTail(disp);
	if (CComBSTR(eventName) == "OnCardRemoved")
		notifyCardRemove.AddTail(disp);
	if (CComBSTR(eventName) == "OnReadersChanged")
		notifyReadersChanged.AddTail(disp);
	return S_OK;
}
STDMETHODIMP CSmartCardSigner::removeEventListener(BSTR eventName,IDispatch *unk) {
	return S_OK;
	}


void callAllFunctions(CInterfaceList<IDispatch> &list,WPARAM wParam) {
	POSITION myPos = list.GetHeadPosition();
	CComPtr<IDispatch> ptr;
	while( myPos )  {
		ptr = list.GetNext(myPos);
		_variant_t par_this("nothing"),par_one(wParam);
		ptr.Invoke2(L"call",&par_this,&par_one);
		} 
	}

LRESULT CSmartCardSigner::OnCardInserted(UINT uMsg, WPARAM wParam,
	LPARAM lParam, BOOL& bHandled) {
	if (m_selectedReader == wParam) 
		clearCaches();
	callAllFunctions(notifyCardInsert, wParam);
	Fire_CardWasInserted((int)wParam);
	return 0;
	}
LRESULT CSmartCardSigner::OnCardRemoved(UINT uMsg, WPARAM wParam,
	LPARAM lParam, BOOL& bHandled)
{
	if (m_selectedReader == wParam) 
		clearCaches();
	callAllFunctions(notifyCardRemove, wParam);
	Fire_CardWasRemoved((int)wParam);
	return 0;
}
LRESULT CSmartCardSigner::OnReadersChanged(UINT uMsg, WPARAM wParam,
	LPARAM lParam, BOOL& bHandled)
{
	clearCaches();
	callAllFunctions(notifyReadersChanged, wParam);
	Fire_ReadersChanged((int)wParam);
	return 0;
}

STDMETHODIMP CSmartCardSigner::getReaderName(USHORT readerNum,BSTR* retVal)
{
	_bstr_t returnVal("");
	try {
		returnVal = _bstr_t(m_mgr.getReaderName(readerNum).c_str());
	} catch (std::runtime_error &err) {
		return errMsg(err.what());
		}
	*retVal = returnVal.Detach();
	return S_OK;
}

STDMETHODIMP CSmartCardSigner::getReaders(BSTR* retVal)
{
	_bstr_t returnList("");
	try {
		mutexObjLocker _lock(criticalSection);
		uint rdrs = m_mgr.getReaderCount(true);
		for(uint i = 0; i < rdrs; i++) {
			returnList+= _bstr_t(m_mgr.getReaderName(i).c_str()) + _bstr_t(",");
			}
	} catch (std::runtime_error &err) {
		return errMsg(err.what());
		}
	*retVal = returnList.Detach();
	return S_OK;
}


STDMETHODIMP CSmartCardSigner::get_selectedReader(SHORT* pVal)
{
	*pVal = m_selectedReader;
	return S_OK;
}

STDMETHODIMP CSmartCardSigner::put_selectedReader(SHORT newVal)
{
	if (m_selectedReader != newVal)
		clearCaches();
	m_selectedReader = newVal;
	return S_OK;
}
