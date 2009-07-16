/*!
	\file		EstEIDError.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
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

