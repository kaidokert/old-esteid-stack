/*!
	\file		netObj.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

enum connType {
	HTTP,
	HTTPS,
	FTP
};

#ifdef _WIN32 
#include "winInetObj.h"
typedef winInetObj netObj;
typedef inetConnect netConnect;
#else
#include "opensslObj.h"
typedef opensslObj netObj;
typedef opensslConnect netConnect;
#endif
