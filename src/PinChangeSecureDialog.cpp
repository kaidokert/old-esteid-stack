/*!
	\file		PinChangeDialog.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-26 12:27:14 +0200 (Mon, 26 Nov 2007) $
*/
// Revision $Revision: 149 $
#include "precompiled.h"
#include "PinChangeDialog.h" //pull in PINOPS
#include "PinChangeSecureDialog.h"

enum {
	idChoiceOld,
	idChoiceNew
};

BEGIN_EVENT_TABLE(PinChangeSecureDialog, wxDialog)
	EVT_CHOICE(idChoiceOld,PinChangeSecureDialog::OnSelect)
	EVT_CHOICE(idChoiceNew,PinChangeSecureDialog::OnSelect)
END_EVENT_TABLE()

void PinChangeSecureDialog::OnSelect(wxCommandEvent& evt) {
	wxString s1 = mChoiceOld->GetStringSelection();
	wxString s2 = mChoiceNew->GetStringSelection();
	bool isValid = (s1.Len() > 0 && s2.Len());
	mBtnOk->Enable(isValid);
	if (isValid) 
		mBtnOk->SetDefault();
	else
		mBtnCancel->SetDefault();
	}

PinChangeSecureDialog::PinChangeSecureDialog(wxWindow *parent,wxString title, int type) :
	wxDialog(parent,wxID_ANY,title,wxDefaultPosition,
		wxSize(450,200))
{
	wxString prompt1,prompt2;
	int mMaxPinLen = 12,mMinPinLen1,mMinPinLen2;
	wxString mChoices[] = { wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10"), wxT("11"),wxT("12") };
	switch (type) {
		case CHANGEAUTH :
			mMinPinLen1 = mMinPinLen2 = 4;
			prompt1 = _("Current authentication PIN length");
			prompt2 = _("New authentication PIN (PIN 1) length");
			break;
		case CHANGESIGN:
			mMinPinLen1 = mMinPinLen2 =  5;
			prompt1 = _("Current signature PIN length");
			prompt2 = _("New signature PIN (PIN 2) length");
			break;
		case CHANGEPUK:
			mMinPinLen1 = mMinPinLen2 = 8;
			prompt1 = _("Current PUK length");
			prompt2 = _("New PUK length");
			break;
		case UNBLOCKAUTH :
			mMinPinLen1 = 8;
			mMinPinLen2 = 4;
			prompt1 = _("Length of PUK code");
			prompt2 = _("New authentication PIN (PIN 1) length");
			break;
		case UNBLOCKSIGN:
			mMinPinLen1 = 8;
			mMinPinLen2 = 5;
			prompt1 = _("Length of PUK code");
			prompt2 = _("New signature PIN (PIN 2) length");
			break;
		}

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );

	wxString prompt;
	if ((type == UNBLOCKAUTH ) || (type == UNBLOCKSIGN) ) 
		prompt = _("To unblock PIN code on Secure PINpad reader the PUK code and new PIN length have to be entered first. For example if Your PUK code is '17777777' and for new PIN You would enter '77711', then You should choose 8 and 5 accordingly.");
	else
		prompt = _("To change PIN or PUK code on Secure PINpad reader the current PIN and new PIN length have to be entered first. For example if Your current authentication PIN is '1777' and for new PIN You would enter '77711', then You should choose 4 and 5 accordingly.");

	m_textExplain = new wxTextCtrl( this, wxID_ANY, 
		prompt, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxNO_BORDER );
	m_textExplain->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	//m_textExplain->SetMinSize( wxSize( -1, 20) );
	
	bSizer15->Add( m_textExplain, 1, wxALL|wxEXPAND, 10 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	m_labelOld = new wxStaticText( this, wxID_ANY, prompt1, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_labelOld, 0, wxALL|wxEXPAND, 5 );

	int mChoiceOldNChoices = sizeof( mChoices ) / sizeof( wxString ) - mMinPinLen1 + 4;
	wxString *mChoiceOldChoices = mChoices + (mMinPinLen1 - 4);
	mChoiceOld = new wxChoice( this, idChoiceOld, wxDefaultPosition, wxDefaultSize, mChoiceOldNChoices, mChoiceOldChoices, 0 );
	bSizer19->Add( mChoiceOld, 0, wxALL|wxEXPAND, 5 );
	
	bSizer16->Add( bSizer19, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	m_labelNew = new wxStaticText( this, wxID_ANY, prompt2, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( m_labelNew, 0, wxALL|wxEXPAND, 5 );
	
	int mChoiceNewNChoices = sizeof( mChoices ) / sizeof( wxString ) - mMinPinLen2 + 4;
	wxString *mChoiceNewChoices = mChoices + (mMinPinLen2 - 4);
	mChoiceNew = new wxChoice( this, idChoiceNew, wxDefaultPosition, wxDefaultSize, mChoiceNewNChoices, mChoiceNewChoices, 0 );
	bSizer21->Add( mChoiceNew, 0, wxALL|wxEXPAND, 5 );
	
	bSizer16->Add( bSizer21, 1, wxEXPAND, 5 );
	
	bSizer15->Add( bSizer16, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer18->Add( 0, 0, 1, wxEXPAND, 0 );
	
	mBtnOk = new wxButton( this, wxID_OK, _("OK"));
	bSizer18->Add( mBtnOk, 5, wxALL, 5 );
	
	mBtnCancel = new wxButton( this, wxID_CANCEL, _("Cancel"));
	bSizer18->Add( mBtnCancel, 5, wxALL, 5 );
	
	bSizer18->Add( 0, 0, 1, wxEXPAND, 0 );
	
	bSizer15->Add( bSizer18, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer15 );
	this->Layout();
	wxCommandEvent dummy;
	OnSelect(dummy);
}
