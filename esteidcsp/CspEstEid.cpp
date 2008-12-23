/*!
	\file		CspEstEid.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#include "precompiled.h"
#include "CspEstEid.h"

#include "EstEidCard.h"
#include "SmartCardManager.h"
#include "utility.h"
#include <iostream>
#include <wincrypt.h>
#include <algorithm>

#include <WinCred.h>
#pragma comment(lib,"Credui")

class EstEidContext : public CSPContext {
	std::string cachedAuthPin;
protected:
	SmartCardManager cardMgr;
public:
	EstEidContext();
	~EstEidContext();
	SmartCardManager & getMgr() { return cardMgr;};
	CSPKeyContext * EstEidContext::createKeyContext();
	CSPHashContext * EstEidContext::createHashContext();
	std::string getAuthPin();
	std::string getSignPin();
	bool findCard(EstEidCard &card) {
		Widen<wchar_t> to_wstring;
		for(uint i = 0;i < cardMgr.getReaderCount();i++) {
			if (!card.isInReader(i)) continue;
			card.connect(i);
			const std::string name = card.readCardID();
			if (m_containerName != to_wstring(name)) continue;
			return true;
			}
		return false;
		}
	};

class EsteidHashContext : public CSPHashContext {
	EstEidContext *m_ctx;
public:
	EsteidHashContext(EstEidContext *ctx) : m_ctx(ctx) {
		}
	virtual std::vector<BYTE> sign(std::vector<BYTE> &);
	};

class EsteidKeyContext : public CSPKeyContext {
	EstEidContext *m_ctx;
public:
	EsteidKeyContext(EstEidContext *ctx) : m_ctx(ctx) {
		m_algId = CALG_RSA_KEYX;
		}
	void setPubkeySpec(DWORD dwKeySpec) {
		Widen<wchar_t> to_wstring;
		m_keySpec = dwKeySpec;
		EstEidCard card(m_ctx->getMgr());
		if (!m_ctx->findCard(card)) throw err_noKey();
		if (m_keySpec == AT_KEYEXCHANGE)
			m_certificateBlob = card.getAuthCert();
		else 
			m_certificateBlob = card.getSignCert();
		}
	};

EstEidContext::EstEidContext() {
	try {
		Widen<wchar_t> to_wstring;
		EstEidCard card(cardMgr);
		for(uint i = 0;i < cardMgr.getReaderCount();i++) {
			m_log << to_wstring(cardMgr.getReaderName(i)) << std::endl;
			if (card.isInReader(i)) {
				m_log << "esteid card in here" << std::endl;
				card.connect(i);
				const std::string name = card.readCardID();
				m_log << "card id:" <<  to_wstring(name) << std::endl;
				m_containers.push_back(name);
				}
			}
	} catch(std::runtime_error &err) {
		m_log << "exc : " << err.what() << std::endl;
		}
}

std::string getPin() {
	CREDUI_INFO ui_info = {
		sizeof(CREDUI_INFO),
		NULL,
		_T("enter authentication pin"),
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
	std::wstring tmp(passPrompt);
	return narrow(tmp);
	}

std::string EstEidContext::getAuthPin() {
	if (!cachedAuthPin.length())
		cachedAuthPin = getPin();
	return cachedAuthPin;
	}

std::string EstEidContext::getSignPin() {
	return getPin();
	}

EstEidContext::~EstEidContext() {
}

CspEstEid::CspEstEid(HMODULE module,TCHAR *name) : Csp(module,name)
{
}

CspEstEid::~CspEstEid(void)
{
}

CSPContext * CspEstEid::createCSPContext() {
	return new EstEidContext();}

CSPKeyContext * EstEidContext::createKeyContext() {
	return new EsteidKeyContext(this);
	}

CSPHashContext * EstEidContext::createHashContext() {
	return new EsteidHashContext(this);
	}

std::vector<BYTE> EsteidHashContext::sign(std::vector<BYTE> &hash) {
	std::vector<BYTE> ret;
	if (m_algId == CALG_SSL3_SHAMD5 ) {
		EstEidCard card(m_ctx->getMgr());
		if (!m_ctx->findCard(card)) throw err_noKey();
		ret = card.calcSSL(hash, m_ctx->getAuthPin() ) ;
		reverse(ret.begin(),ret.end());		
		}
	return ret;
}
