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

