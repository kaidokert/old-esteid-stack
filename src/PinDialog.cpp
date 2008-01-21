/*!
	\file		PinDialog.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-27 00:54:07 +0200 (Tue, 27 Nov 2007) $
*/
// Revision $Revision: 162 $
#include "precompiled.h"
#include "PinDialog.h"

#ifndef __WXMSW__
#include "resources/auth_icon.xpm"
#endif

BEGIN_EVENT_TABLE(PinDialog, wxDialog)
    EVT_TEXT(wxID_ANY, PinDialog::OnText)
END_EVENT_TABLE()

void PinDialog::OnText(wxCommandEvent& WXUNUSED(event))
{
	bool isValid = true;
	if (m_pinEntry->GetValue().Len() < 4) isValid = false; //require minimum PIN len
	if (m_pinEntry->GetValue().Len() > 12) isValid = false; //allow max PIN len
	mBtnOk->Enable(isValid);
	if (isValid) 
		mBtnOk->SetDefault();
	else
		mBtnCancel->SetDefault();
}

PinDialog::PinDialog( wxWindow *parent,wxString title )
:wxDialog(parent,wxID_ANY,title,wxDefaultPosition,
		wxDefaultSize)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	bSizer20->SetMinSize(wxSize( 280,-1 )); 
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer21->Add( 18, 0, 0, 0, 0 );
	
	m_icon = new wxStaticBitmap( this, wxID_ANY, 
		wxICON(auth_icon), wxDefaultPosition, wxDefaultSize, 0 );

	bSizer21->Add( m_icon, 0, wxALL, 0 );
	
	m_pinLabel = new wxStaticText( this, wxID_ANY, wxT("Enter PIN for authentication (PIN 1)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_pinLabel, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer20->Add( bSizer21, 1, wxEXPAND, 5 );
	
	m_pinEntry = new wxTextCtrl( this, wxID_ANY, 
		wxEmptyString, wxDefaultPosition, wxDefaultSize, 
		wxTE_PASSWORD,wxTextValidator(wxFILTER_NUMERIC, &mInput1 ));
	bSizer20->Add( m_pinEntry, 0, wxEXPAND|wxRIGHT|wxLEFT, 30 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer22->Add( 0, 0, 1, 0, 0 );
	
	mBtnOk = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer22->Add( mBtnOk, 0, wxALL, 7 );
	mBtnOk->Enable(false);
	
	mBtnCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer22->Add( mBtnCancel, 0, wxALL, 7 );
	
	bSizer22->Add( 20, 0, 0, wxEXPAND, 0 );
	
	bSizer20->Add( bSizer22, 1, wxEXPAND|wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer20 );
	this->Layout();
	bSizer20->Fit( this );
}
