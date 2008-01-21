/*!
	\file		winInetObj.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-26 20:56:37 +0200 (Mon, 26 Nov 2007) $
*/

enum connType {
	HTTP,
	HTTPS
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
