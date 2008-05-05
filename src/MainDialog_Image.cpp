/*!
	\file		MainDialog_Image.cpp
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
#include "wx/image.h"
#include <fstream>

#include "utility/netObj.h"
using std::string;
using std::runtime_error;

void saveTemp(ByteVec &contents,wxString iname) {
	wxString wxfile = wxFileName::CreateTempFileName(iname );
	std::ofstream strm(wxfile.ToAscii(),std::ios::binary);
	strm.write((const char*)&contents[0],std::streamsize(contents.size()));
	}

void findUrlFromPage(ByteVec &page,string lookFor,string &pageUrl) {
	string pageStr(page.size(),'\0');
	copy(page.begin(),page.end(),pageStr.begin());
	size_t pos = pageStr.find(lookFor);
	if ( pos == string::npos )
		throw runtime_error("Webpage contents unrecognized");
	size_t posEnd = pageStr.find('"',pos);
	if ( pos == string::npos)
		throw runtime_error("Webpage contents unrecognized");
	pageUrl = pageStr.substr(pos,posEnd - pos);
	while(string::npos != (pos = pageUrl.find("&amp;")))
		pageUrl.erase(pos+1,4);
	}

void MainDialog::OnDownloadPicture(wxCommandEvent&  event) {
	void * clientCert;
	if (!havePersonalCard) return;
	if (!mCardManager) return;

try {
	SetStatusText(_("Reading certificate.."));
	ByteVec certBytes;
	{//descope card early
		EstEidCard card(*mCardManager);
		if (!FindCard(card)) return;
		certBytes = card.getAuthCert();
	}
	//in case we were using CTAPI, it can have only one connection
	delete mCardManager;
	mCardManager = NULL;

#ifdef __WXMSW__
	doWindowsCertCheck(certBytes,&clientCert);
#endif
	SetStatusText(wxString(_("Connecting")) + _T(" ."));

	netObj net(this);
	//kludge, we should be passing cert as with winInet and doing compare
	net.init(mSelectedReader);
	string appUrl("/x/kodanik/");
	netConnect conn(net,"portaal-id.riik.ee", HTTPS ,clientCert );

	ByteVec page1,page2,picBytes;

	SetStatusText(wxString(_("Retrieving data")) + _T(" .."));
	Update();
	conn.getHttpsFile(appUrl + "index.php?fp=querylist",page1);
//	saveTemp(page1,_T("page1"));

	string page2Url,picUrl;
	findUrlFromPage(page1,"index.php?fq=kmais/kmais.kodaniku_andmed.v1&qk=",page2Url);

	SetStatusText(wxString(_("Retrieving data")) + _T(" ..."));
	Update();
	conn.getHttpsFile(appUrl + page2Url,page2);
//	saveTemp(page2,_T("page2"));

	findUrlFromPage(page2,"index.php?fp=appendix&amp;filename=",picUrl);

	SetStatusText(wxString(_("Retrieving data")) + _T(" ....")  );
	Update();
	conn.getHttpsFile(appUrl + picUrl,picBytes);
//	saveTemp(picBytes,_T("pic.jpg"));

	wxMemoryInputStream mstream( &picBytes[0],picBytes.size());
	wxImage bmp(mstream);
	if (!bmp.Ok()) throw runtime_error("Corrupted data received");
	bmp.Rescale(100,132);
	userBitmap = wxBitmap(bmp);
	SetStatusText(_("Image loaded"));
	Refresh();
} catch(runtime_error &err) {
	mEnableErrorPopup = true;
	doShowError(err);
	}

	SwitchCardManager(AppSetting().getUseCTAPI());
}
