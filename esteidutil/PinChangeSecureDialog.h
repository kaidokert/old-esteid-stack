/*!
	\file		PinChangeSecureDialog.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#pragma once

class PinChangeSecureDialog :
	public wxDialog
{
protected:
	wxTextCtrl* m_textExplain;
	wxStaticText* m_labelOld;
	wxStaticText* m_labelNew;
	wxButton* mBtnOk;
	wxButton* mBtnCancel;

	void OnSelect(wxCommandEvent& );

	DECLARE_EVENT_TABLE()

public:
	PinChangeSecureDialog(wxWindow *parent,wxString title, int type);
	wxChoice* mChoiceOld;
	wxChoice* mChoiceNew;
};
