/*!
	\file		Csp.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#include "precompiled.h"
#include "Csp.h"
#include "RegKey.h"
#include "CSPTypes.h"
#include "utility.h"

Csp::Csp(HMODULE module,TCHAR *cspName) : 
	m_module(module),m_cspName(cspName),m_nextHandle(1)
{
}

Csp::~Csp(void)
{
}

HRESULT Csp::DllRegisterServer(void) {
	try {
		RegKey providerKey(HKEY_LOCAL_MACHINE,
			_T("SOFTWARE\\Microsoft\\Cryptography\\Defaults\\Provider"));
		RegKey cspKey(providerKey,m_cspName,KEY_WRITE);

		std::vector<TCHAR> modPath(MAX_PATH,'\0');
		DWORD sz = GetModuleFileName(m_module,&modPath[0],(DWORD)modPath.size());
		modPath.resize(sz+1);
		cspKey.setString(_T("Image Path"),&modPath[0]);
		cspKey.setInt(_T("Type"),getCSPType());
		cspKey.setInt(_T("SigInFile"),0);
	} catch(std::runtime_error &) {
		return S_FALSE;
		}
	return S_OK;
	}


HRESULT Csp::DllUnregisterServer(void) {
	try {
		RegKey providerKey(HKEY_LOCAL_MACHINE,
			_T("SOFTWARE\\Microsoft\\Cryptography\\Defaults\\Provider"),KEY_WRITE);
		providerKey.deleteKey(m_cspName);
	} catch(std::runtime_error &) {
		return S_FALSE;
		}
	return S_OK;
	}


DWORD Csp::getNextHandle() {
	return ++m_nextHandle;
}

Csp::CSPContextIter Csp::findContext(HCRYPTPROV hProv) {
	ptrSearchTerm<HCRYPTPROV,CSPContext> term(hProv);
	CSPContextIter it = 
		find_if(m_contexts.begin(),m_contexts.end(),term);
	if (it == m_contexts.end()) throw err_badContext();
	return it;
	}

BOOL Csp::CPAcquireContext(
		OUT HCRYPTPROV *phProv,
		IN  LPCSTR szContainer,
		IN  DWORD dwFlags,
		IN  PVTableProvStruc pVTable){
	retType ret("CPAcquireContext");
	try {
		CSPContext* ctx = createCSPContext();
		flagCheck f(dwFlags);
		f.check(CRYPT_VERIFYCONTEXT,ctx->m_verifyContext);
		f.check(CRYPT_NEWKEYSET,ctx->m_newKeyset);
		f.check(CRYPT_DELETEKEYSET,ctx->m_deleteKeyset);
		f.check(CRYPT_MACHINE_KEYSET,ctx->m_machineKeyset);
		f.check(CRYPT_SILENT,ctx->m_silent);
		f.final();
		ctx->m_provId = getNextHandle();
		std::string container(szContainer == 0 ? "" : szContainer);
		Widen<wchar_t > to_wstring;
		ctx->m_containerName = to_wstring(container) ;
		m_contexts.push_back(ctx);
		*phProv = ctx->m_provId;
		ret.SetOk();
	} catch(std::runtime_error &err) {
		ret.logReturn(err);
	}
	return ret;
}

BOOL Csp::CPAcquireContextW(
		OUT HCRYPTPROV *phProv,
		IN  LPCWSTR szContainer,
		IN  DWORD dwFlags,
		IN  PVTableProvStrucW pVTable){
	retType ret("CPAcquireContextW");
	try {

		ret.SetOk();
	} catch(std::runtime_error &) {
	}
	return ret;
}

BOOL Csp::CPReleaseContext(
		IN  HCRYPTPROV hProv,
		IN  DWORD dwFlags){
	retType ret("CPReleaseContext");
	try {
		m_contexts.erase(findContext(hProv));
		ret.SetOk();
	} catch(std::runtime_error &err) {
		ret.logReturn(err);
	}
	return ret;
}

BOOL Csp::CPSetProvParam(
		IN  HCRYPTPROV hProv,
		IN  DWORD dwParam,
		IN  CONST BYTE *pbData,
		IN  DWORD dwFlags){
	retType ret("CPSetProvParam");
	try {
		CSPContextIter it = findContext(hProv);
		switch(dwParam) {
			case PP_KEYEXCHANGE_PIN: {
				}
			case PP_SIGNATURE_PIN: {
				}
			}

		ret.SetOk();
	} catch(std::runtime_error &) {
	}
	return ret;
}

BOOL Csp::CPGetProvParam(
		IN  HCRYPTPROV hProv,
		IN  DWORD dwParam,
		OUT LPBYTE pbData,
		IN OUT LPDWORD pcbDataLen,
		IN  DWORD dwFlags){
	retType ret("CPGetProvParam");
	try {
		bool first;
		packData dat(pbData,pcbDataLen);
		CSPContext *it = *findContext(hProv);
		flagCheck f(dwFlags);
		f.check(CRYPT_FIRST,first);
		f.final();

		switch (dwParam) {
			case PP_NAME :
				dat.setValue(m_cspName);break;
			case PP_VERSION:
				dat.setValue(DWORD(0x00000200));break;
			case PP_PROVTYPE:
				dat.setValue(DWORD(PROV_RSA_FULL));break;
			case PP_IMPTYPE :
				dat.setValue(DWORD(CRYPT_IMPL_MIXED));break;
			case PP_CONTAINER :
			case PP_UNIQUE_CONTAINER :
			case PP_ENUMCONTAINERS:
				if (dwFlags & CRYPT_FIRST) {
					it->m_containersEnum.resize(it->m_containers.size());
					copy(it->m_containers.begin(),it->m_containers.end(),
						it->m_containersEnum.begin());
					}
				if (!it->m_containersEnum.empty()) {
					dat.setValue(it->m_containersEnum.back());
					it->m_containersEnum.pop_back();
				}
				else 
					throw err_noMoreItems();
				break;
			case PP_ENUMALGS :
				if (first) {
					BOOL success = TRUE;
					PROV_ENUMALGS alg;
					DWORD flag = CRYPT_FIRST;
					while(success){
						DWORD sz = sizeof(alg);
						success = CryptGetProvParam(
							*it->m_wrapCsp,PP_ENUMALGS,(LPBYTE)&alg,&sz,flag);
						if (success) m_enumAlgs.push_back(alg);
						flag = 0;
					}
					}
				else {
					if (m_enumAlgs.empty()) throw err_noMoreItems();
					dat.setValue(m_enumAlgs.back());
					m_enumAlgs.pop_back();
					}
				break;
			case PP_ENUMALGS_EX :
				if (first) {
					BOOL success = TRUE;
					PROV_ENUMALGS_EX  alg;
					DWORD flag = CRYPT_FIRST;
					while(success){
						DWORD sz = sizeof(alg);
						success = CryptGetProvParam(
							*it->m_wrapCsp,PP_ENUMALGS_EX,(LPBYTE)&alg,&sz,flag);
						if (success) m_enumAlgsEx.push_back(alg);
						flag = 0;
					}
					}
				else {
					if (m_enumAlgs.empty()) throw err_noMoreItems();
					dat.setValue(m_enumAlgs.back());
					m_enumAlgs.pop_back();
					}
				break;
			case PP_SIG_KEYSIZE_INC :
			case PP_KEYX_KEYSIZE_INC:
			default:
				throw err_badType();
				break;
			}

		ret.SetOk();
	} catch(std::runtime_error &err) {
		ret.logReturn(err);
	}
	return ret;
}

BOOL Csp::CPEncrypt(
		IN  HCRYPTPROV hProv,
		IN  HCRYPTKEY hKey,
		IN  HCRYPTHASH hHash,
		IN  BOOL fFinal,
		IN  DWORD dwFlags,
		IN OUT LPBYTE pbData,
		IN OUT LPDWORD pcbDataLen,
		IN  DWORD cbBufLen){
	retType ret("CPEncrypt");
	try {
		CSPContextIter it = findContext(hProv);
		ret.SetOk();
	} catch(std::runtime_error &) {
	}
	return ret;
}

BOOL Csp::CPDecrypt(
		IN  HCRYPTPROV hProv,
		IN  HCRYPTKEY hKey,
		IN  HCRYPTHASH hHash,
		IN  BOOL fFinal,
		IN  DWORD dwFlags,
		IN OUT LPBYTE pbData,
		IN OUT LPDWORD pcbDataLen){
	retType ret("CPDecrypt");
	try {
		CSPContextIter it = findContext(hProv);
		ret.SetOk();
	} catch(std::runtime_error &) {
	}
	return ret;
}

BOOL Csp::CPGenRandom(
		IN  HCRYPTPROV hProv,
		IN  DWORD cbLen,
		OUT LPBYTE pbBuffer){
	retType ret("CPGenRandom");
	try {
		CSPContext *it = *findContext(hProv);
		CryptGenRandom(*it->m_wrapCsp,cbLen,pbBuffer);
		ret.SetOk();
	} catch(std::runtime_error &) {
	}
	return ret;
}

