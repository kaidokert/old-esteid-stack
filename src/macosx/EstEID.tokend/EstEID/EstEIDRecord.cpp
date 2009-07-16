/*!
	\file		EstEIDRecord.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
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

#include <security_cdsa_client/aclclient.h>
#include "EstEID_utility.h"

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
    EstEIDRecord(description),
	mSignOnly(signOnly)
{
	FLOG;
    attributeAtIndex(metaRecord.metaAttribute(kSecKeyDecrypt).attributeIndex(),
                     new Tokend::Attribute(!signOnly));
    attributeAtIndex(metaRecord.metaAttribute(kSecKeyUnwrap).attributeIndex(),
                     new Tokend::Attribute(!signOnly));
    attributeAtIndex(metaRecord.metaAttribute(kSecKeySign).attributeIndex(),
                     new Tokend::Attribute(signOnly));
}

EstEIDKeyRecord::~EstEIDKeyRecord() {}

void EstEIDKeyRecord::getAcl(const char *tag, uint32 &count,
	AclEntryInfo *&acls)
{
	FLOG;
	
	// @@@ Key 1 has any acl for sign, key 2 has pin1 acl, and key3 has pin1
	// acl with auto-lock which we express as a prompted password subject.
	if (!mAclEntries) {
		mAclEntries.allocator(Allocator::standard());
        // Anyone can read the DB record for this key (which is a reference
		// CSSM_KEY)
        mAclEntries.add(CssmClient::AclFactory::AnySubject(
			mAclEntries.allocator()),
			AclAuthorizationSet(CSSM_ACL_AUTHORIZATION_DB_READ, 0));

		// Using this key to sign or decrypt will require PIN1
		mAclEntries.add(CssmClient::AclFactory::PinSubject(
			mAclEntries.allocator(), 1),
			AclAuthorizationSet((mSignOnly
				? CSSM_ACL_AUTHORIZATION_SIGN
				: CSSM_ACL_AUTHORIZATION_DECRYPT), 0));
	}
	count = mAclEntries.size();
	acls = mAclEntries.entries();
}


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
		std::vector<unsigned char> arrCert = token.getCard().getAuthCert();
		data.Data = &arrCert[0];
		data.Length = arrCert.size();
	} catch (std::exception &err) {
		secdebug("tok_esteid","exception thrown in *EstEIDCertRecord::getDataAttribute '%s'",err.what());
		return NULL;
		}
	token.cacheObject(0, mDescription, data);
	return new Tokend::Attribute(data.Data, data.Length);
	}



