
#include <string>

class InstallChecker
{
public:
	InstallChecker(void);
	~InstallChecker(void);
	static void getInstalledVersion(std::wstring upgradeCode,std::wstring &version);
	static bool verifyPackage(std::wstring filePath);
	static bool installPackage(std::wstring filePath);
};
