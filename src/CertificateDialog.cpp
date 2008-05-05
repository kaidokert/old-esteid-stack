/*!
	\file		CertificateDialog.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "CertificateDialog.h"
#include "utility/asnCertificate.h"
#include "wx/mstream.h"
#include <wx/image.h>

typedef std::vector<unsigned char> ByteVec;

#ifdef __WXMSW__
bool myLoadUserResource(const wxString& resourceName, LPCTSTR resourceType,wxMemoryBuffer &buf);
#else
#include "resources/cert.cdata"
#include "resources/cert_invalid.cdata"
#endif

wxBitmap chooseBmp(bool valid) {
	unsigned char *data;size_t sz;
#ifdef __WXMSW__
	wxMemoryBuffer buff;
	myLoadUserResource(valid ? wxT("CERT") : wxT("CERT_INVALID") ,RT_RCDATA,buff);
	data = (unsigned char*)buff.GetData();
	sz = buff.GetDataLen();
#else
	if (valid){	data = cert_cdata; sz = sizeof(cert_cdata);}
	else{ data = cert_invalid_cdata; sz = sizeof(cert_invalid_cdata);}
#endif
	wxMemoryInputStream mstream(data,sz);
	wxImage bmp(mstream);
	return bmp;
	}

CertificateDialog::	CertificateDialog(wxWindow *parent,wxString title,
									  std::vector<unsigned char> &cert):
	wxDialog(parent,wxID_ANY,title,wxDefaultPosition,
		wxSize(400,490)),certBytes(cert)
{
	this->SetSizeHints( wxSize( 410,480 ), wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel1 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_panel4 = new wxPanel( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	m_panel4->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_bitmap1 = new wxStaticBitmap( m_panel4, wxID_ANY, 
		chooseBmp(true), 
		wxDefaultPosition, wxDefaultSize, 0 );
	//m_bitmap1->SetIcon(iCert);
	bSizer5->Add( m_bitmap1, 0, wxALL, 10 );
	
	m_staticText2 = new wxStaticText( m_panel4, wxID_ANY, wxT("Certificate information"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_staticText2, 0, wxALIGN_BOTTOM|wxALL, 10 );
	
	bSizer4->Add( bSizer5, 0, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_staticline1->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	m_staticline1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	m_staticline1->SetMinSize( wxSize( 330,-1 ) );
	
	bSizer4->Add( m_staticline1, 0, wxLEFT|wxRIGHT, 14 );
	
	bSizer4->Add( 0, 5, 0, wxEXPAND, 0 );
	
	m_staticText1 = new wxStaticText( m_panel4, wxID_ANY, wxT("This certificate is intended for the following purpose(s):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_staticText1, 0, wxLEFT, 12 );
	
	m_textCtrl1 = new wxTextCtrl( m_panel4, wxID_ANY, wxT("- Proves your identity\n- Protects your e-mail messages\n"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxNO_BORDER );
	m_textCtrl1->SetMinSize( wxSize( 300,100 ) );
	
	bSizer4->Add( m_textCtrl1, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticline2 = new wxStaticLine( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	m_staticline2->SetMinSize( wxSize( 330,-1 ) );
	
	bSizer4->Add( m_staticline2, 0, wxLEFT|wxRIGHT, 12 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( m_panel4, wxID_ANY, wxT("Issued to:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );
	
	bSizer7->Add( m_staticText3, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( m_panel4, wxID_ANY, wxT("KERT, KAIDO, 37834"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer7->Add( m_staticText6, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer7, 2, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText4 = new wxStaticText( m_panel4, wxID_ANY, wxT("Issued by:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );
	
	bSizer8->Add( m_staticText4, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( m_panel4, wxID_ANY, wxT("ESTEID-SK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_staticText7, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer8, 2, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5 = new wxStaticText( m_panel4, wxID_ANY, wxT("Valid from:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );
	
	bSizer9->Add( m_staticText5, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( m_panel4, wxID_ANY, wxT("9.11.2004"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_staticText8, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( m_panel4, wxID_ANY, wxT("to"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->SetFont( wxFont( 8, 74, 90, 92, false, wxT("Tahoma") ) );
	
	bSizer9->Add( m_staticText9, 0, wxALL, 5 );
	
	m_staticText10 = new wxStaticText( m_panel4, wxID_ANY, wxT("14.11.2007"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_staticText10, 0, wxALL, 5 );
	
	bSizer6->Add( bSizer9, 2, wxEXPAND, 5 );
	
	bSizer6->Add( 0, 0, 1, 0, 0 );
	
	bSizer4->Add( bSizer6, 1, wxLEFT, 40 );
	
	m_panel4->SetSizer( bSizer4 );
	m_panel4->Layout();
	bSizer4->Fit( m_panel4 );
	bSizer2->Add( m_panel4, 1, wxEXPAND | wxALL, 10 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 0 );
	
/*	m_button2 = new wxButton( m_panel1, wxID_ANY, wxT("Install Certificate..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button2, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	m_button3 = new wxButton( m_panel1, wxID_ANY, wxT("Issuer Statement"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button3, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	bSizer2->Add( bSizer3, 0, wxALIGN_RIGHT|wxEXPAND, 5 );
*/	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_notebook1->AddPage( m_panel1, wxT("General"), false );
/*	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText11 = new wxStaticText( m_panel2, wxID_ANY, wxT("Show:"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( m_staticText11, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer13, 0, wxEXPAND, 5 );
	
	m_listCtrl1 = new wxListCtrl( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxLC_REPORT );
	bSizer11->Add( m_listCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrl2 = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer11->Add( m_textCtrl2, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer12->Add( 0, 0, 1, wxEXPAND, 0 );
	
	m_button4 = new wxButton( m_panel2, wxID_ANY, wxT("Edit properties..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_button4->Enable( false );
	
	bSizer12->Add( m_button4, 0, wxALL, 7 );
	
	m_button5 = new wxButton( m_panel2, wxID_ANY, wxT("Copy to file..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_button5, 0, wxALL, 7 );
	
	bSizer11->Add( bSizer12, 0, wxEXPAND, 10 );
	
	m_panel2->SetSizer( bSizer11 );
	m_panel2->Layout();
	bSizer11->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, wxT("Details"), true ); */
	
	bSizer1->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	m_button1 = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_button1, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
}

CertificateDialog::~CertificateDialog(void)
{
}

wxString l_fromX509Data(ByteVec val) {
	wxString ret;
	if (val.end() != std::find(val.begin(),val.end(),0)) {//contains zeroes, is bigendian UCS
		for(ByteVec::iterator it = val.begin(); it < val.end(); it+=2)
			 iter_swap(it,it+1);
		val.push_back(0);val.push_back(0);
		return (wchar_t *) &val[0];
	} else
		return wxString((const char*)&val[0],wxConvUTF8,val.size());
	}

void CertificateDialog::OnInitDialog(wxInitDialogEvent& event) {
	std::ostringstream decodebuf;
	asnCertificate cert(certBytes,decodebuf);

	m_staticText6->SetLabel(l_fromX509Data(cert.getSubjectCN()));
	m_staticText7->SetLabel( l_fromX509Data(cert.getIssuerCN()));
	m_staticText8->SetLabel(wxString::FromAscii(cert.getValidFrom().c_str()));
	m_staticText10->SetLabel(wxString::FromAscii(cert.getValidTo().c_str()));
	m_textCtrl1->SetValue(_T(""));
	if (!cert.isTimeValid()) {
		//set bmp invalid
		m_bitmap1->SetBitmap(chooseBmp(false));
		m_staticText1->SetLabel(_T("- This certificate has expired or is not yet valid\n"));
		return;
		}

	if (cert.hasExtKeyUsage()) {
		if (cert.checkKeyUsage("1.3.6.1.5.5.7.3.2")) // Client Authentication
			m_textCtrl1->AppendText(_T("- Proves your identity to a remote computer\n"));
		if (cert.checkKeyUsage("1.3.6.1.5.5.7.3.4")) //Secure Email , 
			m_textCtrl1->AppendText(_T("- Protects e-mail messages\n"));
		if (cert.checkKeyUsage("3.7.33.7")) //??
			m_textCtrl1->AppendText(_T("- Removes warts and cures cancer\n"));
		}
	else 
		m_textCtrl1->SetValue(_T(" - All application policies\n"));
}

BEGIN_EVENT_TABLE(CertificateDialog,wxDialog)
	EVT_INIT_DIALOG(CertificateDialog::OnInitDialog)
END_EVENT_TABLE()
