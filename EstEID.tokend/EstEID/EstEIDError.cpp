/*!
	\file		EstEIDError.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2008-11-14 03:36:37 +0200 (Fri, 14 Nov 2008) $
*/

/*
 *  EstEIDError.cpp
 */

#include "EstEIDError.h"

#include <Security/cssmerr.h>

//
// EstEIDError exceptions
//
EstEIDError::EstEIDError(uint16_t sw) : SCardError(sw)
{
	IFDEBUG(debugDiagnose(this));
}

EstEIDError::~EstEIDError() throw ()
{
}

const char *EstEIDError::what() const throw ()
{ return "EstEID error"; }

void EstEIDError::throwMe(uint16_t sw)
{ throw EstEIDError(sw); }

#if !defined(NDEBUG)

void EstEIDError::debugDiagnose(const void *id) const
{
/*    secdebug("tok_esteid", "%p EstEIDError %s (%04hX)",
             id, errorstr(statusWord), statusWord);*/
}

#endif //NDEBUG

