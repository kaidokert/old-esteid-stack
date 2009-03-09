// SmartCardSigner.cpp : Implementation of CSmartCardSigner

#include "precompiled.h"
#include "SmartCardSigner.h"
#include <comutil.h>
#include <WinCred.h> //for credui

#pragma comment(lib,"comsuppw")
#pragma comment(lib,"credui")

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
	if (!cardData.size()) {
		EstEidCard card(m_mgr,0U);
		card.readPersonalData(cardData,EstEidCard::SURNAME,EstEidCard::COMMENT4);
		}
	return _bstr_t(cardData[rec].c_str()).Detach();
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

STDMETHODIMP CSmartCardSigner::sign(BSTR hashToBeSigned)
{
	CREDUI_INFO ui_info = {
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
	return S_OK;
}
