#include "precompiled.h"
#include "id_updaterMainFrame.h"
#include <wx/cmdline.h>

class mApp : public wxApp
{
	bool verbose;
	wxString updateUrl;
public:
	mApp();
    virtual bool OnInit();
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
};

DECLARE_APP(mApp)
IMPLEMENT_APP(mApp)

mApp::mApp() : updateUrl("http://kaidokert.com/work/updater/") {}

bool mApp::OnCmdLineParsed(wxCmdLineParser& parser) {
	verbose = parser.Found(_T("verbose"));
	wxString setUrl;
	if (parser.Found("url",&setUrl))
		updateUrl = setUrl;
	return wxApp::OnCmdLineParsed(parser);
	}

void mApp::OnInitCmdLine(wxCmdLineParser& parser) {
	const wxCmdLineEntryDesc cmdLineDesc[] =
	{
		{ wxCMD_LINE_OPTION, wxT("v"), wxT("verbose"),  _("show verbose log") },
		{ wxCMD_LINE_OPTION, wxT("u"), wxT("url"),  _("url for updates") },
		{ wxCMD_LINE_NONE }
	};

	parser.SetDesc(cmdLineDesc);
	wxApp::OnInitCmdLine(parser);
	}

bool mApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

	wxFrame * frame = new id_updaterMainFrame(NULL,updateUrl);
	frame->Show();
	return true;
}

void wxLogAdvMsg(const wxChar *szFormat, ... ) {}
void wxLogUsrMsg(const wxChar *szFormat, ... ) {}
