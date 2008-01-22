/*!
	\file		MainDialog.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "MainDialog.h"
#include "AppSetting.h"
#include "wx/menu.h"
#include "wx/mstream.h"
#include "wx/aboutdlg.h"
#include "wx/help.h"
#include "cardlib/PCSCManager.h"
#include "cardlib/CTAPIManager.h"
#include "Setup.h"

enum MainMenu {
	idNoReaders,
	idSelectReader0,
	idSelectReader1,
	idSelectReader2,
	idSelectReader3,
	idReadData,
	idShow,
	idSaveToFile,
	idDownloadPicture,
	idOpenWelcomeWeb,
	idLangEstonian,
	idLangEnglish,
	idLangRussian,
	idPCSC,
	idCTAPI,
	idCertRegister,
	idCertAutoremoveOn,
	idCertAutoremoveOff,
	idShowAuthCert,
	idShowSignCert,
	idSaveAuthCert,
	idSaveSignCert,
	idVerifyCerts,
	idPinCounter,
	idChangeAuthPin,
	idChangeSignPin,
	idChangePUK,
	idUnblockAuthPin,
	idUnblockSignPin,
//builtin	idHelp,
	idDiagnostics,
	idOpenIdWeb,
	idOpenSupportWeb,
	idSysinfo,
//builtin	idAbout,
};

int MainDialog::rIDs[] = {idSelectReader0,idSelectReader1,idSelectReader2,idSelectReader3};

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_MY_CUSTOM_COMMAND, 7777)
END_DECLARE_EVENT_TYPES()
DEFINE_EVENT_TYPE(wxEVT_MY_CUSTOM_COMMAND)

BEGIN_EVENT_TABLE(MainDialog, wxFrame)
	EVT_CUSTOM(wxEVT_MY_CUSTOM_COMMAND, wxID_ANY, MainDialog::OnMyEvent)
	EVT_PAINT(MainDialog::OnPaint)
	EVT_ERASE_BACKGROUND(MainDialog::OnEraseBackground)
	EVT_MENU(wxID_EXIT, MainDialog::OnQuit)
	EVT_MENU(idSelectReader0,MainDialog::OnSelectReader)
	EVT_MENU(idSelectReader1,MainDialog::OnSelectReader)
	EVT_MENU(idSelectReader2,MainDialog::OnSelectReader)
	EVT_MENU(idSelectReader3,MainDialog::OnSelectReader)
	EVT_MENU(idReadData,MainDialog::OnReadData)
	EVT_MENU(idShow,MainDialog::OnShow)
	EVT_MENU(idSaveToFile,MainDialog::OnSaveToFile)
	EVT_MENU(idDownloadPicture,MainDialog::OnDownloadPicture)
	EVT_MENU(idOpenWelcomeWeb,MainDialog::OnOpenWelcomeWeb)
	EVT_MENU(idLangEstonian,MainDialog::OnLangEstonian)
	EVT_MENU(idLangEnglish,MainDialog::OnLangEnglish)
	EVT_MENU(idLangRussian,MainDialog::OnLangRussian)
	EVT_MENU(idPCSC,MainDialog::OnInterfaceSelect)
	EVT_MENU(idCTAPI,MainDialog::OnInterfaceSelect)
	EVT_MENU(idCertRegister,MainDialog::OnCertRegister)
	EVT_MENU(idCertAutoremoveOn,MainDialog::OnCertAutoremove)
	EVT_MENU(idCertAutoremoveOff,MainDialog::OnCertAutoremove)
	EVT_MENU(idShowAuthCert,MainDialog::OnShowAuthCert)
	EVT_MENU(idShowSignCert,MainDialog::OnShowSignCert)
	EVT_MENU(idSaveAuthCert,MainDialog::OnSaveAuthCert)
	EVT_MENU(idSaveSignCert,MainDialog::OnSaveSignCert)
	EVT_MENU(idVerifyCerts,MainDialog::OnVerifyCerts)
	EVT_MENU(idPinCounter,MainDialog::OnPinCounter)
	EVT_MENU(idChangeAuthPin,MainDialog::OnChangeAuthPin)
	EVT_MENU(idChangeSignPin,MainDialog::OnChangeSignPin)
	EVT_MENU(idChangePUK,MainDialog::OnChangePUK)
	EVT_MENU(idUnblockAuthPin,MainDialog::OnUnblockAuthPin)
	EVT_MENU(idUnblockSignPin,MainDialog::OnUnblockSignPin)
	EVT_MENU(wxID_HELP,MainDialog::OnHelp)
	EVT_MENU(idDiagnostics,MainDialog::OnDiagnostics)
	EVT_MENU(idOpenIdWeb,MainDialog::OnOpenIdWeb)
	EVT_MENU(idOpenSupportWeb,MainDialog::OnOpenSupportWeb)
	EVT_MENU(idSysinfo,MainDialog::OnSysinfo)
	EVT_MENU(wxID_ABOUT,MainDialog::OnAbout)
	EVT_LEFT_UP(MainDialog::OnMouseClick)
#ifdef __WXMSW__ //no need to create load
	EVT_MOTION(MainDialog::OnMouseMove)
#endif
END_EVENT_TABLE()

#ifdef __WXMSW__
bool myLoadUserResource(const wxString& resourceName, LPCTSTR resourceType,wxMemoryBuffer &buf) {
    HRSRC hResource = ::FindResource(wxGetInstance(), resourceName, resourceType);
    if ( hResource == 0 ) return false;

    HGLOBAL hData = ::LoadResource(wxGetInstance(), hResource);
    if ( hData == 0 ) return false;

    wxChar *theData = (wxChar *)::LockResource(hData);
    if ( !theData ) return false;

    int len = ::SizeofResource(wxGetInstance(), hResource);
	buf.SetDataLen(0);
	buf.AppendData(theData,len);
    return true;
}
#else
#include "resources/esteid.xpm"
#include "resources/logo_sk.xpm"
#include "resources/esteidbmp.cdata"
#include "resources/digistampbmp.cdata"
#endif

void MainDialog::ReloadMenu(wxLanguage lang)
{
    cardMenu = new wxMenu;
	//
	//wxMenu *readerMenu = new wxMenu;
	readerMenu = new wxMenu;
	cardMenu->AppendSubMenu(readerMenu,_("Select reader"));
	//
	cardMenu->Append(idReadData,_("Read data"));
    wxMenu *personalMenu = new wxMenu;
	personalMenu->Append(idShow,_("Show"));
	personalMenu->Append(idSaveToFile,_("Save to file"));
	cardMenu->AppendSubMenu(personalMenu,_("Personal data"));
	cardMenu->Append(idDownloadPicture,_("Download picture"));
	cardMenu->FindItem(idDownloadPicture)->Enable(false);
	cardMenu->Append(idOpenWelcomeWeb,_("Open 'Welcome' webpage"));

	cardMenu->AppendSeparator();
	cardMenu->Append(wxID_EXIT,_("Exit"));

    wxMenu *settingsMenu = new wxMenu;
	wxMenu *languageMenu = new wxMenu;
	languageMenu->AppendCheckItem(idLangEstonian,_("Estonian"));
	languageMenu->Check(idLangEstonian, lang == wxLANGUAGE_ESTONIAN);
	languageMenu->AppendCheckItem(idLangEnglish,_("English"));
	languageMenu->Check(idLangEnglish, lang == wxLANGUAGE_ENGLISH);
	languageMenu->AppendCheckItem(idLangRussian,_("Russian"));
	languageMenu->Check(idLangRussian, lang == wxLANGUAGE_RUSSIAN);
	settingsMenu->AppendSubMenu(languageMenu,_("Language"));
	interfaceMenu = new wxMenu;
	interfaceMenu->AppendCheckItem(idPCSC,_T("PCSC"));
	interfaceMenu->Check(idPCSC,!AppSetting().getUseCTAPI());
	interfaceMenu->AppendCheckItem(idCTAPI,_T("CTAPI"));
	interfaceMenu->Check(idCTAPI,AppSetting().getUseCTAPI());
	settingsMenu->AppendSubMenu(interfaceMenu,_("Interface"));

    wxMenu *certMenu = new wxMenu;
#ifdef __WXMSW__
	certMenu->Append(idCertRegister,_("Register"));
	autoRemoveMenu = new wxMenu;
	autoRemoveMenu->AppendCheckItem(idCertAutoremoveOn,_("On"));
	autoRemoveMenu->Check(idCertAutoremoveOn, AppSetting().getAutoRemove());
	autoRemoveMenu->AppendCheckItem(idCertAutoremoveOff,_("Off"));
	autoRemoveMenu->Check(idCertAutoremoveOff, !AppSetting().getAutoRemove());
	certMenu->AppendSubMenu(autoRemoveMenu,_("Automatic removal"));
	certMenu->AppendSeparator();
#endif
	wxMenu* showMenu = new wxMenu;
	showMenu->Append(idShowAuthCert,_("Authentication certificate"));
	showMenu->Append(idShowSignCert,_("Signature certificate"));
	certMenu->AppendSubMenu(showMenu,_("Show"));
	wxMenu* saveMenu = new wxMenu;
	saveMenu->Append(idSaveAuthCert,_("Authentication certificate"));
	saveMenu->Append(idSaveSignCert,_("Signature certificate"));
	certMenu->AppendSubMenu(saveMenu,_("Save to file"));
	certMenu->AppendSeparator();
	certMenu->Append(idVerifyCerts,_("Verify certificates"));

    wxMenu *pinMenu = new wxMenu;
	pinMenu->Append(idPinCounter,_("PIN counter"));
	pinMenu->Append(idChangeAuthPin,_("Change authentication PIN (PIN 1)"));
	pinMenu->Append(idChangeSignPin,_("Change signature PIN (PIN 2)"));
	pinMenu->Append(idChangePUK,_("Change PUK"));
	pinMenu->Append(idUnblockAuthPin,_("Unblock authentication PIN (PIN 1)"));
	pinMenu->Append(idUnblockSignPin,_("Unblock signature PIN (PIN 2)"));

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP,_("User guide"));
	helpMenu->AppendSeparator();
	helpMenu->Append(idOpenIdWeb,_("Open ID-card website"));
	helpMenu->Append(idOpenSupportWeb,_("Open support website"));
	helpMenu->AppendSeparator();
	helpMenu->Append(idDiagnostics,_("Diagnostics"));
	helpMenu->Append(idSysinfo,_("System information"));
	helpMenu->Append(wxID_ABOUT,_("About"));

	wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(cardMenu, _("Card"));
    menuBar->Append(settingsMenu, _("Settings"));
    menuBar->Append(certMenu, _("Certificates"));
    menuBar->Append(pinMenu, _("PIN"));
    menuBar->Append(helpMenu, _("Help"));
	wxMenuBar *oldBar = GetMenuBar();
    SetMenuBar(menuBar);
	if (oldBar)
		wxDELETE(oldBar);
}

void MainDialog::doUpdateMenus() {
	cardMenu->FindItem(idDownloadPicture)->Enable(havePersonalCard);
	}

void MainDialog::doPopupError(wxString message) {
	wxMessageBox(message
		,getAppName(),wxOK | wxCENTRE |wxICON_ERROR);
	}
void MainDialog::doPopupInfo(wxString message) {
	wxMessageBox(message
		,getAppName(),wxOK | wxCENTRE |wxICON_INFORMATION);
	}

void MainDialog::doShowError(wxString prompt) {
	SetStatusText(prompt);
	if (mEnableErrorPopup) {
		mEnableErrorPopup = false;
		doPopupError(prompt);
		}
	}
void MainDialog::doShowError(std::runtime_error &err) {
	wxString prompt = wxString(_("Error occured")) + _T(" : ")
			+ wxString::FromAscii(err.what());
	doShowError(prompt);
	}

void MainDialog::SwitchCardManager(bool useCTAPI) {
	if (mCardManager) {
		delete mCardManager;
		mCardManager = NULL;
		}
	mPrevSelectedReader = mSelectedReader = -1;
	size_t numReaders=0;
	try {
		if (useCTAPI)
			mCardManager = new CTAPIManager();
		else
			mCardManager = new PCSCManager();
		numReaders = mCardManager->getReaderCount();
		if (numReaders == 0 )
			doShowError(_T("There are no smart card readers installed"));

	} catch(std::runtime_error &err) {
		doShowError(err);
	}

	if (readerMenu->FindItem(idNoReaders)) readerMenu->Delete(idNoReaders);

	for (size_t i=0;i < sizeof(rIDs)/sizeof(*rIDs);i++) {
		if (readerMenu->FindItem(rIDs[i]))
			readerMenu->Delete(rIDs[i]);
		if (numReaders <= i) continue;
		readerMenu->AppendCheckItem(rIDs[i],
			menuFromReaderName(mCardManager->getReaderName(uint(i)),""));
		}
	if (numReaders == 0) {
		readerMenu->Append(idNoReaders,_("No smart card readers"))->Enable(false);
		}
	}

wxString MainDialog::getAppName() {
	return _("ID-card tool");
};

void MainDialog::SwitchLanguage(wxLanguage lang)
{
	if (m_locale) wxDELETE(m_locale);
	m_locale = new wxLocale;
	m_locale->Init(lang,wxLOCALE_CONV_ENCODING);
    m_locale->AddCatalogLookupPathPrefix(wxT("."));
    m_locale->AddCatalog(wxT("EsteidUtil"));

	ReloadMenu(lang);
	SetTitle(getAppName());
	this->Refresh();
	AppSetting().setLanguage(lang);

	/*update reader option*/
	SwitchCardManager(AppSetting().getUseCTAPI());
	ReflectSelectedReader();
}

MainDialog::MainDialog(void) :
	wxFrame(NULL, wxID_ANY,
             _("ID-card tool"),
             wxDefaultPosition,
			wxSize(417,328),
            wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX |
             wxCAPTION | wxCLIP_CHILDREN)
	,m_locale(NULL),mCardManager(NULL),havePersonalCard(true),
	mIdConv(wxFONTENCODING_CP1252),mEnableErrorPopup(true),
	mPicRect(280,98,100,132),handCursor(wxCURSOR_HAND)
{
	wxImage::AddHandler( new wxGIFHandler );
	wxImage::AddHandler( new wxJPEGHandler );
	SetIcon(wxICON(esteid));

    CreateStatusBar(1,wxFULL_REPAINT_ON_RESIZE);
    SetStatusText(_("..."));

	SwitchLanguage( AppSetting().getLanguage());

	SwitchBitmap();

/*adjust client size to match bitmap size*/
	int cw,ch,fw,fh;
	GetClientSize(&cw,&ch);
	GetSize(&fw,&fh);
	SetSize(411 + (fw - cw), 264 + (fh - ch));

    wxCommandEvent eventCustom(wxEVT_MY_CUSTOM_COMMAND);
    wxPostEvent(this, eventCustom);
}

void MainDialog::SwitchBitmap() {
	unsigned char *data;
	size_t sz;
#ifdef __WXMSW__
	wxMemoryBuffer buff;
	myLoadUserResource(havePersonalCard ? wxT("BACKGROUND") : wxT("ALTBACKGROUND")
		,RT_RCDATA,buff);
	data = (unsigned char*)buff.GetData();
	sz = buff.GetDataLen();
#else
	if (havePersonalCard) {
		data = esteidbmp_cdata; sz = sizeof(esteidbmp_cdata);
		}
	else {
		data = digistampbmp_cdata; sz = sizeof(digistampbmp_cdata);
		}
#endif
	wxMemoryInputStream mstream(data,sz);
	wxImage bmp(mstream);
	Bitmap = wxBitmap(bmp);
	}

MainDialog::~MainDialog(void)
{
	if (m_locale)
		wxDELETE(m_locale);
	if (mCardManager)
		delete mCardManager;
}

void MainDialog::OnMouseClick(wxMouseEvent &evt) {
	if (!havePersonalCard) return;
	if (userBitmap.IsOk()) return;
	if (mPicRect.Contains(evt.GetX(),evt.GetY())) {
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED,	idDownloadPicture);
		wxPostEvent(this,evt);
		}
	}

void MainDialog::OnMouseMove(wxMouseEvent &evt) {
	if (!havePersonalCard) return;
	if (mPicRect.Contains(evt.GetX(),evt.GetY())) {
		if (!mCursorIsHand) {
			if (userBitmap.IsOk()) return;
			this->SetCursor(handCursor);
			mCursorIsHand = true;
			}
		}
	else {
		if (mCursorIsHand) this->SetCursor(*wxSTANDARD_CURSOR);
		mCursorIsHand = false;
		}
	}


void MainDialog::OnMyEvent( wxEvent &event )
{
	doStartupChecks();
}

void MainDialog::OnQuit(wxCommandEvent& event)
{
    Close(true);
}

void MainDialog::OnEraseBackground(wxEraseEvent& refEvent)
{
}

void MainDialog::ReflectSelectedReader() {
	for(size_t i = 0; i < sizeof(rIDs)/sizeof(*rIDs);i++)
		if (readerMenu->FindItem(rIDs[i]))
			readerMenu->Check(rIDs[i], size_t(mSelectedReader) == i);
	}

void MainDialog::OnSelectReader(wxCommandEvent&  event) {
	int savedReader = mSelectedReader;

	mSelectedReader =0;
	for(size_t i = 0; i < sizeof(rIDs)/sizeof(*rIDs);i++) {
		if (event.GetId() == rIDs[i])
			mSelectedReader = int(i);
		}
	ReflectSelectedReader();

	if (savedReader != mSelectedReader) {
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED,idReadData);
		wxPostEvent(this,evt);
		}
	}

void MainDialog::OnLangEstonian(wxCommandEvent&  event) {
	SwitchLanguage(wxLANGUAGE_ESTONIAN);
}
void MainDialog::OnLangEnglish(wxCommandEvent&  event) {
	SwitchLanguage(wxLANGUAGE_ENGLISH);
}
void MainDialog::OnLangRussian(wxCommandEvent&  event) {
	SwitchLanguage(wxLANGUAGE_RUSSIAN);
}

void MainDialog::OnInterfaceSelect(wxCommandEvent&  event)
{
	bool useCTAPI = event.GetId() == idCTAPI;

	SwitchCardManager(useCTAPI);

	interfaceMenu->FindItem(idPCSC)->Check(!useCTAPI);
	interfaceMenu->FindItem(idCTAPI)->Check(useCTAPI);
	if (useCTAPI) {
		std::string lib;
		unsigned int port;
		((CTAPIManager *)mCardManager)->getDefaultLibAndPort(lib,port);
		AppSetting().setUseCTAPI(wxString::FromAscii(lib.c_str()),port);
		}
	else
		AppSetting().setUseCTAPI(_T(""),0);
	wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED,idReadData);
	wxPostEvent(this,evt);
}

void MainDialog::OnCertAutoremove(wxCommandEvent&  event)
{
	bool switchOn = event.GetId()== idCertAutoremoveOn;
	AppSetting().setAutoremove(switchOn);
	autoRemoveMenu->Check(idCertAutoremoveOn, switchOn);
	autoRemoveMenu->Check(idCertAutoremoveOff,! switchOn);
}

void MainDialog::OnHelp(wxCommandEvent&  event)
{
	wxHelpController help;
	help.Initialize(_T("Haldusutiliit"));
	help.DisplayContents();
}

void MainDialog::OnOpenWelcomeWeb(wxCommandEvent&  event) {
	doLaunchUrl(_T("https://www.sk.ee/tervitus/"));
}

void MainDialog::OnOpenIdWeb(wxCommandEvent&  event) {
	wxLaunchDefaultBrowser(_T("http://www.id.ee/"));
}
void MainDialog::OnOpenSupportWeb(wxCommandEvent&  event) {
	wxLaunchDefaultBrowser(_T("http://support.sk.ee/"));
}

void MainDialog::OnAbout(wxCommandEvent&  event) {
	wxAboutDialogInfo in;
	in.SetIcon(wxICON(logo_sk));
	in.SetWebSite(_T("http://www.sk.ee"));
	in.SetDescription(_("ID-card utility"));
	in.SetCopyright(_T("(C) 2007 AS Sertifitseerimiskeskus"));
	in.SetVersion(_T(VERSION));
	wxAboutBox(in);
}
