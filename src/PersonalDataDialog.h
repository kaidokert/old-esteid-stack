/*!
	\file		PersonalDataDialog.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#pragma once

class PersonalDataDialog :
	public wxDialog
{
public:
	PersonalDataDialog(wxWindow *parent,wxString title,wxArrayString &mPersonalData);
	~PersonalDataDialog(void);
	void oneInput(wxBoxSizer *pSizer, int width, wxString label,wxString value);
	void twoInputs(wxBoxSizer *pSizer,wxString lb1,wxString val1,wxString lb2,wxString val2);
	static wxArrayString getLabels();
	static void SaveFile(wxString path,wxArrayString &mPersonalData);
};
