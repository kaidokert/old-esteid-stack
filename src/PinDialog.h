/*!
	\file		PinDialog.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-26 23:42:58 +0200 (Mon, 26 Nov 2007) $
*/
// Revision $Revision: 157 $
#pragma once

class PinDialog : public wxDialog 
{
	protected:
		wxStaticBitmap* m_icon;
		wxStaticText* m_pinLabel;
		wxButton* mBtnOk;
		wxButton* mBtnCancel;
		wxTextCtrl* m_pinEntry;
		DECLARE_EVENT_TABLE();
		void OnText(wxCommandEvent& );
	public:
		wxString mInput1;
		PinDialog(wxWindow *parent,wxString title);
};

