/*!
	\file		PinChangeDialog.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#pragma once

enum pinOps {
	CHANGEAUTH,
	CHANGESIGN,
	CHANGEPUK,
	UNBLOCKAUTH,
	UNBLOCKSIGN
};

class PinChangeDialog :
	public wxDialog
{
	wxTextCtrl *CreateInput(wxBoxSizer *pSizer, wxString label,wxString *validate);
	wxTextCtrl *mText1,*mText2,*mText3;
	wxButton *mBtnOk,*mBtnCancel;
	unsigned int mMinPinLen1,mMinPinLen2,mMaxPinLen;

	DECLARE_EVENT_TABLE()
	void OnText(wxCommandEvent& );
public:
	PinChangeDialog(wxWindow *parent,wxString title, int type);
	wxString mInput1,mInput2,mInput3;
};
