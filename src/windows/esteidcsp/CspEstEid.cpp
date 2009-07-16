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

#include "cardlib/EstEidCard.h"
#include "cardlib/SmartCardManager.h"
#include "utility.h"
#include "utility/pinDialog.h"
#include "utility/threadObj.h"
#include "RegKey.h"
#include <iostream>
#include <wincrypt.h>
#include <algorithm>

class EstEidContext : public CSPContext {
	std::string cachedAuthPin;
protected:
	HMODULE m_module;
	SmartCardManager cardMgr;
public:
	EstEidContext(logger &log,HMODULE module);
	~EstEidContext();
	SmartCardManager & getMgr() { return cardMgr;};
	CSPKeyContext * EstEidContext::createKeyContext();
	CSPHashContext * EstEidContext::createHashContext();
	std::vector<BYTE> doSigning(ALG_ID m_algId,std::vector<BYTE> &hash);

	bool findCard(EstEidCard &card) {
		for(uint i = 0;i < cardMgr.getReaderCount();i++) {
			if (!m_readerName.empty() && m_readerName != cardMgr.getReaderName(i)) continue;
			if (!card.isInReader(i)) continue;
			card.connect(i);
			const std::string name = card.readCardID();
			if ( m_containerName.compare(4,name.length(),name))
				continue;
			return true;
			}
		return false;
		}
	virtual void loadUserCerts() {
		EstEidCard card(cardMgr);
		if (!findCard(card)) throw err_badContext();
		std::string name = card.readCardID();
//		getContainer(std::string("AUT_") + name)
		for(std::vector<cardLocation>::iterator i = m_containers.begin();
			i!=m_containers.end(); i++) {
				if (i->containerName == "AUT_" + name)
					i->cert = card.getAuthCert();
				if (i->containerName == "SIG_" + name)
					i->cert = card.getSignCert();
			}
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
		m_keySpec = dwKeySpec;
		EstEidCard card(m_ctx->getMgr());
		if (!m_ctx->findCard(card)) throw err_noKey();
		if (m_keySpec == AT_KEYEXCHANGE)
			m_certificateBlob = card.getAuthCert();
		else 
			m_certificateBlob = card.getSignCert();
		}
	};

EstEidContext::EstEidContext(logger &log,HMODULE module) : 
	CSPContext(log),m_module(module) {
	try {
		EstEidCard card(cardMgr);
		for(uint i = 0;i < cardMgr.getReaderCount();i++) {
			m_log << cardMgr.getReaderName(i) << std::endl;
			if (card.isInReader(i)) {
				m_log << "esteid card in here" << std::endl;
				card.connect(i);
				const std::string name = card.readCardID();
				const std::string rdr  = cardMgr.getReaderName(i);
				m_log << "card id:" << name << " reader:" << rdr <<  std::endl;
				m_containers.push_back(cardLocation(std::string("SIG_") + name ,  rdr , AT_SIGNATURE ) );
				m_containers.push_back(cardLocation(std::string("AUT_") + name ,  rdr , AT_KEYEXCHANGE) );
				}
			}
	} catch(std::runtime_error &err) {
		m_log << "exc : " << err.what() << std::endl;
		}
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
	return new EstEidContext(m_log,m_module);
	}

CSPKeyContext * EstEidContext::createKeyContext() {
	return new EsteidKeyContext(this);
	}

CSPHashContext * EstEidContext::createHashContext() {
	return new EsteidHashContext(this);
	}

struct pinDialogPriv_l {
	HINSTANCE m_hInst;
	WORD m_resourceID;
};

struct sign_op : public pinOpInterface {
	ByteVec & hash,& result;
	ALG_ID m_algId;
	sign_op(ByteVec &_hash,ByteVec &_result,
		EstEidCard &card,mutexObj &mutex,ALG_ID algid) : 
		pinOpInterface(card,mutex),hash(_hash),result(_result),m_algId(algid) {}
	void call(EstEidCard &card,const std::string &pin,EstEidCard::KeyType key) {
		if (m_algId == CALG_SSL3_SHAMD5 )
			result = card.calcSSL(hash ) ;
		if (m_algId == CALG_SHA1 ) 
			result = card.calcSignSHA1(hash, EstEidCard::SIGN ) ;
		}
};

std::vector<BYTE> EstEidContext::doSigning(ALG_ID m_algId,std::vector<BYTE> &hash) {
	std::vector<BYTE> ret;
	mutexObj dummy("dummy");

	pinDialogPriv_l params = { m_module,IDD_PIN_DIALOG_ENG};
	pinDialog dlg(&params,EstEidCard::AUTH);

	EstEidCard card(getMgr());
	if (!findCard(card)) throw err_noKey();
	sign_op operation(hash,ret,card,dummy,m_algId);
	dlg.doDialogInloop(operation,cachedAuthPin);

	reverse(ret.begin(),ret.end());		

	return ret;
}

std::vector<BYTE> EsteidHashContext::sign(std::vector<BYTE> &hash) {
	return m_ctx->doSigning(m_algId,hash);
}