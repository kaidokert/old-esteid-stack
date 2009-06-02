/*!
	\file		AppSetting.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "AppSetting.h"
#include <wx/settings.h>

AppSetting::AppSetting(void)
{
	config = new wxConfig(_T("EsteidUtil"));
}

AppSetting::~AppSetting(void)
{
	delete config;
}
#ifdef __WXMSW__ //use CSP settings from registry
#define REGPATH \
	_T("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography\\") \
	_T("Defaults\\Provider\\EstEID Card CSP")
#define IESETTINGS \
	_T("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Ext\\Settings")
#define CERTRMVGUID \
	_T("{57C571FD-3CE1-4699-9AE3-22C129EE35AD}")

wxLanguage AppSetting::getLanguage() {
	long regVal = wxLANGUAGE_ESTONIAN;
	wxRegKey key(REGPATH);
	if (key.Exists() && key.Open(wxRegKey::Read)) {
		key.QueryValue(_T("Language"),&regVal);
		}
	return regVal == 0x09 ? wxLANGUAGE_ENGLISH :
		( regVal == 0x19 ? wxLANGUAGE_RUSSIAN : wxLANGUAGE_ESTONIAN);
	}

void AppSetting::setLanguage(wxLanguage lang) {
	wxRegKey key(REGPATH);
	if (!key.Exists()) return;
	{
	wxLogNull noLog;
	if (!key.Open(wxRegKey::Write)) {
		return;
		}
	}
	long regVal = lang == wxLANGUAGE_ENGLISH ? 0x09 :
		(lang == wxLANGUAGE_RUSSIAN ? 0x19 : 0x25);
	key.SetValue(_T("Language"),regVal);
	}

bool AppSetting::getUseCTAPI() {
	wxRegKey key(REGPATH);
	wxString lib;
	if (!key.Exists()) return false;

	if (key.Open(wxRegKey::Read)) {
		wxLogNull noLog;
		key.QueryValue(_T("CTLib"),lib);
		}
	return lib.Len() > 0;
	}

void AppSetting::setUseCTAPI(wxString lib,int portn) {
	wxRegKey key(REGPATH);
	if (lib.Len() > 0 ) { //set
		if (!key.Exists()) {
			if (!key.Create()) return; //couldnt create
			}
		}
	else
		if (!key.Exists()) return; //no need to do anything
	if (!key.Open(wxRegKey::Write)) return;

	key.SetValue(_T("CTPort"), portn);
	key.SetValue(_T("CTLib"), lib);
	}

wxString AppSetting::getInfoAppPath() {
	wxString retVal;

	wxRegKey key(_T("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Shared Tools\\MSINFO"));
	if (key.Exists() && key.Open(wxRegKey::Read)) {
			key.QueryValue(_T("Path"),retVal);
		}

	return retVal;
	}

bool AppSetting::getAutoRemove() {
	wxRegKey ieSet(IESETTINGS);
	if (!ieSet.Exists()) return true;
	if (!ieSet.Open(wxRegKey::Read)) 
		return true;

	wxRegKey key( IESETTINGS _T("\\") CERTRMVGUID );
	long val = 0;

	if (key.Exists())
		if (key.Open(wxRegKey::Read))
			key.QueryValue(_T("Flags"),&val);
	return !(val == 1);
	}

void AppSetting::setAutoremove(bool doSet) {
	wxRegKey ieSet(IESETTINGS);
	wxRegKey key(IESETTINGS _T("\\") CERTRMVGUID );

	if (!doSet) {
		if (!ieSet.Exists()) {
			if (!ieSet.Create(true)) return;
			}
		if (!key.Create(true)) return;
		key.SetValue(_T("Flags"),1);
		key.SetValue(_T("Version"),_T("*"));
		}
	else
		if (key.Exists()) key.DeleteSelf();
	}


#else //use wxConfig
wxLanguage AppSetting::getLanguage() {
	wxLanguage val = (wxLanguage )config->Read(_T("Language"),wxLANGUAGE_ESTONIAN);
	if (val != wxLANGUAGE_ENGLISH && val != wxLANGUAGE_RUSSIAN)
		val = wxLANGUAGE_ESTONIAN;
	return val;
	}

void AppSetting::setLanguage(wxLanguage lang) {
	config->Write(_T("Language"),lang);
	}

bool AppSetting::getUseCTAPI() {
	long value;
	config->Read(_T("UseCTAPI"),&value,0);
	return value != 0;
	}

void AppSetting::setUseCTAPI(wxString lib,int portn) {
	long value = (lib.Len() > 0);
	config->Write(_T("UseCTAPI"),value);
	}

wxString AppSetting::getInfoAppPath() {
    return _T("kinfocenter"); //should work something out here... GNOME sysinfo isnt popular
    }

bool AppSetting::getAutoRemove() {return false;}
void AppSetting::setAutoremove(bool ) {}

#endif
