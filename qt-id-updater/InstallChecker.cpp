#include "InstallChecker.h"

#if defined(WIN32)

#include <Windows.h>
#include <Msi.h>
#include <tchar.h>
#include <vector>
#include <iostream>

#pragma comment(lib,"msi")

InstallChecker::InstallChecker(void)
{
}

InstallChecker::~InstallChecker(void)
{
}

void InstallChecker::getInstalledVersion(std::wstring upgradeCode,std::wstring &version) {
	TCHAR prodCode[40];
	version = L"none";
	const wchar_t *code =upgradeCode.c_str();
	if (ERROR_SUCCESS != MsiEnumRelatedProducts(code,0,0,prodCode)) return;

	DWORD sz;
	TCHAR version_prop[100];
	sz = sizeof(version_prop) / sizeof(*version_prop);
	MsiGetProductInfo(prodCode,INSTALLPROPERTY_VERSIONSTRING, version_prop,&sz);
	version = version_prop;
	}

/*
void InstallChecker::startChecking() {
	UINT code = ERROR_SUCCESS;
	TCHAR prodCode[40];
	TCHAR name[1024],manufacturer[1024],version[100];
	for(DWORD idx = 0; code == ERROR_SUCCESS;idx++) {
		code =MsiEnumProducts(idx,prodCode);
		if (ERROR_SUCCESS != code) break;
		DWORD sz = sizeof(name) / sizeof(*name);
		MsiGetProductInfo(prodCode,INSTALLPROPERTY_INSTALLEDPRODUCTNAME, name,&sz);
		sz = sizeof(manufacturer) / sizeof(*manufacturer);
		MsiGetProductInfo(prodCode,INSTALLPROPERTY_PUBLISHER, manufacturer,&sz);
		sz = sizeof(version) / sizeof(*version);
		MsiGetProductInfo(prodCode,INSTALLPROPERTY_VERSIONSTRING, version,&sz);
		MSIHANDLE handle;
		UINT ret = MsiOpenProduct(prodCode,&handle);
		TCHAR uCode[40];
		sz = sizeof(uCode) / sizeof(*uCode);
		MsiGetProductProperty(handle,L"UpgradeCode",uCode,&sz);
		MsiCloseHandle(handle);
		std::wcout << "product: " << name << std::endl;
		}
	}
*/
struct msiPack {
	MSIHANDLE hProduct;
	WCHAR prodCode[1024]; 
	std::wstring m_msiFile;
	msiPack(std::wstring file) : hProduct(NULL),m_msiFile(file) {
		MsiOpenPackage(m_msiFile.c_str(),&hProduct);
		DWORD sz = sizeof(prodCode);
		MsiGetProductProperty(hProduct,L"ProductCode",prodCode,&sz);
		MsiCloseHandle(hProduct);
		hProduct = NULL;
		}
	~msiPack() {
		if (hProduct) 
			MsiCloseHandle(hProduct);
		}
	bool install() {
		MsiSetInternalUI(INSTALLUILEVEL_FULL,0);
		UINT retCode = MsiReinstallProduct(prodCode, 
				REINSTALLMODE_FILEREPLACE |
				REINSTALLMODE_MACHINEDATA|
				REINSTALLMODE_USERDATA |
				REINSTALLMODE_SHORTCUT |
				REINSTALLMODE_PACKAGE); 
		if (retCode == ERROR_UNKNOWN_PRODUCT) {
			const wchar_t *pack = m_msiFile.c_str();
			retCode = MsiInstallProduct(pack,L"ACTION=INSTALL");
			}
		return retCode == ERROR_SUCCESS;
		}
	};

bool InstallChecker::installPackage(std::wstring filePath) {
	msiPack pack(filePath);
	return pack.install();
	}

bool InstallChecker::verifyPackage(std::wstring filePath) {
	return true;
	}
#else
void InstallChecker::getInstalledVersion(std::wstring upgradeCode,std::wstring &version) {
	version = L"1.20";
	}

bool InstallChecker::installPackage(std::wstring filePath) {
	return true;
	}

bool InstallChecker::verifyPackage(std::wstring filePath) {
	return true;
	}
#endif