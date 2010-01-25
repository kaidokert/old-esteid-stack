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
 *  EstEIDSchema.h
 */

#ifndef _ESTEIDSCHEMA_H_
#define _ESTEIDSCHEMA_H_

#include <tokend/Schema.h>
#include "EstEIDKeyHandle.h"

namespace Tokend
{
	class Relation;
	class MetaRecord;
	class AttributeCoder;
}

class EstEIDSchema : public Tokend::Schema
{
	NOCOPY(EstEIDSchema)
public:
    EstEIDSchema();
    virtual ~EstEIDSchema();

	virtual void create();

protected:
	Tokend::Relation *createKeyRelation(CSSM_DB_RECORDTYPE keyType);

private:
	Tokend::ConstAttributeCoder mKeyAlgorithmCoder;
	Tokend::ConstAttributeCoder mKeySizeCoder;

	EstEIDKeyHandleFactory mEstEIDKeyHandleFactory;
};

#endif /* !_ESTEIDSCHEMA_H_ */

