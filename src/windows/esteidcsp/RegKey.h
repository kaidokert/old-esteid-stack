/*!
	\file		RegKey.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#pragma once
#include <string>
#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

class RegKey
{
	HKEY key;
public:
	RegKey(HKEY parent,tstring name,REGSAM access = KEY_READ);
	RegKey(HKEY parent,std::string name,REGSAM access = KEY_READ);
	~RegKey(void);
	std::string readString(std::string name);
	tstring RegKey::readString(tstring name);

	void setString(tstring valueName,tstring value);
	void setInt(tstring valueName,DWORD value);
	void setBin(tstring valueName,std::vector<BYTE> &bin);

	void deleteKey(tstring keyName);

	operator HKEY() const {return key;}
};
