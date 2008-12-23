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
#include "RegKey.h"
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

std::string getPin(PCWSTR prompt) {
	CREDUI_INFO ui_info = {
		sizeof(CREDUI_INFO),
		NULL,
		prompt,
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
		cachedAuthPin = getPin(_T("Enter authentication PIN"));
	return cachedAuthPin;
	}

std::string EstEidContext::getSignPin() {
	return getPin(_T("Enter signature PIN"));
	}

EstEidContext::~EstEidContext() {
}

CspEstEid::CspEstEid(HMODULE module,TCHAR *name) : Csp(module,name)
{
}

CspEstEid::~CspEstEid(void)
{
}

struct CspEstEid::cardSetup CspEstEid::cards[3] = {
	{_T("EstEID National ID Card"),
		_T("3b000000000000000000004573744549442076657220312e3000"),
		_T("ff00000000000000000000ffffffffffffffffffffffffffff00")},
	{_T("EstEID National ID Card (MultOS)"),
		_T("3b000000000000000000  4573744549442076657220312e3000"),
		_T("ff000000000000000000  ffffffffffffffffffffffffffff00")},
	{_T("EstEID National ID Card (short ATR)"),
		_T("3b000000              4573744549442076657220312e30"),
		_T("ff000000              ffffffffffffffffffffffffffff")}
	};

HRESULT CspEstEid::DllUnregisterServer(void) {
	if (Csp::DllUnregisterServer() != S_OK) return FALSE;
	RegKey scKey(HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Cryptography\\Calais\\SmartCards"));
	scKey.deleteKey(cards[0].name);
	scKey.deleteKey(cards[1].name);
	scKey.deleteKey(cards[2].name);
	return S_OK;
	}

HRESULT CspEstEid::DllRegisterServer(void) {
	if (Csp::DllRegisterServer() != S_OK) return FALSE;
	regCard(cards[0]);
	regCard(cards[1]);
	regCard(cards[2]);
	return S_OK;
	}

unsigned char x2b(wchar_t c) {
	if (c >= '0' && c <= '9') return c - '0';
	else if (c >= 'a') return c - 'a' + 10;
	return c - 'A' + 10;
}

std::vector<BYTE> hex2bin(tstring hex) {
	std::vector<BYTE> ret;
	for (size_t i = 0; i < hex.length(); ) {
		BYTE c;
		c =  x2b(hex[i++]) << 4;
		c |= x2b(hex[i++]);
		if (c!=249) ret.push_back(c);
		}
	return ret;
	}

void CspEstEid::regCard(CspEstEid::cardSetup &card) {
	RegKey scKey(HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Cryptography\\Calais\\SmartCards"));
	RegKey subKey(scKey,card.name,KEY_WRITE);
	subKey.setString(_T("Crypto Provider"),m_cspName);
	std::vector<BYTE> atr(hex2bin(card.atr)),atrMask(hex2bin(card.atrMask));
	subKey.setBin(_T("ATR"),atr);
	subKey.setBin(_T("ATRMask"),atrMask);
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
	if (m_algId == CALG_SHA1 ) {
		EstEidCard card(m_ctx->getMgr());
		if (!m_ctx->findCard(card)) throw err_noKey();
		ret = card.calcSignSHA1(hash, EstEidCard::SIGN, m_ctx->getSignPin() ) ;
		reverse(ret.begin(),ret.end());		
		}
	return ret;
}
