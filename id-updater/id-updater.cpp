#include "precompiled.h"
#include "id_updaterMainFrame.h"

class mApp : public wxApp
{
	bool verbose;
public:
    virtual bool OnInit();
};

DECLARE_APP(mApp)
IMPLEMENT_APP(mApp)

bool mApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

	wxFrame * frame = new id_updaterMainFrame(NULL);
	frame->Show();
	return true;
}

void wxLogAdvMsg(const wxChar *szFormat, ... ) {}
void wxLogUsrMsg(const wxChar *szFormat, ... ) {}
