/*!
	\file		esteid.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/

/*
 * esteid.cpp - EstEID.tokend main program
 */

#include "EstEIDToken.h"

int main(int argc, const char *argv[])
{
	secdebug("tok_esteid", "main starting with %d arguments", argc);
	secdelay("/tmp/delay/EstEID");

	token = new EstEIDToken();
	return SecTokendMain(argc, argv, token->callbacks(), token->support());
}

