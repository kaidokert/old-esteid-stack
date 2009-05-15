///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __id_updater_gen__
#define __id_updater_gen__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/gauge.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/combobox.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			wxID_productsPanel = 1000,
			wxID_prodDownloadStatus,
			wxID_prodCombo,
			wxID_prodProgressGauge,
		};
		
		wxPanel* m_headerPanel;
		wxButton* m_checkUpdatesButton;
		wxGauge* m_xmlGauge;
		wxStaticText* m_updateStatus;
		wxPanel* m_productsPanel;
		wxPanel* m_productPanel;
		wxStaticText* m_staticText2;
		wxStaticText* m_productDescription;
		wxStaticText* m_installedText;
		wxStaticText* m_availableText;
		wxStaticText* m_downloadText;
		wxComboBox* m_comboAction;
		wxGauge* m_progressGauge;
		wxPanel* m_productPanel1;
		wxStaticText* m_staticText21;
		wxStaticText* m_productDescription1;
		wxStaticText* m_installedText1;
		wxStaticText* m_availableText1;
		wxComboBox* m_comboAction1;
		wxGauge* m_progressGauge1;
		wxButton* m_downloadButton;
		wxButton* m_button3;
		wxButton* m_btnCheck;
		wxButton* m_button5;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnUpdateUI( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void OnCheckUpdates( wxCommandEvent& event ){ event.Skip(); }
		virtual void downloadClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnClickAdd( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 608,417 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrame();
	
};

#endif //__id_updater_gen__
