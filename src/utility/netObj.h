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

#ifdef WIN32 
#include "utility/winInetObj.h"
typedef winInetObj netObj;
typedef inetConnect netConnect;
#else
#include "utility/opensslObj.h"
typedef opensslObj netObj;
typedef opensslConnect netConnect;
#endif
