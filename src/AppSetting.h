/*!
	\file		AppSetting.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-26 12:27:14 +0200 (Mon, 26 Nov 2007) $
*/
// Revision $Revision: 149 $
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
