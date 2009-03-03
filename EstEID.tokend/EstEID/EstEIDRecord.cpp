/*!
	\file		EstEIDRecord.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2008-11-14 03:36:37 +0200 (Fri, 14 Nov 2008) $
*/

/*
 *  EstEIDRecord.cpp
 */
#include "EstEIDRecord.h"

#include "EstEIDError.h"
#include "EstEIDToken.h"
#include <CoreServices/../Frameworks/CarbonCore.framework/Headers/MacTypes.h>
#include <Security/SecKey.h>
#include <tokend/Attribute.h>
#include <tokend/MetaAttribute.h>
#include <tokend/MetaRecord.h>

//
// EstEIDRecord
//
EstEIDRecord::~EstEIDRecord()
{
}



EstEIDKeyRecord::EstEIDKeyRecord(
	const char *description, const Tokend::MetaRecord &metaRecord,
	bool signOnly) :
    EstEIDRecord(description)
{
    attributeAtIndex(metaRecord.metaAttribute(kSecKeyDecrypt).attributeIndex(),
                     new Tokend::Attribute(!signOnly));
    attributeAtIndex(metaRecord.metaAttribute(kSecKeyUnwrap).attributeIndex(),
                     new Tokend::Attribute(!signOnly));
    attributeAtIndex(metaRecord.metaAttribute(kSecKeySign).attributeIndex(),
                     new Tokend::Attribute(signOnly));
}

EstEIDKeyRecord::~EstEIDKeyRecord() {}

EstEIDCertRecord::~EstEIDCertRecord() {}

Tokend::Attribute *EstEIDCertRecord::getDataAttribute(Tokend::TokenContext *tokenContext) {
	EstEIDToken &token = dynamic_cast<EstEIDToken &>(*tokenContext);
	CssmData data;
	if (token.cachedObject(0, mDescription, data))
	{
		Tokend::Attribute *attribute =
			new Tokend::Attribute(data.Data, data.Length);
		free(data.Data);
		return attribute;
	}
	try {
		unsigned char arr[] = {0x30};
		data.Data = arr;
		data.Length = 1;
	} catch (std::exception &err) {
		secdebug("esteid.tokend","exception thrown in *EstEIDCertRecord::getDataAttribute '%s'",err.what());
		return NULL;
		}
	token.cacheObject(0, mDescription, data);
	return new Tokend::Attribute(data.Data, data.Length);
	}



