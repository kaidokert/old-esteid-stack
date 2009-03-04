/*!
	\file		EstEIDSchema.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2008-11-14 03:36:37 +0200 (Fri, 14 Nov 2008) $
*/


/*
 *  EstEIDSchema.cpp
 */
#include <CoreServices/../Frameworks/CarbonCore.framework/Headers/MacTypes.h>
#include "EstEIDSchema.h"

#include <tokend/MetaAttribute.h>
#include <tokend/MetaRecord.h>

#include <Security/SecCertificate.h>
#include <Security/SecKeychainItem.h>
#include <Security/SecKey.h>

#include "EstEID_utility.h"

using namespace Tokend;

EstEIDSchema::EstEIDSchema() :
	mKeyAlgorithmCoder(uint32(CSSM_ALGID_RSA)),
	mKeySizeCoder(uint32(1024))
{
}

EstEIDSchema::~EstEIDSchema()
{
}

Tokend::Relation *EstEIDSchema::createKeyRelation(CSSM_DB_RECORDTYPE keyType)
{
	FLOG;
	
	Relation *rn = createStandardRelation(keyType);

	// Set up coders for key records.
	MetaRecord &mr = rn->metaRecord();
	mr.keyHandleFactory(&mEstEIDKeyHandleFactory);

	// Print name of a key might as well be the key name.
	mr.attributeCoder(kSecKeyPrintName, &mDescriptionCoder);

	// Other key valuess
	mr.attributeCoder(kSecKeyKeyType, &mKeyAlgorithmCoder);
	mr.attributeCoder(kSecKeyKeySizeInBits, &mKeySizeCoder);
	mr.attributeCoder(kSecKeyEffectiveKeySize, &mKeySizeCoder);

	// Key attributes
	mr.attributeCoder(kSecKeyExtractable, &mFalseCoder);
	mr.attributeCoder(kSecKeySensitive, &mTrueCoder);
	mr.attributeCoder(kSecKeyModifiable, &mFalseCoder);
	mr.attributeCoder(kSecKeyPrivate, &mTrueCoder);
	mr.attributeCoder(kSecKeyNeverExtractable, &mTrueCoder);
	mr.attributeCoder(kSecKeyAlwaysSensitive, &mTrueCoder);

	// Key usage
	mr.attributeCoder(kSecKeyEncrypt, &mFalseCoder);
	mr.attributeCoder(kSecKeyWrap, &mFalseCoder);
	mr.attributeCoder(kSecKeyVerify, &mFalseCoder);
	mr.attributeCoder(kSecKeyDerive, &mFalseCoder);
	mr.attributeCoder(kSecKeySignRecover, &mFalseCoder);
	mr.attributeCoder(kSecKeyVerifyRecover, &mFalseCoder);

	return rn;
}

void EstEIDSchema::create()
{
	Schema::create();

	createStandardRelation(CSSM_DL_DB_RECORD_X509_CERTIFICATE);
	createKeyRelation(CSSM_DL_DB_RECORD_PRIVATE_KEY);
    createStandardRelation(CSSM_DL_DB_RECORD_GENERIC);
}

