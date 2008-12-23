/*!
	\file		esteidcsp.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#include "precompiled.h"
#include "esteidcsp.h"
#include "CSPEstEid.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

Csp *csp = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		csp = new CspEstEid(hModule,TEXT("EstEID NewCard CSP"));
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		delete csp;
		csp = NULL;
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
