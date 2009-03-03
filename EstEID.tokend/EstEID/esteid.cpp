/*!
	\file		esteid.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2008-11-14 03:36:37 +0200 (Fri, 14 Nov 2008) $
*/

/*
 * esteid.cpp - EstEID.tokend main program
 */

#include "EstEIDToken.h"

int main(int argc, const char *argv[])
{
	secdebug("esteid.tokend", "main starting with %d arguments", argc);
	secdelay("/tmp/delay/EstEID");

	token = new EstEIDToken();
	return SecTokendMain(argc, argv, token->callbacks(), token->support());
}

