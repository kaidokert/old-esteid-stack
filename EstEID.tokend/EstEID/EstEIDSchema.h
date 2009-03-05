/*!
	\file		EstEIDSchema.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/

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

