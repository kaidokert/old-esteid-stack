// SmartCardRemoval.cpp : Implementation of WinMain


#include "precompiled.h"
#include "resource.h"
#include "SmartCardRemoval.h"

#include <stdio.h>
#include <fstream>
#include <utility/monitorThread.h>

#pragma comment(lib,"crypt32")

using std::endl;

class CSmartCardRemovalModule : 
		public CAtlServiceModuleT< CSmartCardRemovalModule, IDS_SERVICENAME >,
		public monitorObserver
{
	mutexObj lock;
	std::ofstream log;
public :
	CSmartCardRemovalModule() : lock("lock") {
		CHAR path[MAX_PATH];
		GetTempPathA(sizeof(path),path);
		strcat(path,"smartcardremoval.run.log");
		log.open(path);
		}

	virtual void onEvent(monitorEvent eType,int param) {
		switch(eType) {
			case READERS_CHANGED:
				log << "readers changed" << std::endl;break;
			case NO_CARDS_LEFT:
				log << "no esteid cards" << std::endl;
				log << "launch cleanup" << std::endl;
				removeCerts();
				break;
			}
		}

	DECLARE_LIBID(LIBID_SmartCardRemovalLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SMARTCARDREMOVAL, "{DEAE87CA-A84D-4F75-BC47-721D7F0F7848}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for 
		// your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.

		return S_OK;
	}
	void RunMessageLoop()
	{
		monitorThread monitor(*this,lock);
		monitor.start();
		MSG msg;
		log << "running .. " << std::endl;
		while (GetMessage(&msg, 0, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	struct certStore { //wrap store handle
		HCERTSTORE hStore;
	public:
		certStore(LPCTSTR storeName) {
			hStore = CertOpenStore(CERT_STORE_PROV_SYSTEM,0, (HCRYPTPROV)NULL,
					CERT_SYSTEM_STORE_CURRENT_USER /*| CERT_STORE_READONLY_FLAG  */
					| CERT_STORE_OPEN_EXISTING_FLAG,storeName);
			if (!hStore) throw std::runtime_error("cant open certstore");
		};
		~certStore() {CertCloseStore(hStore,0);};
		operator HCERTSTORE() const {return hStore;}
		};

	bool CertFromCSP(PCCERT_CONTEXT ctx) {
		std::vector<BYTE> buf;
		DWORD sz = -1;
		if (TRUE != CertGetCertificateContextProperty(ctx,
			CERT_KEY_PROV_INFO_PROP_ID,NULL,&sz)) return false;
		buf.resize(sz);
		if (TRUE != CertGetCertificateContextProperty(ctx,
			CERT_KEY_PROV_INFO_PROP_ID,&buf[0],&sz)) return false;
		CRYPT_KEY_PROV_INFO *info = (CRYPT_KEY_PROV_INFO *) &buf[0];
		if (info->pwszProvName == NULL) return false;
		if (std::wstring(info->pwszProvName) == std::wstring(L"EstEID NewCard CSP")) {
			log << "found EstEID NewCard CSP cert" << endl;
			return true;
			}
		}

	void removeCerts() {
		log << "enuming certs" << endl;
		try {
			certStore my(_T("MY"));
			PCCERT_CONTEXT ctx = NULL;
			while (ctx = CertEnumCertificatesInStore(my,ctx) ) {
				log << "checking if Esteid cert" << endl;
				if (!CertFromCSP(ctx)) continue;

				PCCERT_CONTEXT dup = CertDuplicateCertificateContext(ctx);
				log << "erasing ctx .." << endl;
				BOOL suc = CertDeleteCertificateFromStore(dup);
				if (!suc) {
					DWORD gle = GetLastError();
					log << "we got an error on CertDeleteCertificateFromStore, " << gle << endl;
					}
				}
		} catch(...) {return;}
		}
};

CSmartCardRemovalModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

