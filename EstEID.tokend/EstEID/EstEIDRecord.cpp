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

#include "EstEID_utility.h"
#include "utility/asnCertificate.h"

EstEIDRecord::EstEIDRecord(const char *description) :
		mDescription(description) {
		secdebug("tok_esteid","EstEIDRecord '%s' created",mDescription);
		}

const char *EstEIDRecord::description() { 
	secdebug("tok_esteid","EstEIDRecord::description '%s'",mDescription);
	return mDescription; 
	}

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
	FLOG;
	secdebug("tok_esteid","getDAtaAttribute, tokenContext = %p",tokenContext);
	EstEIDToken &token = dynamic_cast<EstEIDToken &>(*tokenContext);
//return NULL;
	CssmData data;
	if (token.cachedObject(0, mDescription, data))
	{
		Tokend::Attribute *attribute =
			new Tokend::Attribute(data.Data, data.Length);
		free(data.Data);
		return attribute;
	}
	try {
		std::vector<unsigned char> arrCert = token.getAuthCert();
		std::stringstream dummy;
//		std::ofstream decodeLog("decode.log");
		asnCertificate cert(arrCert,dummy);
		arrCert.resize(cert.size + (cert.body_start - cert.start));

		data.Data = &arrCert[0];
		data.Length = arrCert.size();
	} catch (std::exception &err) {
		secdebug("tok_esteid","exception thrown in *EstEIDCertRecord::getDataAttribute '%s'",err.what());
		return NULL;
		}
	token.cacheObject(0, mDescription, data);
	return new Tokend::Attribute(data.Data, data.Length);
	}



