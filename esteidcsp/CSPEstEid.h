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
	static struct cardSetup {
		const tstring name;
		const tstring atr;
		const tstring atrMask;
	} cards[3];
	void regCard(cardSetup &card);
public:
	virtual CSPContext * createCSPContext();

	virtual HRESULT DllUnregisterServer(void);
	virtual HRESULT DllRegisterServer(void);

	CspEstEid(HMODULE module,TCHAR *);
	~CspEstEid(void);
};
