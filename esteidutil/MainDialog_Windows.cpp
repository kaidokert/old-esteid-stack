/*!
	\file		MainDialog_Windows.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "MainDialog.h"
#include "AppSetting.h"
#include "cardlib/PCSCManager.h"
#include "cardlib/EstEidCard.h"
#include <wx/filename.h>
#include "wx/mstream.h"
#include <fstream>

#ifdef __WXMSW__
#include <wx/msw/private.h> //for wxGetInstance()
#include <wx/strconv.h>

#define ENCODING (X509_ASN_ENCODING | PKCS_7_ASN_ENCODING)

struct wrapCertStore {
	HCERTSTORE store;
	wrapCertStore(const char * name) {
		store = CertOpenStore(CERT_STORE_PROV_SYSTEM_A,X509_ASN_ENCODING,0,
			CERT_SYSTEM_STORE_CURRENT_USER,name);
		if (!store) 
			throw std::runtime_error("Unable to open certificate store");
		}
	~wrapCertStore() {
		CertCloseStore(store,0);
		}
	bool findCert(PCCERT_CONTEXT cert,PCCERT_CONTEXT &existing) {
		existing = CertFindCertificateInStore(store, ENCODING,
			0,CERT_FIND_SUBJECT_CERT,cert->pCertInfo ,NULL);
		if (existing) return true;
		return false;
		}
	operator HCERTSTORE() const {return store;}
};
struct wrapCertContext {
	PCCERT_CONTEXT context;
	wrapCertContext(ByteVec &certBytes) {
		context = CertCreateCertificateContext(ENCODING
			,&certBytes[0],DWORD(certBytes.size()));
		if (!context) throw std::runtime_error("invalid certificate");
		}
	~wrapCertContext() {
		CertFreeCertificateContext(context);
		}
	operator PCCERT_CONTEXT() const {return context;}
	};

void addCert(HCERTSTORE store,ByteVec &cert,wxString friendlyName,
			 wxString container,DWORD keySpec) {
	PCCERT_CONTEXT pCertContext = NULL;
	
	BOOL code = CertAddEncodedCertificateToStore(store,X509_ASN_ENCODING ,
		&cert[0],(DWORD)cert.size(),
		CERT_STORE_ADD_REPLACE_EXISTING,&pCertContext);
	if (!code) return;

	size_t outLen;
	wxWCharBuffer buf(
		wxConvCurrent->cMB2WC(friendlyName.ToAscii() ,friendlyName.Len() + 1,&outLen));
	CRYPT_DATA_BLOB DataBlob = {DWORD(outLen + 1) * 2 , (BYTE *)buf.data()};
	code = CertSetCertificateContextProperty(pCertContext,
		CERT_FRIENDLY_NAME_PROP_ID,0,&DataBlob);

	wxWCharBuffer contBuf(wxConvCurrent->cMB2WC(container.ToAscii()));
	CRYPT_KEY_PROV_INFO KeyProvInfo = {
		contBuf.data(),L"EstEID Card CSP",PROV_RSA_FULL,0,0,NULL,keySpec};
	code = CertSetCertificateContextProperty(pCertContext,
		CERT_KEY_PROV_INFO_PROP_ID,0,&KeyProvInfo);
	if (keySpec == AT_SIGNATURE) { // limit usages
		unsigned char asnEncodedUsage[] =  // ask no questions ..
				"\x30\x38\x06\x0A\x2B\x06\x01\x04\x01\x82\x37\x0A\x05\x01\x06\x0A"
				"\x2B\x06\x01\x04\x01\x82\x37\x0A\x03\x02\x06\x0A\x2B\x06\x01\x04"
				"\x01\x82\x37\x0A\x03\x01\x06\x08\x2B\x06\x01\x05\x05\x07\x03\x08"
				"\x06\x08\x2B\x06\x01\x05\x05\x07\x03\x03";
		CRYPT_DATA_BLOB asnBlob = {sizeof(asnEncodedUsage)-1,asnEncodedUsage};
		CertSetCertificateContextProperty(pCertContext,CERT_ENHKEY_USAGE_PROP_ID,0,&asnBlob);
	}
	CertFreeCertificateContext(pCertContext);
}

void MainDialog::OnCertRegister(wxCommandEvent&  event) 
{
	if (!mCardManager) return;
	EstEidCard card(*mCardManager);
	if (!FindCard(card)) return;
	SetStatusText(_("Reading certificate.."));

	wxString name = mPersonalData[EstEidCard::SURNAME] + _T(",") + mPersonalData[EstEidCard::FIRSTNAME]; 
	try {
		wrapCertStore store("MY");

		addCert(store,card.getAuthCert(),wxString(_T("Authentication ")) + name,
			wxString(_T("AUT_") + m_cardId),AT_KEYEXCHANGE);
		addCert(store,card.getSignCert(),wxString(_T("Signature ")) + name,
		wxString(_T("SIG_") + m_cardId),AT_SIGNATURE);

		doPopupInfo(_("Certificates registered"));
		SetStatusText(_T(""));
	} catch(std::runtime_error &err) {
		doShowError(err);
		}
}
#if 1
void MainDialog::doShowCert(ByteVec &certBytes) {
	wxString wxfile = wxFileName::CreateTempFileName(_T("cer") + m_cardId);
	{
	std::ofstream strm(wxfile.ToAscii(),std::ios::binary);
	strm.write((const char*)&certBytes[0],std::streamsize(certBytes.size()));
	}
	DynamicLibrary lib("CryptExt");
	int (CALLBACK *fnPtr)(HWND, HINSTANCE, const CHAR * , int) = 
		(int (CALLBACK *)(HWND, HINSTANCE, const CHAR * , int)) 
		lib.getProc("CryptExtOpenCER");
	(*fnPtr)(NULL,wxGetInstance(),wxfile.ToAscii() ,SW_NORMAL);
	}
#endif
void MainDialog::doCheckCertRegistration(ByteVec &certBytes) {
	wrapCertStore store("MY");
	wrapCertContext cert(certBytes);

	PCCERT_CONTEXT clientCert;
	if (!store.findCert(cert,clientCert)) {
		wxString prompt = _("Certificates are not registered. Register now ?");
		int answer = wxMessageBox(
			prompt ,getAppName(),wxYES_NO | wxCENTRE |wxICON_QUESTION);
		if (answer == wxYES )
			OnCertRegister(wxCommandEvent());
		}
	}

void MainDialog::doWindowsCertCheck(ByteVec &certBytes,void ** ref) {
	doCheckCertRegistration(certBytes);

	wrapCertStore store("MY");
	wrapCertContext cert(certBytes);
	PCCERT_CONTEXT clientCert;
	if (!store.findCert(cert,clientCert)) 
		throw std::runtime_error("Certificate not found");
	*ref = (void*)clientCert;
	}
#endif
