/*!
	\file		CSPEestEid.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#pragma once
#include "Csp.h"



class CspEstEid : public Csp
{
public:
	virtual CSPContext * createCSPContext();

	CspEstEid(HMODULE module,TCHAR *);
	~CspEstEid(void);
};
