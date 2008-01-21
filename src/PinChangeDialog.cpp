/*!
	\file		PinChangeDialog.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-26 12:27:14 +0200 (Mon, 26 Nov 2007) $
*/
// Revision $Revision: 149 $
#include "precompiled.h"
#include "PinChangeDialog.h"

#define ControlWidth 170

enum {
	idInput1,
	idInput2,
	idInput3,
};

BEGIN_EVENT_TABLE(PinChangeDialog, wxDialog)
    EVT_TEXT(wxID_ANY, PinChangeDialog::OnText)
END_EVENT_TABLE()

void PinChangeDialog::OnText(wxCommandEvent& WXUNUSED(event))
{
	bool isValid = true;
	if (mText1->GetValue().Len() < mMinPinLen1) isValid = false; //require minimum PIN len
	if (mText2->GetValue().Len() < mMinPinLen2) isValid = false;
	if (mText2->GetValue() != mText3->GetValue()) 
		isValid = false; //matching PIN entrys
	if (mText1->GetValue() == mText2->GetValue()) 
		isValid = false; //require different new PIN
	mBtnOk->Enable(isValid);
	if (isValid) 
		mBtnOk->SetDefault();
	else
		mBtnCancel->SetDefault();
}

PinChangeDialog::PinChangeDialog(wxWindow *parent,wxString title, int pinType) :
	wxDialog(parent,wxID_ANY,title,wxDefaultPosition,
		wxSize(200,200))
{
	wxString prompt1,prompt2,prompt3;
	mMaxPinLen = 12;
	switch (pinType) {
		case CHANGEAUTH :
			mMinPinLen1 = mMinPinLen2 = 4;
			prompt1 = _("Current authentication PIN (PIN 1)");
			prompt2 = _("New authentication PIN");
			prompt3 = _("Repeat new authentication PIN");
			break;
		case CHANGESIGN:
			mMinPinLen1 = mMinPinLen2 =  5;
			prompt1 = _("Current signature PIN (PIN 2)");
			prompt2 = _("New signature PIN");
			prompt3 = _("Repeat new signature PIN");
			break;
		case CHANGEPUK:
			mMinPinLen1 = mMinPinLen2 = 8;
			prompt1 = _("Current PUK");
			prompt2 = _("New PUK");
			prompt3 = _("Repeat new PUK");
			break;
		case UNBLOCKAUTH :
			mMinPinLen1 = 8;
			mMinPinLen2 = 4;
			prompt1 = _("PUK code");
			prompt2 = _("New authentication PIN (PIN 1)");
			prompt3 = _("Repeat new authentication PIN");
			break;
		case UNBLOCKSIGN:
			mMinPinLen1 = 8;
			mMinPinLen2 = 5;
			prompt1 = _("PUK code");
			prompt2 = _("New signature PIN (PIN 2)");
			prompt3 = _("Repeat new signature PIN");
			break;
		}

	wxBoxSizer* surroundSizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* vertSizer = new wxBoxSizer( wxVERTICAL );

	mText1 = CreateInput(vertSizer,prompt1,&mInput1);
	mText2 = CreateInput(vertSizer,prompt2,&mInput2);
	mText3 = CreateInput(vertSizer,prompt3,&mInput3);

	wxBoxSizer* bHoriz1 = new wxBoxSizer( wxHORIZONTAL );
	mBtnOk = new wxButton(this,wxID_OK,_("OK"));
	bHoriz1->Add(mBtnOk,0,wxALL,5);
	mBtnCancel = new wxButton(this,wxID_CANCEL,_("Cancel"));
	bHoriz1->Add(mBtnCancel,0,wxALL,5);
	vertSizer->Add(bHoriz1);

	surroundSizer->Add(vertSizer,0,wxALL,10);
	this->SetSizer( surroundSizer );
	this->Layout();
}

wxTextCtrl * PinChangeDialog::CreateInput(wxBoxSizer *pSizer, wxString label, 
							   wxString *validate) {
	wxStaticText * m_staticText = new wxStaticText( this, wxID_ANY, label);
	pSizer->Add( m_staticText,0,(wxUP | wxRIGHT | wxLEFT),2);
	wxTextCtrl * m_textCtrl = new wxTextCtrl( this, wxID_ANY,wxEmptyString,	wxDefaultPosition
		,wxDefaultSize,wxTE_PASSWORD,wxTextValidator(wxFILTER_NUMERIC,validate));
	m_textCtrl->SetMaxLength(mMaxPinLen);
	m_textCtrl->SetMinSize( wxSize( ControlWidth,-1 ) );
	pSizer->Add(m_textCtrl,0, wxALL,2);
	return m_textCtrl;
	}

