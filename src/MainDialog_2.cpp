/*!
	\file		MainDialog_2.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "MainDialog.h"
#include "cardlib/PCSCManager.h"
#include "cardlib/EstEidCard.h"
#include "utility/asnObject.h"
#include "utility/asnCertificate.h"
#include "PersonalDataDialog.h"
#include "CertificateDialog.h"
#include "PinChangeDialog.h"
#include "DiagnosticsDialog.h"
#include "AppSetting.h"
#include <fstream>
#include <algorithm>
#include "wx/mstream.h"

void renderPair(wxPaintDC &dc,wxString lbl,wxString val,int x, int y) {
	wxFont fsmall(8 ,wxSWISS,wxNORMAL,wxFONTWEIGHT_NORMAL,false,wxT("Arial"));
	wxFont fbold(10 ,wxSWISS,wxNORMAL,wxFONTWEIGHT_BOLD,false,wxT("Arial"));
	dc.SetFont(fsmall);
	int xx,yy;
	dc.GetTextExtent(lbl,&xx,&yy);
	dc.DrawText(lbl,x,y);
	dc.SetFont(fbold);
	dc.DrawText(val,x+xx,y-2);
	}

void MainDialog::OnPaint(wxPaintEvent &refEvent) {
	static bool hadPersonalCard = false;
	if (hadPersonalCard != havePersonalCard) {
		SwitchBitmap();
		hadPersonalCard = havePersonalCard;
		}
	wxPaintDC dc(this);
 	wxMemoryDC dcMem;
 	dcMem.SelectObject(Bitmap);
	dc.Blit(0, 0, Bitmap.GetWidth(),Bitmap.GetHeight(), & dcMem, 0, 0);
	dcMem.SelectObject(wxNullBitmap);

	if (mPersonalData.GetCount() < EstEidCard::EXPIRY + 1)
		mPersonalData.SetCount(EstEidCard::EXPIRY + 1);

	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	if (havePersonalCard) {
		dc.DrawText(mPersonalData[EstEidCard::SURNAME] ,120,66);
		dc.DrawText(mPersonalData[EstEidCard::FIRSTNAME],120,95);
		dc.DrawText(mPersonalData[EstEidCard::SEX ],170,125);
		dc.DrawText(mPersonalData[EstEidCard::CITIZEN ],170,140);
		dc.DrawText(mPersonalData[EstEidCard::BIRTHDATE],170,155);
		dc.DrawText(m_cardId,170,170);
		dc.DrawText(mPersonalData[EstEidCard::DOCUMENTID],170,200);
		dc.DrawText(mPersonalData[EstEidCard::EXPIRY ],170,230);
		dc.DrawText(m_cardEmail,170,245);
//#ifdef __WXMSW__ //enable this for others as well, once we have SSL client code
		wxRect frame(mPicRect);
		frame.Inflate(2,2);
		dc.SetPen(*wxLIGHT_GREY_PEN);
		wxPoint tl = frame.GetTopLeft();
		wxPoint br = frame.GetBottomRight();
		dc.DrawLine(tl.x , tl.y	, br.x , tl.y );
		dc.DrawLine(br.x , tl.y , br.x , tl.y  );
		dc.SetPen(*wxWHITE_PEN);
		dc.DrawLine(tl.x, tl.y , tl.x , br.y );
		dc.DrawLine(tl.x, br.y , br.x , br.y );
		if (userBitmap.Ok()) {
			dcMem.SelectObject(userBitmap);
			dc.Blit(mPicRect.GetX(),mPicRect.GetY()
				,userBitmap.GetWidth() , userBitmap.GetHeight(),
				&dcMem,0,0, wxCOPY);
			dcMem.SelectObject(wxNullBitmap);
		} else {
			wxFont f(*wxNORMAL_FONT);
			f.SetWeight(wxBOLD);
			f.SetPointSize(f.GetPointSize() - 1);
			dc.SetFont(f);
			wxString prompt = _("Download picture");
			wxString lb1=prompt.Left(prompt.Find(_T(" ")));
			wxString lb2=prompt.Right(prompt.Len() - lb1.Len() - 1);
			int w1,w2,dummy;
			dc.GetTextExtent(lb1,&w1,&dummy);
			dc.GetTextExtent(lb2,&w2,&dummy);
			int midx = (frame.GetLeft() + frame.GetRight()) / 2
				- (w1 / 2);
			dc.DrawText(lb1,midx, frame.GetTop() + 50);
			midx = (frame.GetLeft() + frame.GetRight()) / 2
				- (w2 / 2);
			dc.DrawText(lb2,midx, frame.GetTop() + 62);
			}
//#endif
	} else {
		renderPair(dc,_T(""),subjCN,30,148);
		renderPair(dc,wxString(_("ORGANIZATION"))+ _T("  :  "), subjO ,30,170);
		renderPair(dc,wxString(_("UNIT"))+ _T("  :  "), subjOU,30,188);
		renderPair(dc,wxString(_("E-MAIL"))+ _T("  :  "), m_cardEmail,30,206);
		renderPair(dc,wxString(_("VALIDITY"))+ _T("  :  ") , validFrom + _T(" - ") + validTo,30,224);
		renderPair(dc,wxString(_("CARD NUMBER"))  + _T("  :  ") , m_cardId ,30,242);

/*		dc.DrawText(subjCN,30,146);
		dc.DrawText(wxString(_("ORGANIZATION")) + _T("  :  ") + subjO ,30,170);
		dc.DrawText(wxString(_("UNIT"))  + _T("  :  ") + subjOU,30,188);
		dc.DrawText(wxString(_("E-MAIL"))  + _T("  :  ") + m_cardEmail,30,206);
		dc.SetFont(fsmall);
		dc.DrawText(wxString(_("VALIDITY"))  + _T("  :  ")
			+ validFrom + _T(" - ") + validTo,30,224);
		dc.DrawText(wxString(_("CARD NUMBER"))  + _T("  :  ") + m_cardId
			,30,242);*/
/*		wxFont f(*wxSWISS_FONT);
		f.SetWeight(wxBOLD);
		f.SetPointSize(f.GetPointSize() + 4);
		dc.SetFont(f);*/
/*		dc.SetFont(wxFont( 12 ,wxSWISS,wxNORMAL,wxFONTWEIGHT_NORMAL,false,wxT("Arial")));
		dc.DrawText(subjCN,30,146);*/
		}
	}

void MainDialog::ClearCardData() {
	userBitmap = wxNullBitmap;
	m_prevCardId = m_cardId = _T("");
	m_cardEmail = _T("");
	mPersonalData.Clear();
	havePersonalCard = true;
	SetStatusText(_("Card not found"));
	if (mEnableErrorPopup) {
		mEnableErrorPopup = false;
		doPopupError(_("Card not found"));
		}
	}

bool endsWith(wxString &comp,wxString look) {
	if (look.length() > comp.length() ) return false;
	wxString test = comp.Mid( comp.length() - look.length() );
	return test == look;
	}

wxString MainDialog::menuFromReaderName(std::string reader,std::string name) {
	wxString ret = wxString::FromAscii(reader.c_str());
	if (endsWith(ret,_T(" 0"))) ret.Truncate(ret.Len() - 2);
	if (endsWith(ret,_T(" 00"))) ret.Truncate(ret.Len() - 3);
	if (endsWith(ret,_T(" 00"))) ret.Truncate(ret.Len() - 3);
	if (name.length() >0 )
		ret += _T(" [") + wxString(name.c_str(),mIdConv) + _T("]");
	return ret;
	}

bool MainDialog::FindCard(CardBase &card) {
	if (!mCardManager) return false;

	int firstCardIndex = -1,totalCards = 0;
	size_t numReaders = 0;
	try {
		numReaders = mCardManager->getReaderCount();
		if (numReaders == 0 ) {
			doShowError(_("There are no smart card readers installed"));
			return false;
			}
		for(unsigned int i = 0;i < numReaders;i++) {
			std::string rd = mCardManager->getReaderName(uint(i));
			if (card.isInReader(i)) {
				EstEidCard tmpCard(*mCardManager);
				tmpCard.connect(i);
				wxMenuItem * item = readerMenu->FindItem(rIDs[i]);
				if (item) {
					item->SetText(
						menuFromReaderName(rd,
							tmpCard.readCardName()));
					}

				totalCards++;
				if (firstCardIndex == -1 ) firstCardIndex = i;
				}
			}
		if (totalCards == 0 ) {
			ClearCardData();
			return false;
			}
		if (mSelectedReader == -1)  //user hasnt picked anything
			mSelectedReader = firstCardIndex;

		if (card.isInReader(mSelectedReader)) {
			card.connect(mSelectedReader);
			//make sure the card works ( hack for Multos )
			EstEidCard *p = (EstEidCard *)&card;
			p->readCardID();
			//reflect menu state with our pick
			ReflectSelectedReader();
			return true;
			}
		ClearCardData();
	} catch (std::runtime_error &err){
		doShowError(err);
		}
	return false;
	}

wxString fromX509Data(ByteVec val) {
	wxString ret;
	if (val.end() != std::find(val.begin(),val.end(),0)) {//contains zeroes, is bigendian UCS
		for(ByteVec::iterator it = val.begin(); it < val.end(); it+=2)
			 iter_swap(it,it+1);
		val.push_back(0);val.push_back(0);
		return (wchar_t *) &val[0];
	} else
		return wxString((const char*)&val[0],wxConvUTF8,val.size());
	}

void MainDialog::ReloadCardData(int lastRecord,bool withCert) {
	havePersonalCard = true;
	if (!mCardManager) return;

	EstEidCard card(*mCardManager);
	if (!FindCard(card)) return;
	SetStatusText(_("Reading data.."));

	std::vector<std::string> tmp;
	try {
		card.readPersonalData(tmp,EstEidCard::SURNAME,lastRecord);
		mPersonalData.Clear();
		mPersonalData.Add(_T(""));
		for(int i = EstEidCard::SURNAME;i <= lastRecord; i++ )
			mPersonalData.Add(wxString(tmp[i].c_str(),mIdConv));

		m_cardId = mPersonalData[EstEidCard::ID];
		havePersonalCard = !m_cardId.StartsWith(_T("90000"));
		doUpdateMenus();
		if (!withCert) {
			SetStatusText(_T(""));
			return;
			}
		Refresh();
		SetStatusText(_("Reading certificate.."));
		ByteVec certBytes = card.getAuthCert();
		std::stringstream dummy;
//		std::ofstream decodeLog("decode.log");
		asnCertificate cert(certBytes,dummy);
		m_cardEmail = wxString::FromAscii(cert.getSubjectAltName().c_str());
		subjCN		= fromX509Data(cert.getSubjectCN());
		subjO		= fromX509Data(cert.getSubjectO());
		subjOU		= fromX509Data(cert.getSubjectOU());
		validFrom	= wxString::FromAscii(cert.getValidFrom().c_str());
		validTo		= wxString::FromAscii(cert.getValidTo().c_str());

		wxString issuerCN = fromX509Data(cert.getIssuerCN());
		wxString issuerO = fromX509Data(cert.getIssuerO());
		if (issuerCN.StartsWith(_T("KLASS3"))
				&& issuerO == _T("AS Sertifitseerimiskeskus"))
			havePersonalCard = false;
		doUpdateMenus();

		SetStatusText(_T(""));
		if (mPrevSelectedReader == mSelectedReader &&
			m_prevCardId == m_cardId) return; //same card and reader, no nags

		m_prevCardId = m_cardId;
		userBitmap = wxNullBitmap;
		mPrevSelectedReader = mSelectedReader;
		SetStatusText(_("Checking card status.."));
		byte puk,pinAuth,pinSign;
		card.getRetryCounts(puk,pinAuth,pinSign);
		wxString blockMsg;
		if (puk == 0)
			blockMsg += _("The PUK code of this card is blocked\n");
		if (pinAuth == 0)
			blockMsg += _("Authentication PIN (PIN 1) of this card is blocked\n");
		if (pinSign == 0)
			blockMsg += _("Signature PIN (PIN 2) of this card is blocked\n");
		if (blockMsg.Length() > 0)
			doPopupError(blockMsg);

		int answer = wxCANCEL;
		if (!cert.isTimeValid())
			answer = wxMessageBox(
				_("Your certificates are expired. Start the renewal now ?")
				,getAppName(),wxYES_NO | wxCENTRE |wxICON_ERROR);
		else if (!cert.isTimeValid(14))
			answer = wxMessageBox(
				_("Your certificates will expire in two weeks. Start the renewal now ?")
				,getAppName(),wxYES_NO | wxCENTRE |wxICON_ERROR);
		if (answer == wxYES )
			doLaunchUrl(_T("http://www.sk.ee/id-uuendus"));
		else if (answer == wxCANCEL)
			doCheckCertRegistration(certBytes);

		mEnableErrorPopup = false;
		SetStatusText(_T(""));
	} catch (runtime_error &err ) {
		doShowError(err);
		}
	}

void MainDialog::doStartupChecks() {
	SwitchCardManager(AppSetting().getUseCTAPI());
	Update();
	ReloadCardData(EstEidCard::EXPIRY,true);
	Refresh();
	}

void MainDialog::OnReadData(wxCommandEvent&  event) {
	ReloadCardData(EstEidCard::EXPIRY,true);
	Refresh();
	}

void MainDialog::OnShow(wxCommandEvent&  event){
	ReloadCardData(EstEidCard::COMMENT4);
	wxDialog *dlg = new PersonalDataDialog(this,getAppName(),mPersonalData);
	dlg->ShowModal();
	}

void MainDialog::OnSaveToFile(wxCommandEvent&  event)
{
	ReloadCardData(EstEidCard::COMMENT4);
	wxFileDialog* SaveDialog = new wxFileDialog(
		this, _("Save to file"),wxEmptyString,_T("idcarddata.txt"),
		_T("*.txt"), wxFD_SAVEFLAGS);
	if (SaveDialog->ShowModal() == wxID_OK ) {
		PersonalDataDialog::SaveFile(SaveDialog->GetPath(),mPersonalData);
		}
}

void MainDialog::OnDiagnostics(wxCommandEvent&  event)
{
	wxDialog *dlg = new DiagnosticsDialog(this,getAppName());
	dlg->ShowModal();
}

void MainDialog::OnSysinfo(wxCommandEvent&  event)
{
	::wxExecute(AppSetting().getInfoAppPath());
}

#ifndef __WXMSW__ //linux stubs
#include "PinDialog.h"

void getPassword(void *appdata,std::string prompt,int maxinput,std::string &pin) {
    MainDialog *m_dlg = (MainDialog *)appdata;
	pin = "";
	PinDialog *dlg = new PinDialog(m_dlg,m_dlg->getAppName());
	if (wxID_OK != dlg->ShowModal()) return;
	pin = dlg->mInput1.ToAscii();
	}

void MainDialog::OnCertRegister(wxCommandEvent&  event) {}

void MainDialog::doCheckCertRegistration(ByteVec &certBytes) { }

void MainDialog::doWindowsCertCheck(ByteVec &certBytes,void ** ref) { }

void MainDialog::doShowCert(ByteVec &certBytes) {
#ifdef __WXMAC__
	::wxExecute(_T("open tmp.cer"));
#else
	wxDialog *dlg = new CertificateDialog(this,getAppName(),certBytes);
	dlg->ShowModal();
#endif
	}
#endif

void MainDialog::OnShowAuthCert(wxCommandEvent&  event)
{
	try {
		if (!mCardManager) return;

		EstEidCard card(*mCardManager);
		if (!FindCard(card)) return;
		SetStatusText(_("Reading certificate.."));
		ByteVec certBytes = card.getAuthCert();
		doShowCert(certBytes);
		SetStatusText(_T(""));
	} catch(runtime_error &err) {
		doShowError(err);
		}
}

void MainDialog::OnShowSignCert(wxCommandEvent&  event)
{
	try {
		if (!mCardManager) return;

		EstEidCard card(*mCardManager);
		if (!FindCard(card)) return;
		SetStatusText(_("Reading certificate.."));
		ByteVec certBytes = card.getSignCert();
		doShowCert(certBytes);
		SetStatusText(_T(""));
	} catch(runtime_error &err) {
		doShowError(err);
		}
}

void MainDialog::doSaveCert(bool authCert) {
	if (!mCardManager) return;
	EstEidCard card(*mCardManager);
	if (!FindCard(card)) return;
	SetStatusText(_("Reading certificate.."));
	ByteVec certBytes;
	try {
		if (authCert)
			certBytes = card.getAuthCert();
		else
			certBytes = card.getSignCert();
	} catch (runtime_error &err ) {
		doShowError(err);
		return;
	}

	wxFileDialog* SaveDialog = new wxFileDialog(
		this, _("Save to file"),wxEmptyString,m_cardId + _T("_") +
		( authCert ? _T("auth") : _T("sign") )+ _T(".cer"),
		_T("DER encoded binary X.509 (*.CER)|*.cer"), wxFD_SAVEFLAGS);
	if (SaveDialog->ShowModal() == wxID_OK ) {
		std::ofstream fs(SaveDialog->GetPath().ToAscii(),std::ios_base::binary);
		fs.write((const char*)&certBytes[0], std::streamsize(certBytes.size()) );
		}
	SetStatusText(_T(""));
	}

void MainDialog::OnSaveAuthCert(wxCommandEvent&  event)
{
	doSaveCert(true);
}

void MainDialog::OnSaveSignCert(wxCommandEvent&  event)
{
	doSaveCert(false);
}

void MainDialog::OnVerifyCerts(wxCommandEvent&  event) {
	doLaunchUrl(_T("http://www.sk.ee/id-kontroll/"));
}

void MainDialog::doLaunchUrl(wxString url) {
	int result = 0;
#ifdef __WXMSW__
	wxRegKey ie(_T("HKEY_CLASSES_ROOT\\Applications\\iexplore.exe")
		_T("\\shell\\open\\command"));
	if (ie.Exists() && ie.Open(wxRegKey::Read)) {
		wxString path;
		ie.QueryValue(_T(""),path);
		if (endsWith(path,_T("%1"))) path.Truncate(path.Len() - 2);
		result = wxExecute(path + _T(" ") + url);
		}
#endif
	if (!result)
		wxLaunchDefaultBrowser(url);
	}

