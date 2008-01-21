/*!
	\file		PersonalDataDialog.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-26 12:27:14 +0200 (Mon, 26 Nov 2007) $
*/
// Revision $Revision: 149 $
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
