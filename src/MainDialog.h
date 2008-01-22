/*!
	\file		 MainDialog.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#pragma once
#include "wx/frame.h"
#include "wx/intl.h"
#include "cardlib/ManagerInterface.h"
#include "cardlib/CardBase.h"

class MainDialog :
	public wxFrame
{
public:
	MainDialog(void);
	~MainDialog(void);

    void OnQuit(wxCommandEvent& );
    void OnEraseBackground(wxEraseEvent& );
	void OnPaint(wxPaintEvent &);
	void OnMyEvent(wxEvent &);

	void OnSelectReader(wxCommandEvent& );
	void OnReadData(wxCommandEvent& );
	void OnShow(wxCommandEvent& );
	void OnSaveToFile(wxCommandEvent& );
	void OnDownloadPicture(wxCommandEvent& );
	void OnOpenWelcomeWeb(wxCommandEvent& );
	void OnLangEstonian(wxCommandEvent& );
	void OnLangEnglish(wxCommandEvent& );
	void OnLangRussian(wxCommandEvent& );
	void OnInterfaceSelect(wxCommandEvent& );
	void OnCertRegister(wxCommandEvent& );
	void OnCertAutoremove(wxCommandEvent& );
	void OnShowAuthCert(wxCommandEvent& );
	void OnShowSignCert(wxCommandEvent& );
	void OnSaveAuthCert(wxCommandEvent& );
	void OnSaveSignCert(wxCommandEvent& );
	void OnVerifyCerts(wxCommandEvent& );
	void OnPinCounter(wxCommandEvent& );
	void OnChangeAuthPin(wxCommandEvent& );
	void OnChangeSignPin(wxCommandEvent& );
	void OnChangePUK(wxCommandEvent& );
	void OnUnblockAuthPin(wxCommandEvent& );
	void OnUnblockSignPin(wxCommandEvent& );
	void OnHelp(wxCommandEvent& );
	void OnDiagnostics(wxCommandEvent& );
	void OnOpenIdWeb(wxCommandEvent& );
	void OnOpenSupportWeb(wxCommandEvent& );
	void OnSysinfo(wxCommandEvent& );
	void OnAbout(wxCommandEvent& );
	void OnMouseClick(wxMouseEvent&);
	void OnMouseMove(wxMouseEvent&);

private:
    DECLARE_EVENT_TABLE()
	wxBitmap Bitmap;
	wxLocale *m_locale;
	wxRect mPicRect;
	wxCursor handCursor;
	bool mCursorIsHand;

	wxCSConv mIdConv; //encoding converter
	ManagerInterface *mCardManager;
	wxMenu *readerMenu;
	wxMenu *interfaceMenu;
	wxMenu *autoRemoveMenu;
	wxMenu *cardMenu;
	int mSelectedReader,mPrevSelectedReader;
	static int rIDs[];
	bool mEnableErrorPopup;
	wxBitmap userBitmap;

public:
	enum BackBitmaps {
		PERSONAL_ID_CARD,
		ORGANIZATION_SIGNATURE_CARD
	};

	wxString menuFromReaderName(std::string reader,std::string name);
	void ReloadMenu(wxLanguage lang);
	void SwitchLanguage(wxLanguage lang);
	void SwitchCardManager(bool useCTAPI);
	void ReflectSelectedReader();
	void ReloadCardData(int lastRecord,bool withCert = false);
	bool FindCard(CardBase &card);
	void ClearCardData();
	void doUpdateMenus();

	void doPinChange(int type );
	void doSaveCert(bool authCert);
	void doShowCert(ByteVec &certBytes);
	void doStartupChecks();
	void doLaunchUrl(wxString url);
	void doCheckCertRegistration(ByteVec &certBytes);
	void doPopupError(wxString message);
	void doPopupInfo(wxString message);
	void doShowError(wxString prompt);
	void doShowError(std::runtime_error &err);
	void doWindowsCertCheck(ByteVec &certBytes,void ** ref);

	void SwitchBitmap();
	wxString getAppName();

	//cardData
	wxString m_cardId,m_prevCardId;
	wxArrayString mPersonalData;
	wxString m_cardEmail;
	bool havePersonalCard;

	wxString subjCN;
	wxString subjO;
	wxString subjOU;
	wxString validFrom;
	wxString validTo;

};
