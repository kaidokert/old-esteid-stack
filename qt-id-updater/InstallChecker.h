
#include <string>

class InstallChecker
{
public:
	InstallChecker(void);
	~InstallChecker(void);
	void startChecking();
	void getInstalledVersion(std::wstring upgradeCode,std::wstring &version);
	static void installPackage(std::wstring filePath);
};
