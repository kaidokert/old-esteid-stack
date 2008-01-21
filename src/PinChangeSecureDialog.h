/*!
	\file		PinChangeSecureDialog.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-26 12:27:14 +0200 (Mon, 26 Nov 2007) $
*/
// Revision $Revision: 149 $
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

	DECLARE_EVENT_TABLE();

public:
	PinChangeSecureDialog(wxWindow *parent,wxString title, int type);
	wxChoice* mChoiceOld;
	wxChoice* mChoiceNew;
};
