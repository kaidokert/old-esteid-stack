#include "precompiled.h"
#include "InstallChecker.h"
#include <Msi.h>
#include <vector>
#include <iostream>
#include <wx/xml/xml.h>
#include <wx/sstream.h>
#include <wx/protocol/http.h>

#pragma comment(lib,"msi")

InstallChecker::InstallChecker(void)
{
}

InstallChecker::~InstallChecker(void)
{
}

void InstallChecker::getInstalledVersion(wxString upgradeCode,wxString &version) {
	TCHAR prodCode[40];
	version = wxT("none");
	const wxChar *code =upgradeCode.c_str();
	if (ERROR_SUCCESS != MsiEnumRelatedProducts(code,0,0,prodCode)) return;

	DWORD sz;
	TCHAR version_prop[100];
	sz = sizeof(version_prop) / sizeof(*version_prop);
	MsiGetProductInfo(prodCode,INSTALLPROPERTY_VERSIONSTRING, version_prop,&sz);
	version = version_prop;
	}


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
		MsiGetProductProperty(handle,_T("UpgradeCode"),uCode,&sz);
		MsiCloseHandle(handle);
		std::wcout << "product: " << name << std::endl;
		}
	}

