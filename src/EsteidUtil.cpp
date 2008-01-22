/*!
	\file		EsteidUtil.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "MainDialog.h"

class mApp : public wxApp
{
public:
    virtual bool OnInit();
};

DECLARE_APP(mApp)
IMPLEMENT_APP(mApp)

#undef PALOOKA 
#define PALOOKA

bool mApp::OnInit()
{
#if defined(_DEBUG) && defined(PALOOKA)
	FILE * m_pLogFile = fopen( "log.txt", "w" );
	delete wxLog::SetActiveTarget(new wxLogStderr(m_pLogFile));
	wxLog::EnableLogging();
	wxLog::SetLogLevel(wxLOG_Max);
	wxLog::SetVerbose();
	wxLog::AddTraceMask(_T("module"));
	
#endif

    if ( !wxApp::OnInit() )
        return false;

	wxFrame * frame = new MainDialog();
	frame->Show();
#if defined(_DEBUG) && defined(PALOOKA)
	delete wxLog::SetActiveTarget(NULL);
	fclose(m_pLogFile);
#endif
	return true;
}
