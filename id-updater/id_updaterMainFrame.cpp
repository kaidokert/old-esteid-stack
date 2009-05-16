#include "precompiled.h"
#include "id_updaterMainFrame.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/socket.h>

#define PRODUCTS "products.xml"

void id_updaterMainFrame::addProductPanel(wxXmlNode *node) {
	m_productPanel = new wxPanel( m_productsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerPanel;
	bSizerPanel = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( m_productPanel, wxID_ANY,
		node->GetPropVal(wxT("ProductName"),wxT("productName"))
		, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizerPanel->Add( m_staticText2, 1, wxALL, 10 );
	
	wxBoxSizer* bSizerVertMain;
	bSizerVertMain = new wxBoxSizer( wxVERTICAL );
	
	m_productDescription = new wxStaticText( m_productPanel, wxID_ANY, 
		_("productDescription"),
		wxDefaultPosition, wxDefaultSize, 0 );
	m_productDescription->Wrap( -1 );
	bSizerVertMain->Add( m_productDescription, 0, wxALL, 10 );
	
	wxBoxSizer* bSizerUp;
	bSizerUp = new wxBoxSizer( wxHORIZONTAL );
	
	wxString installedVersion,productVersion;
	installer.getInstalledVersion(node->GetPropVal(wxT("UpgradeCode"),wxT("")),installedVersion);
	productVersion = node->GetPropVal(wxT("ProductVersion"),wxT("none"));

	m_installedText = new wxStaticText( m_productPanel, wxID_ANY, installedVersion, wxDefaultPosition, wxDefaultSize, 0 );
	m_installedText->Wrap( -1 );
	bSizerUp->Add( m_installedText, 0, wxALL, 5 );

	m_availableText = new wxStaticText( m_productPanel, wxID_ANY, 
		_(" available version:") + productVersion
		, wxDefaultPosition, wxDefaultSize, 0 );
	m_availableText->Wrap( -1 );
	bSizerUp->Add( m_availableText, 0, wxALL, 5 );

	m_downloadText = new wxStaticText( m_productPanel, wxID_prodDownloadStatus, _("downloadStatus"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_downloadText->Wrap( -1 );
	bSizerUp->Add( m_downloadText, 1, wxALL|wxEXPAND, 5 );

	bSizerVertMain->Add( bSizerUp, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerLow;
	bSizerLow = new wxBoxSizer( wxHORIZONTAL );
	
	bool installable = productVersion != installedVersion;
	m_comboAction = new wxComboBox( m_productPanel, wxID_prodCombo, 
		installable ? _("Install")  : _("No action")
		, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	if (installable) m_comboAction->Append( _("Install") );
	m_comboAction->Append( _("Update") );
	m_comboAction->Append( _("No action") );
	if (installedVersion != "none") m_comboAction->Append( _("Uninstall") );
	bSizerLow->Add( m_comboAction, 0, wxALL, 5 );
	
	m_progressGauge = new wxGauge( m_productPanel, wxID_prodProgressGauge, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	bSizerLow->Add( m_progressGauge, 1, wxEXPAND, 5 );
	
	bSizerVertMain->Add( bSizerLow, 1, wxEXPAND, 5 );
	
	bSizerPanel->Add( bSizerVertMain, 5, 0, 5 );
	
	m_productPanel->SetSizer( bSizerPanel );
	m_productPanel->Layout();
	bSizerPanel->Fit( m_productPanel );
	wxWindow *l_productsPanel = FindWindowById(wxID_productsPanel);
//	wxWindowList& mList = l_productsPanel ->GetChildren();
//	wxWindow *pSizer = mList.GetFirst()->GetData();
	
	wxSizer * pSz = l_productsPanel ->GetSizer();
	pSz->Add( m_productPanel, 0, wxEXPAND, 5 );

	l_productsPanel->SetSizer( pSz );
	l_productsPanel->Layout();
	pSz ->Fit( l_productsPanel );

	this->Layout();
	this->Fit();
}

id_updaterMainFrame::id_updaterMainFrame( wxWindow* parent, wxString baseUrl  )
:
MainFrame( parent ),
m_baseURI(baseUrl),
m_downloadingXml(false),m_downloadIndex(-1),
m_curGauge(m_xmlGauge),m_curDownloadText(NULL)
{
	SetAutoLayout(true);
	wxSocketBase::Initialize();
	m_dThread = new wxDownloadThread(this);
	m_dThread->Create();
	m_dThread->Run();
	Connect(wxEVT_COMMAND_DOWNLOAD_COMPLETE,
		wxCommandEventHandler( id_updaterMainFrame::OnDownloadComplete),NULL,this);
	clearProducts();
	m_basePath = wxStandardPaths::Get().GetUserDataDir();
	if (!wxFileName::DirExists(m_basePath))
		wxFileName::Mkdir(m_basePath,wxPATH_MKDIR_FULL);
}

id_updaterMainFrame::~id_updaterMainFrame() {
	if (m_dThread) delete m_dThread;
}

void id_updaterMainFrame::OnClose( wxCloseEvent& event ){ 
    if (m_dThread->IsPaused()) m_dThread->Run();           // we need the thread running if we want to delete it !
    if (m_dThread->IsRunning()) m_dThread->Delete();
	Destroy();
	}

void id_updaterMainFrame::loadProducts() {
	m_xmlDoc.Load(m_basePath + "/" + PRODUCTS);
	if (m_xmlDoc.GetRoot()->GetName() != wxT("products")) return;
	for(
		wxXmlNode *child = m_xmlDoc.GetRoot()->GetChildren();
		child;
		child = child->GetNext()) {
			addProductPanel(child);
		}
}

void id_updaterMainFrame::OnDownloadComplete(wxCommandEvent &)
{
   if (!m_dThread->DownloadWasSuccessful()) {
//        wxWebUpdateInstaller::Get()->ShowErrorMsg(wxT("Could not download the ") + m_dThread->m_strResName +
//                wxT(" from\n\n") + m_dThread->m_strURI + wxT("\n\nURL... "));
   } else {
	   m_curGauge->SetValue(m_curGauge->GetRange());
	   if (m_downloadingXml) 
		   loadProducts();
	   else {
		   m_downloadIndex++;
		   startNextDownload();
		}
       wxLogUsrMsg(wxT("wxWebUpdateDlg::OnDownloadComplete - Download status: successfully completed"));
	}
   m_downloadingXml = false;
}

void id_updaterMainFrame::OnCheckUpdates( wxCommandEvent& event ) {
	clearProducts();
	m_downloadingXml = true;
	m_dThread->m_strURI = m_baseURI + PRODUCTS;
	m_dThread->m_strOutput = m_basePath + "/" + PRODUCTS;
	m_dThread->BeginNewDownload();
    m_xmlGauge->SetValue(0);
    unsigned long m_size = wxGetSizeOfURI(m_dThread->m_strURI);
    m_xmlGauge->SetRange(m_size);
	m_curGauge = m_xmlGauge;
	}

void id_updaterMainFrame::OnCancel( wxCommandEvent& event ) {
    if (m_dThread->IsDownloading()) {
        m_dThread->AbortDownload();
	    }
	}

void id_updaterMainFrame::clearProducts() {
	this->FindWindowById(wxID_productsPanel)->GetSizer()->Clear(true);
	Fit();
	}

void id_updaterMainFrame::OnClickAdd( wxCommandEvent& event ) {
	addProductPanel(m_xmlDoc.GetRoot()->GetChildren()->GetNext() );
}

void id_updaterMainFrame::OnClick( wxCommandEvent& event ) {
	InstallChecker check;
//	check.startChecking();
}

void id_updaterMainFrame::startNextInstall() {
	int i = 0;
	wxXmlNode *child = NULL;
	for(child = m_xmlDoc.GetRoot()->GetChildren(); 
		i < m_installIndex && child;	
		child = child->GetNext(),i++) {}
	if (!child) return;

	wxWindowList& mList = FindWindowById(wxID_productsPanel)->GetChildren();
	wxWindow *panel = mList.Item(m_installIndex)->GetData();
	wxComboBox * box  = (wxComboBox *)panel->FindWindow(wxID_prodCombo);
	if (box->GetValue() == "Install") {
		wxString fileName = child->GetPropVal(wxT("filename"),wxT(""));
		wxString downloadedFile = m_basePath + "\\" + fileName;
		installer.installMsi(downloadedFile);
		}
	m_installIndex++;
	startNextInstall();
	}

void id_updaterMainFrame::startNextDownload() {
	int i = 0;
	wxXmlNode *child = NULL;
	for(child = m_xmlDoc.GetRoot()->GetChildren(); 
		i < m_downloadIndex && child;	
		child = child->GetNext(),i++) {}
	if (!child) {
		m_installIndex = 0;
		startNextInstall();
		return;
		}
	wxString fileName = child->GetPropVal(wxT("filename"),wxT(""));

	wxWindowList& mList = FindWindowById(wxID_productsPanel)->GetChildren();
	wxWindow *panel = mList.Item(m_downloadIndex)->GetData();
	wxComboBox * box  = (wxComboBox *)panel->FindWindow(wxID_prodCombo);
	if (box->GetValue() == "No action") {
		m_downloadIndex++;
		startNextDownload();
		return;
		}

	m_curGauge  = (wxGauge *)panel->FindWindow(wxID_prodProgressGauge);
	m_curDownloadText = (wxStaticText *)panel->FindWindow(wxID_prodDownloadStatus);

	m_dThread->m_strURI = m_baseURI + fileName;
	m_dThread->m_strOutput = m_basePath + "/" + fileName;
	m_dThread->BeginNewDownload();
    m_curGauge->SetValue(0);
    unsigned long m_size = wxGetSizeOfURI(m_dThread->m_strURI);
    m_curGauge->SetRange(m_size);
	}

void id_updaterMainFrame::downloadClick( wxCommandEvent& event ) {
	m_downloadIndex = 0;
	startNextDownload();
	}

void id_updaterMainFrame::OnUpdateUI( wxUpdateUIEvent& event ) {
	if (!m_dThread->IsDownloading()) return;
    static wxDateTime lastupdate = wxDateTime::UNow();
    wxDateTime current = wxDateTime::UNow();
    wxTimeSpan diff = current - lastupdate;
    if (diff.GetMilliseconds().ToLong() > 500) {
        lastupdate = current;
        long value = m_dThread->GetCurrDownloadedBytes();
        m_curGauge->SetValue(value >= 0 ? value : 0);
		if (m_curDownloadText) {
			m_curDownloadText->SetLabel("Speed: " + m_dThread->GetDownloadSpeed() +
				" Time left:" + m_dThread->GetRemainingTime() );
			}
		}
}
