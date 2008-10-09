/*!
	\file		DiagnosticsDialog.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#pragma once
#include "cardlib/ManagerInterface.h"

class DiagnosticsDialog :
	public wxDialog
{
	bool m_haveCardmodule;
	bool listFileVersion(wxString desc,wxString libName,int libVer = 1);
	void doDiagnostics();
	void SCTest(int level,ManagerInterface &cardMgr);
	void PCSCTests();
	void CTAPITests();
	void systemSpecificTests();
	void PKCS11Tests();
protected:
	wxTextCtrl* mDiagText;
	wxButton* mBtnSave;
	wxButton* mBtnClose;
	void OnMyEvent( wxEvent &event );
	void OnSaveToFile( wxCommandEvent &event );
	DECLARE_EVENT_TABLE();
public:
	void addLine(wxString line);
	DiagnosticsDialog(wxWindow *parent,wxString title);
	~DiagnosticsDialog(void);
};
