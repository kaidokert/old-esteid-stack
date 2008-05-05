/*!
	\file		PersonalDataDialog.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "PersonalDataDialog.h"
#include <fstream>

#ifndef WIN32 //this is ugly, but because XPM is static...
#include "resources/esteid.xpm"  //it cant be declared in header
#endif

#define ControlWidth 375

wxArrayString PersonalDataDialog::getLabels() {
	wxArrayString ret;
	ret.Add(_T(""));
	ret.Add(_("Last name"));
	ret.Add(_("Given names"));
	ret.Add(_("Given names"));
	ret.Add(_("Sex"));
	ret.Add(_("Citizenship"));
	ret.Add(_("Date of birth (dd.mm.yyyy)"));
	ret.Add(_("Personal Code"));
	ret.Add(_("Document number"));
	ret.Add(_("Date of expiry (dd.mm.yyyy)"));
	ret.Add(_("Birth-place"));
	ret.Add(_("Date of issue (dd.mm.yyyy)"));
	ret.Add(_("Type of residence permit"));
	ret.Add(_("Comment 1"));
	ret.Add(_("Comment 2"));
	ret.Add(_("Comment 3"));
	ret.Add(_("Comment 4"));
	return ret;
	};

void PersonalDataDialog::SaveFile(wxString path,wxArrayString &mPersonalData) {
	wxArrayString labels = getLabels();
	std::ofstream fs(path.ToAscii());
	for(size_t i = 1;i < mPersonalData.Count() ; i++) {

		fs << (const char *) labels[i].mb_str(wxConvUTF8)  << " - "
            << (const char *) mPersonalData[i].mb_str(wxConvUTF8) << std::endl;
		}
	}

void PersonalDataDialog::oneInput(wxBoxSizer *pSizer, int width, wxString label,wxString value) {
	wxStaticText * m_staticText = new wxStaticText( this, wxID_ANY, label);
	pSizer->Add( m_staticText,0,(wxUP | wxRIGHT | wxLEFT),2);
	wxTextCtrl * m_textCtrl = new wxTextCtrl( this, wxID_ANY,value);
	m_textCtrl->SetMinSize( wxSize( width,-1 ) );
	pSizer->Add(m_textCtrl,0, wxALL,2);
	}

void PersonalDataDialog::twoInputs(wxBoxSizer *pSizer,wxString lb1,wxString val1,wxString lb2,wxString val2) {
	wxBoxSizer* bHoriz1 = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* bVert1 = new wxBoxSizer( wxVERTICAL );
	oneInput(bVert1,ControlWidth / 2 ,lb1,val1);
	bHoriz1->Add(bVert1);
	wxBoxSizer* bVert2 = new wxBoxSizer( wxVERTICAL );
	oneInput(bVert2,ControlWidth / 2 ,lb2,val2);
	bHoriz1->Add(bVert2);
	pSizer->Add(bHoriz1);
	}

PersonalDataDialog::PersonalDataDialog(wxWindow *parent,wxString title,wxArrayString &mPersonalData) :
	wxDialog(parent,wxID_ANY,title,wxDefaultPosition,
		wxSize(400,490))
{
	SetIcon(wxICON(esteid));

	wxBoxSizer* surroundSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* vertSizer = new wxBoxSizer( wxVERTICAL );
	vertSizer->Add( 10, 10, 1, wxEXPAND, 0 );

	if (mPersonalData.GetCount() < 17)
		mPersonalData.SetCount(17);

	wxArrayString labels = getLabels();

	oneInput(vertSizer,ControlWidth,labels[1], mPersonalData[1]);
	oneInput(vertSizer,ControlWidth,labels[2], mPersonalData[2] + _T(" ") + mPersonalData[3]);

	twoInputs(vertSizer,	labels[4],mPersonalData[4],
						labels[5],mPersonalData[5]);
	twoInputs(vertSizer,labels[6],mPersonalData[6],
			labels[7],mPersonalData[7]);

	oneInput(vertSizer,ControlWidth,labels[10],mPersonalData[10]);
	twoInputs(vertSizer,	labels[8],mPersonalData[8],
						labels[12],mPersonalData[12]);
	twoInputs(vertSizer,	labels[9],mPersonalData[9],
						labels[11],mPersonalData[11]);
	oneInput(vertSizer,ControlWidth,labels[13],mPersonalData[13]);
	oneInput(vertSizer,ControlWidth,labels[14],mPersonalData[14]);
	oneInput(vertSizer,ControlWidth,labels[15],mPersonalData[15]);
	oneInput(vertSizer,ControlWidth,labels[16],mPersonalData[16]);

	surroundSizer->Add(vertSizer,0,wxALL,5);
	this->SetSizer( surroundSizer );
	this->Layout();
	this->Fit();
}

PersonalDataDialog::~PersonalDataDialog(void)
{
}
