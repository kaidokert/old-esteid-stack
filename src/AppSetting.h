/*!
	\file		AppSetting.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#pragma once
#include <wx/config.h>

class AppSetting
{
	wxConfig * config;
public:
	AppSetting(void);
	~AppSetting(void);
	wxLanguage getLanguage();
	void setLanguage(wxLanguage );
	wxString getInfoAppPath();
	bool getUseCTAPI();
	void setUseCTAPI(wxString lib,int portn);
	bool getAutoRemove();
	void setAutoremove(bool );
};
