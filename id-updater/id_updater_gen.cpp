///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "precompiled.h"

#include "id_updater_gen.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 400,-1 ), wxDefaultSize );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_headerPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkUpdatesButton = new wxButton( m_headerPanel, wxID_ANY, _("Check for updates"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_checkUpdatesButton, 0, wxALL, 5 );
	
	m_xmlGauge = new wxGauge( m_headerPanel, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	bSizer6->Add( m_xmlGauge, 1, wxALL, 5 );
	
	bSizer5->Add( bSizer6, 1, wxEXPAND, 5 );
	
	m_updateStatus = new wxStaticText( m_headerPanel, wxID_ANY, _("Update status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_updateStatus->Wrap( -1 );
	bSizer5->Add( m_updateStatus, 0, wxALL|wxEXPAND, 5 );
	
	m_headerPanel->SetSizer( bSizer5 );
	m_headerPanel->Layout();
	bSizer5->Fit( m_headerPanel );
	bSizer3->Add( m_headerPanel, 0, wxEXPAND, 5 );
	
	m_productsPanel = new wxPanel( this, wxID_productsPanel, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_productPanel = new wxPanel( m_productsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerPanel;
	bSizerPanel = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText2 = new wxStaticText( m_productPanel, wxID_ANY, _("productName"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizerPanel->Add( m_staticText2, 1, wxALL, 10 );
	
	wxBoxSizer* bSizerVertMain;
	bSizerVertMain = new wxBoxSizer( wxVERTICAL );
	
	m_productDescription = new wxStaticText( m_productPanel, wxID_ANY, _("productDescription"), wxDefaultPosition, wxDefaultSize, 0 );
	m_productDescription->Wrap( -1 );
	bSizerVertMain->Add( m_productDescription, 0, wxALL, 10 );
	
	wxBoxSizer* bSizerUp;
	bSizerUp = new wxBoxSizer( wxHORIZONTAL );
	
	m_installedText = new wxStaticText( m_productPanel, wxID_ANY, _("installedVer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_installedText->Wrap( -1 );
	bSizerUp->Add( m_installedText, 0, wxALL, 5 );
	
	m_availableText = new wxStaticText( m_productPanel, wxID_ANY, _("availableVer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_availableText->Wrap( -1 );
	bSizerUp->Add( m_availableText, 0, wxALL, 5 );
	
	m_downloadText = new wxStaticText( m_productPanel, wxID_prodDownloadStatus, _("downloadStatus"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_downloadText->Wrap( -1 );
	bSizerUp->Add( m_downloadText, 1, wxALL|wxEXPAND, 5 );
	
	bSizerVertMain->Add( bSizerUp, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerLow;
	bSizerLow = new wxBoxSizer( wxHORIZONTAL );
	
	m_comboAction = new wxComboBox( m_productPanel, wxID_prodCombo, _("Install"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_comboAction->Append( _("Install") );
	m_comboAction->Append( _("Update") );
	m_comboAction->Append( _("No action") );
	m_comboAction->Append( _("Uninstall") );
	bSizerLow->Add( m_comboAction, 0, wxALL, 5 );
	
	m_progressGauge = new wxGauge( m_productPanel, wxID_prodProgressGauge, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	bSizerLow->Add( m_progressGauge, 1, wxEXPAND, 5 );
	
	bSizerVertMain->Add( bSizerLow, 1, wxEXPAND, 5 );
	
	bSizerPanel->Add( bSizerVertMain, 5, 0, 5 );
	
	m_productPanel->SetSizer( bSizerPanel );
	m_productPanel->Layout();
	bSizerPanel->Fit( m_productPanel );
	bSizer15->Add( m_productPanel, 0, wxEXPAND, 5 );
	
	m_productPanel1 = new wxPanel( m_productsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerPanel1;
	bSizerPanel1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText21 = new wxStaticText( m_productPanel1, wxID_ANY, _("productName"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	bSizerPanel1->Add( m_staticText21, 1, wxALL, 10 );
	
	wxBoxSizer* bSizerVertMain1;
	bSizerVertMain1 = new wxBoxSizer( wxVERTICAL );
	
	m_productDescription1 = new wxStaticText( m_productPanel1, wxID_ANY, _("productDescription"), wxDefaultPosition, wxDefaultSize, 0 );
	m_productDescription1->Wrap( -1 );
	bSizerVertMain1->Add( m_productDescription1, 0, wxALL, 10 );
	
	wxBoxSizer* bSizerUp1;
	bSizerUp1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_installedText1 = new wxStaticText( m_productPanel1, wxID_ANY, _("installedVer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_installedText1->Wrap( -1 );
	bSizerUp1->Add( m_installedText1, 0, wxALL, 5 );
	
	m_availableText1 = new wxStaticText( m_productPanel1, wxID_ANY, _("availableVer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_availableText1->Wrap( -1 );
	bSizerUp1->Add( m_availableText1, 0, wxALL, 5 );
	
	bSizerVertMain1->Add( bSizerUp1, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerLow1;
	bSizerLow1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_comboAction1 = new wxComboBox( m_productPanel1, wxID_ANY, _("Install"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_comboAction1->Append( _("Install") );
	m_comboAction1->Append( _("Update") );
	m_comboAction1->Append( _("No action") );
	m_comboAction1->Append( _("Uninstall") );
	bSizerLow1->Add( m_comboAction1, 0, wxALL, 5 );
	
	m_progressGauge1 = new wxGauge( m_productPanel1, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	bSizerLow1->Add( m_progressGauge1, 1, wxEXPAND, 5 );
	
	bSizerVertMain1->Add( bSizerLow1, 1, wxEXPAND, 5 );
	
	bSizerPanel1->Add( bSizerVertMain1, 5, 0, 5 );
	
	m_productPanel1->SetSizer( bSizerPanel1 );
	m_productPanel1->Layout();
	bSizerPanel1->Fit( m_productPanel1 );
	bSizer15->Add( m_productPanel1, 0, wxEXPAND, 5 );
	
	m_productsPanel->SetSizer( bSizer15 );
	m_productsPanel->Layout();
	bSizer15->Fit( m_productsPanel );
	bSizer3->Add( m_productsPanel, 0, 0, 5 );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_downloadButton = new wxButton( this, wxID_ANY, _("Download and execute"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_downloadButton, 0, wxALL, 5 );
	
	m_button3 = new wxButton( this, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_button3, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 1, 0, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btnCheck = new wxButton( this, wxID_ANY, _("DebugCheck"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_btnCheck, 0, wxALL, 5 );
	
	m_button5 = new wxButton( this, wxID_ANY, _("DebugAdd"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_button5, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer16, 1, wxEXPAND, 5 );
	
	bSizer3->Add( bSizer1, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer3 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	this->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrame::OnUpdateUI ) );
	m_checkUpdatesButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnCheckUpdates ), NULL, this );
	m_downloadButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::downloadClick ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnCancel ), NULL, this );
	m_btnCheck->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClick ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickAdd ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	this->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MainFrame::OnUpdateUI ) );
	m_checkUpdatesButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnCheckUpdates ), NULL, this );
	m_downloadButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::downloadClick ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnCancel ), NULL, this );
	m_btnCheck->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClick ), NULL, this );
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickAdd ), NULL, this );
}
