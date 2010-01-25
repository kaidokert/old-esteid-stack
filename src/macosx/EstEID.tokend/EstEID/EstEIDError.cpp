/*
* EstEID.tokend
*
* Copyright (C) 2009 Kaido Kert <kaidokert@gmail.com>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*
*/
// $Revision$
// $Date$
// $Author$

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

