/*!
	\file		CertificateDialog.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#pragma once

#include <wx/notebook.h>
#include <wx/statline.h>
#include <wx/listctrl.h>

class CertificateDialog:
	public wxDialog
{
public:
	CertificateDialog(wxWindow *parent,wxString title,
		std::vector<unsigned char> &cert);
	~CertificateDialog(void);
protected:
	wxNotebook* m_notebook1;
	wxPanel* m_panel1;
	wxPanel* m_panel4;
	wxStaticBitmap* m_bitmap1;
	wxStaticText* m_staticText2;
	wxStaticLine* m_staticline1;
	wxStaticText* m_staticText1;
	wxTextCtrl* m_textCtrl1;
	wxStaticLine* m_staticline2;
	wxStaticText* m_staticText3;
	wxStaticText* m_staticText6;
	wxStaticText* m_staticText4;
	wxStaticText* m_staticText7;
	wxStaticText* m_staticText5;
	wxStaticText* m_staticText8;
	wxStaticText* m_staticText9;
	wxStaticText* m_staticText10;
	wxButton* m_button2;
	wxButton* m_button3;
	wxPanel* m_panel2;
	wxStaticText* m_staticText11;
	wxListCtrl* m_listCtrl1;
	wxTextCtrl* m_textCtrl2;
	wxButton* m_button4;
	wxButton* m_button5;
	wxButton* m_button1;

	void OnInitDialog(wxInitDialogEvent& event);
	DECLARE_EVENT_TABLE()
	std::vector<unsigned char> &certBytes;
/*	asnCertificate cert;
	std::ostringstream strbuf;*/
};
