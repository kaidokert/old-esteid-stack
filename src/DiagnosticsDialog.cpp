/*!
	\file		DiagnosticsDialog.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "DiagnosticsDialog.h"
#include "MainDialog.h"
#include "cardlib/DynamicLibrary.h"
#include "cardlib/PCSCManager.h"
#include "cardlib/CTAPIManager.h"
#include "cardlib/EstEidCard.h"
#include "utility/pkcs11module.h"
#include <wx/utils.h>
#include <fstream>

#define ENDL _T("\r\n")
wxString getCpuType();
wxString getMemoryMB();

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_MY_CUSTOM_COMMAND, 7777)
END_DECLARE_EVENT_TYPES()

BEGIN_EVENT_TABLE(DiagnosticsDialog,wxDialog)
	EVT_CUSTOM(wxEVT_MY_CUSTOM_COMMAND, wxID_ANY, DiagnosticsDialog::OnMyEvent)
	EVT_BUTTON(wxID_SAVE,DiagnosticsDialog::OnSaveToFile)
END_EVENT_TABLE()

void DiagnosticsDialog::OnSaveToFile( wxCommandEvent &event ) {
	wxFileDialog* SaveDialog = new wxFileDialog(
		this, _("Save to file"),wxEmptyString,_T("IDdiagnostics.txt"),
		_T("*.txt"), wxFD_SAVEFLAGS);
	if (SaveDialog->ShowModal() == wxID_OK ) {
		std::ofstream fs(SaveDialog->GetPath().ToAscii());
		wxString val = mDiagText->GetValue();
		wxCharBuffer data = val.mb_str(wxConvUTF8);
		fs << (char *) data.data();
		}
	}

void DiagnosticsDialog::OnMyEvent( wxEvent &event )
{
	doDiagnostics();
}

wxString Asc(std::string asciiStr) {
	return wxString::FromAscii(asciiStr.c_str());
	}

DiagnosticsDialog::DiagnosticsDialog(wxWindow *parent,wxString title)
	:wxDialog(parent,wxID_ANY,title,wxDefaultPosition,
		wxSize(460,220))
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	mDiagText = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	mDiagText->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );

	bSizer13->Add( mDiagText, 1, wxALL|wxEXPAND, 10 );

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );

	mBtnSave = new wxButton( this, wxID_SAVE, _("Save to file"), wxDefaultPosition, wxDefaultSize, 0 );
	mBtnSave->SetMinSize( wxSize( 130,-1 ) );

	bSizer14->Add( mBtnSave, 0, wxALL, 5 );

	mBtnClose = new wxButton( this, wxID_OK, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	mBtnClose->SetMinSize( wxSize( 90,-1 ) );

	bSizer14->Add( mBtnClose, 0, wxALL, 5 );

	bSizer13->Add( bSizer14, 0, wxEXPAND, 5 );

	this->SetSizer( bSizer13 );
	this->Layout();

    wxCommandEvent eventCustom(wxEVT_MY_CUSTOM_COMMAND);
    wxPostEvent(this, eventCustom);
	Update();
}

DiagnosticsDialog::~DiagnosticsDialog(void)
{
}

void DiagnosticsDialog::addLine(wxString line) {
	mDiagText->AppendText(line + ENDL);
	Update();
	wxSafeYield();
	}

void DiagnosticsDialog::doDiagnostics() {
#ifdef __WXMSW__
	listFileVersion(_T("Advanced Win32 Base API"),_T("advapi32"));
	listFileVersion(_T("Crypto API32"),_T("crypt32"));
	listFileVersion(_T("MS Smart Card API"),_T("winscard"));
	listFileVersion(_T("EstEID CSP"),_T("esteid"));
	m_haveCardmodule = listFileVersion(_T("EstEID cardmodule"),_T("esteidcm"));
#else
	listFileVersion(_T("PC/SC Lite"),_T("pcsclite"));
#endif
	listFileVersion(_T("OpenSmartcard PKCS#11"),_T("opensc"),2);
	addLine(::wxGetOsDescription() );
	addLine(_T("Processor ") + getCpuType() );
	addLine(_T("Memory ") + getMemoryMB() );
	PCSCTests();
	CTAPITests();
	PKCS11Tests();
	systemSpecificTests();
	addLine(_T("All done!"));
}

bool DiagnosticsDialog::listFileVersion(wxString desc,wxString libName,int libVer) {
	bool found = false;
	wxString version = _T("missing");
	try {
		DynamicLibrary lib(libName.ToAscii(),libVer);
		version = wxString(_T("ver ")) +
			Asc(lib.getVersionStr());
		found = true;
	} catch(std::runtime_error &err) {
		version = wxT("exception:") + wxString::FromAscii(err.what());
		}
	addLine(desc + _T(" |") + libName + _T("| ") + version);
	return found;
}

void DiagnosticsDialog::SCTest(int level,ManagerInterface &cardMgr) {
	addLine(_T("SCTest level (") + 	wxString::Format(_T("%d"),level) + _T("):"));
	for(uint i = 0;i < cardMgr.getReaderCount();i++) {
		wxString prefix = wxString::Format(_T("[%d] "),i);
		switch(level) {
			case 0: //just list readers
				addLine(prefix + _T("reader: '") +
					Asc(cardMgr.getReaderName(i))
					+ _T("' state:") +
					Asc(cardMgr.getReaderState(i))
					+ ENDL + _T("\tATR:") +
					Asc(cardMgr.getATRHex(i)));
				break;
			case 1: { // try to connect to EstEID
				EstEidCard card(cardMgr);
				if (card.isInReader(i)) {
					card.connect(i);
					addLine(prefix + _T("reader: '") +
						Asc(cardMgr.getReaderName(i))
						+ _T("' has EstEID"));
					}
				break;
				}
			case 2: //try to read data off EstEID
				EstEidCard card(cardMgr);
				if (card.isInReader(i)) {
					card.connect(i);
					addLine(prefix + _T("card ID:") +
						Asc(card.readCardID()));
					}
				break;
			}
		}
	}

void DiagnosticsDialog::PCSCTests() {
	addLine(_T("PCSC Manager tests:"));
	try {
		PCSCManager cardMgr;
		SCTest(0,cardMgr);
		SCTest(1,cardMgr);
		SCTest(2,cardMgr);
	} catch (std::runtime_error& err) {
		addLine(wxString(_T("\tfailed, exception : ")) +
			Asc(err.what()));
			}
	}

void DiagnosticsDialog::CTAPITests() {
	addLine(_T("CTAPI Manager tests:"));
	try {
		CTAPIManager cardMgr;
		SCTest(0,cardMgr);
		SCTest(1,cardMgr);
		SCTest(2,cardMgr);
	} catch (std::runtime_error& err) {
		addLine(wxString(_T("\tfailed, exception : ")) +
			Asc(err.what()));
			}
	}

void DiagnosticsDialog::PKCS11Tests() {
    std::ostringstream strm;
	addLine(_T("PKCS#11 module tests:"));
    try {
        pkcs11module mod("opensc-pkcs11");
        mod.test(strm);
        addLine(Asc(strm.str()));
		strm.str() = "";
		pkcs11module mod2("esteidpkcs11");
		mod2.test(strm);
        addLine(Asc(strm.str()));
	} catch (std::runtime_error& err) {
		addLine(wxString(_T("\tfailed, exception : ")) +
			Asc(err.what()));
			}
	}

#ifdef __WXMSW__
BOOL BuildRestrictedSD(PSECURITY_DESCRIPTOR pSD) {
	BOOL bResult = FALSE;
	if (!InitializeSecurityDescriptor(pSD,
		SECURITY_DESCRIPTOR_REVISION))
		throw std::runtime_error("InitializeSecurityDescriptor failed");
	if (!SetSecurityDescriptorDacl(pSD, TRUE, (PACL) NULL, FALSE))
		throw std::runtime_error("SetSecurityDescriptorDacl failed");
	return TRUE;
	}

wxString testMutexState() {
	HANDLE mutexHandle = INVALID_HANDLE_VALUE;
	DWORD waitRes;
	wxString result;
	try {
		SECURITY_DESCRIPTOR sd;
		SECURITY_ATTRIBUTES sa;
		BuildRestrictedSD(&sd);
		mutexHandle = CreateMutex(&sa,FALSE,_T("_esteidCSPAccessMutex"));
		if (mutexHandle == NULL ) {
			mutexHandle = OpenMutex(SYNCHRONIZE,FALSE,_T("_esteidCSPAccessMutex"));
			if (mutexHandle == NULL) throw std::runtime_error("OpenMutex returns NULL");
			}
		waitRes = WaitForSingleObjectEx(mutexHandle,500,FALSE);
		ReleaseMutex(mutexHandle);
		switch(waitRes) {
			case WAIT_OBJECT_0 :	result+=_T("AOK, WAIT_OBJECT_0");break;
			case WAIT_ABANDONED:	result+=_T("NOK, WAIT_ABANDONED");break;
			case WAIT_IO_COMPLETION:result+=_T("NOK, WAIT_IO_COMPLETION");break;
			case WAIT_TIMEOUT:		result+=_T("NOK, WAIT_TIMEOUT");break;
			default:
				throw std::runtime_error("WaitForSingleObject returns WAIT_FAILED");
			}
	} catch(std::runtime_error err) {
		result = wxString(_T("exception ")) + Asc(err.what()) + _T(" code=") +
			wxString::Format(_T("0x%08X"),GetLastError());
		}
	return _T("Mutex state check: ") + result;
	}

class cryptErr:public std::runtime_error {
public:
	DWORD mCode;
	cryptErr(const char *w) : std::runtime_error(w),mCode(GetLastError()) {}
	};

struct cContext {
	HCRYPTPROV prov;
	cContext(TCHAR *provider,DWORD flags) {
		if (!CryptAcquireContext(&prov,NULL,provider,PROV_RSA_FULL,flags))
			throw cryptErr("CryptAcquireContext failed");
		}
	~cContext() { CryptReleaseContext(prov,0); }
	void getContainerName(ByteVec  &buff) {
		DWORD sz = DWORD(buff.size());
		if (!CryptGetProvParam(prov,PP_CONTAINER,&buff[0],&sz,0))
			throw cryptErr("CryptGetProvParam failed");
		buff[sz] = 0;
		}
	operator HCRYPTPROV() const {return prov;}
	};

struct cUserKey {
	HCRYPTKEY key;
	cUserKey(HCRYPTPROV prov,DWORD keySpec) {
		if (!CryptGetUserKey(prov,keySpec,&key))
			throw cryptErr("CryptGetUserKey failed");
		}
	void getCert(ByteVec &buff) {
		DWORD sz = DWORD(buff.size());
		if (!CryptGetKeyParam(key,KP_CERTIFICATE,&buff[0],&sz,0))
			throw cryptErr("CryptGetKeyParam failed");
		buff.resize(sz);
		}
	~cUserKey() {CryptDestroyKey(key);}
	};

void testCSP(DiagnosticsDialog *d,TCHAR *cspName) {
	try {
		d->addLine(wxString(_T("Doing CSP diagnosis for : '")) + cspName + _("'"));
		cContext ctx(cspName,0);
		d->addLine(_T("Initialized .."));
		ByteVec buff(80);
		ctx.getContainerName(buff);
		d->addLine(wxString(_T("CSP Context acquired, name '")) +
			Asc((const char *) &buff[0]) + _T("'"));
		cUserKey authKey(ctx,AT_KEYEXCHANGE);
		d->addLine(_T("Authkey acquired .."));
		ByteVec cert(8192);
		authKey.getCert(cert);
		d->addLine(_T("Certificate read, len:") +
			wxString::Format(_T("%d"),cert.size()));
	} catch(cryptErr &err) {
		d->addLine(wxString(_T("exception, ")) + Asc(err.what()) + _T(" code=") +
			wxString::Format(_T("0x%08X"),err.mCode));
		}
	}

wxString getCpuType() {
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return wxString::Format(_T("%d"),sysinfo.dwProcessorType);
	}
wxString getMemoryMB() {
	MEMORYSTATUS memsts;
	GlobalMemoryStatus(&memsts);
	return wxString::Format(_T("%d MB"),memsts.dwTotalPhys / (1024 * 1024));
	}
#else

bool readValue(const char *file,std::string pref,std::string &value) {
	std::string line;
	std::ifstream strm;
	char stackProtect1[8] = "0"; //libstc++6 ifstream trashes stack here!
	stackProtect1[1]=stackProtect1[0]; //avoid unused warning
	strm.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
	try {
		strm.open(file);
		while(strm && strm.good() && !strm.eof()) {
			getline(strm,line);
			if (line.substr(0,pref.length()) == pref) {
				line.erase(0,
					line.find_first_not_of(" \t:",pref.length()));
				value = line;
				return true;
				}
			}
	} catch(std::exception &ex) {
		value = ex.what();
		}
	return false;
	}

wxString getCpuType() {
	std::string val;
	if (!readValue("/proc/cpuinfo","model name",val)) return _T("unknown");
	return Asc(val);
	}

wxString getMemoryMB() {
	std::string val;
	if (!readValue("/proc/meminfo","MemTotal",val)) return _T("unknown");
	return Asc(val);
	}
#endif

#ifdef __WXMSW__
void DiagnosticsDialog::systemSpecificTests() {
	addLine(testMutexState());
	testCSP(this,_T("EstEID Card CSP"));
	if (m_haveCardmodule)
		testCSP(this,_T("Microsoft Base Smart Card Crypto Provider"));
	testCSP(this,_T("EstEID NewCard CSP"));
	}
#else

void DiagnosticsDialog::systemSpecificTests() {
	//probably check if pcscd daemon is running.. how ?
	}
#endif
