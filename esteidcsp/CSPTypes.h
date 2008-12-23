/*!
	\file		CSPTypes.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#pragma once

#include "CSPErrors.h"

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

struct CSPKeyContext;
struct CSPHashContext;

typedef std::vector<CSPHashContext *>::iterator CSPHashContextIter;
typedef std::vector<CSPKeyContext *>::iterator CSPKeyContextIter;

/// For searching handle values in vectors of pointers
template <typename A,class B>
class ptrSearchTerm {
	const A m_hvalue;
public:
	ptrSearchTerm(const A hvalue) : m_hvalue(hvalue) {}
	bool operator() (const B * value) {
		return *value == m_hvalue;
		}
};

/// Wrap a crypto context for proxied crypto
class WrapCsp {
	HCRYPTPROV m_hProv;
public:
	WrapCsp(tstring cspName,DWORD type) {
		CryptAcquireContext(&m_hProv,NULL, cspName.c_str(),type,CRYPT_VERIFYCONTEXT);
		}
	~WrapCsp() {
		CryptReleaseContext(m_hProv,0);
		}
	operator HCRYPTPROV() {return m_hProv;}
};

/// Wrap a hash context for proxied hashes
class WrapHash {
	HCRYPTHASH m_hHash;
public:
	WrapHash(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags) {
		CryptCreateHash(hProv,Algid,hKey,dwFlags,&m_hHash);
		}
	~WrapHash() {
		CryptDestroyHash(m_hHash);
		}
	operator HCRYPTHASH() {return m_hHash;}
};

/// Wrap a key context for proxied keys
class WrapKey {
	HCRYPTKEY m_hKey;
public:
	WrapKey() {}
	~WrapKey() {}
	operator HCRYPTKEY() {return m_hKey;}
};

/// Cryptcontext data
struct CSPContext {
	std::wostream &m_log;
	CSPContext(); 
	CSPContext(const CSPContext &c);
	~CSPContext();
	HCRYPTPROV m_provId;
	WrapCsp *m_wrapCsp;
	tstring m_containerName;
	DWORD m_flags;
	bool m_verifyContext;
	bool m_newKeyset;
	bool m_machineKeyset;
	bool m_deleteKeyset;
	bool m_silent;
	BOOL (*FuncReturnhWnd)(DWORD *phWnd);
	bool operator==(const HCRYPTPROV prov) const;

	std::vector<CSPHashContext *> m_hashes;
	std::vector<CSPKeyContext *> m_keys;
	std::vector<std::string> m_containers;
	std::vector<std::string> m_containersEnum;

	CSPHashContextIter findHashContext(HCRYPTHASH hProv);
	CSPKeyContextIter findKeyContext(HCRYPTKEY hKey);

	virtual CSPHashContext * createHashContext();
	virtual CSPKeyContext * createKeyContext();
};

/// Key context data
struct CSPKeyContext {
	HCRYPTKEY m_keyId;
	ALG_ID m_algId;
	DWORD m_blockLen;
	DWORD m_permissions;
	DWORD m_flags;
	DWORD m_keySpec;
	std::vector<BYTE> m_certificateBlob;
	WrapKey *m_wrapKey;
	virtual void setPubkeySpec(DWORD dwKeySpec) {m_keySpec = dwKeySpec;}
	CSPKeyContext() : m_wrapKey(NULL) {}
	~CSPKeyContext() {if (m_wrapKey) delete m_wrapKey;}
	bool operator==(const HCRYPTKEY) const;
};

/// Hash context data
struct CSPHashContext {
	HCRYPTHASH m_hashId;
	ALG_ID m_algId;
	WrapHash *m_wrapHash;
	CSPHashContext() : m_wrapHash(NULL) {}
	~CSPHashContext() { if (m_wrapHash) delete m_wrapHash;}
	bool operator==(const HCRYPTHASH) const;
	virtual std::vector<BYTE> sign(std::vector<BYTE> &) {return std::vector<BYTE>();}
};

/// Class that wraps function return code, for reliable tracing
class retType {
	BOOL m_value;
	std::string m_functionName;
public:
	retType(std::string functionName) : m_functionName(functionName),m_value(FALSE) {
		//log funcentry
		}
	~retType() {
		//log funcexit, value
		}
	void SetOk() {m_value = TRUE;}
	operator BOOL() const {return m_value;}
	void logReturn(std::runtime_error &) {}
};

/// Wraps LPBYTE/DWORD byte arrays, does size checks
struct packData {
	LPBYTE m_pbData,m_src;
	DWORD *m_pcbDataLen,m_originalSz;
	packData(LPBYTE pbData,DWORD *pcbDataLen) : 
		m_pbData(pbData),m_pcbDataLen(pcbDataLen),m_originalSz(*pcbDataLen) {}
	void copyData() {
		if (m_pbData) {
			if (m_originalSz < *m_pcbDataLen) throw err_bufferTooSmall();
			memcpy(m_pbData,m_src,*m_pcbDataLen);
			}
		}
	template<class T>
	void setValue(T a) {
		*m_pcbDataLen = sizeof(a);
		m_src = (LPBYTE)&a;
		copyData();
		}
	void setValue(tstring a) {
		*m_pcbDataLen = (DWORD) (a.length() * sizeof(TCHAR)) + 1;
		m_src = (LPBYTE)a.c_str();
		copyData();
		}
	void setValue(std::string a) {
		*m_pcbDataLen = (DWORD) a.length() + 1;
		m_src = (LPBYTE)a.c_str();
		copyData();
		}
	void setValue(std::vector<BYTE> &a) {
		*m_pcbDataLen = (DWORD)a.size();
		if (a.size()) m_src = &a[0];
		copyData();
		}
};

/// Checks given flags for CSP entry points, throws errors for unknown flags
struct flagCheck {
	DWORD m_flags;
	flagCheck(DWORD flags,bool isFinal = false) : m_flags(flags) {
		if (isFinal) final();
		}
	void check(DWORD mask,bool &refBool) {
		refBool = ((mask & m_flags) == mask);
		m_flags &= ~mask;
		}
	void final() { 
		if (m_flags) throw err_badFlags();
		}
	~flagCheck() {/*final();*/}
};
