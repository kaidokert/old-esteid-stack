/*!
	\file		CspTypes.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#include "precompiled.h"
#include "CSPTypes.h"
#include "CSPErrors.h"
#include <algorithm>

CSPContext::CSPContext(): m_log(std::wclog) {
	m_wrapCsp =  new WrapCsp(MS_ENHANCED_PROV,PROV_RSA_FULL);
	}

CSPContext::CSPContext(const CSPContext &c) : m_log(std::wclog) {
	m_provId=c.m_provId;
	m_provId=c.m_provId;
	m_flags=c.m_flags;
	m_verifyContext=c.m_verifyContext;
	m_newKeyset=c.m_newKeyset;
	m_machineKeyset=c.m_machineKeyset;
	m_deleteKeyset=c.m_deleteKeyset;
	m_silent=c.m_silent;
	m_wrapCsp = new WrapCsp(MS_ENHANCED_PROV,PROV_RSA_FULL);
	}

CSPContext::~CSPContext() {
	delete m_wrapCsp;
}

CSPHashContext * CSPContext::createHashContext() {
	return new CSPHashContext();
	}

CSPKeyContext * CSPContext::createKeyContext() {
	return new CSPKeyContext();
	}

bool CSPContext::operator==(const HCRYPTPROV prov) const {
	return m_provId == prov;
	}

CSPHashContextIter CSPContext::findHashContext(HCRYPTHASH hProv) { 
	ptrSearchTerm<HCRYPTHASH,CSPHashContext> term(hProv);
	CSPHashContextIter it = 
		find_if(m_hashes.begin(),m_hashes.end(),term);
	if (it == m_hashes.end()) throw err_badHash();
	return it;	
}
CSPKeyContextIter CSPContext::findKeyContext(HCRYPTKEY hKey) {
	ptrSearchTerm<HCRYPTKEY,CSPKeyContext> term(hKey);
	CSPKeyContextIter it = 
		find_if(m_keys.begin(),m_keys.end(), term);
	if (it == m_keys.end()) throw err_badKey();
	return it;
}

bool CSPKeyContext::operator==(const HCRYPTKEY keyId) const {
	return m_keyId == keyId;
}

bool CSPHashContext::operator==(const HCRYPTHASH hashId) const {
	return m_hashId == hashId;
}

#if 0
void packData::copyData() {
		if (m_pbData) {
			if (m_originalSz < *m_pcbDataLen) throw err_bufferTooSmall();
			memcpy(m_pbData,m_src,*m_pcbDataLen);
			}
		}
template<class T>
void packData::setValue(T a) {
	*m_pcbDataLen = sizeof(a);
	m_src = (LPBYTE)&a;
	copyData();
	}
void packData::setValue(tstring a) {
	*m_pcbDataLen = (DWORD) (a.length() * sizeof(TCHAR)) + 1;
	m_src = (LPBYTE)a.c_str();
	copyData();
	}
void packData::setValue(std::string a) {
	*m_pcbDataLen = (DWORD) a.length() + 1;
	m_src = (LPBYTE)a.c_str();
	copyData();
	}
void packData::setValue(std::vector<BYTE> &a) {
	*m_pbcData = a.size();
	if (a.size()) m_src = &a[0];
	copyData();
	}
#endif