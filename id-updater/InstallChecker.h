#pragma once

class InstallChecker
{
public:
	InstallChecker(void);
	~InstallChecker(void);
	void startChecking();
	void getInstalledVersion(wxString upgradeCode,wxString &version);
};
