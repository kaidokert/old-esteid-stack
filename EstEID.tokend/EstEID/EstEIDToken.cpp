/*!
	\file		EstEIDToken.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2008-11-14 03:36:37 +0200 (Fri, 14 Nov 2008) $
*/

/*
 *  EstEIDToken.cpp
 */
#include "EstEIDToken.h"

#include "EstEIDError.h"
#include "EstEIDRecord.h"
#include "EstEIDSchema.h"
#include <map>
#include <vector>
#include "cardlib/common.h"
#include "cardlib/PCSCManager.h"
#include "cardlib/EstEidCard.h"

#include <security_cdsa_client/aclclient.h>

#include <CoreServices/../Frameworks/CarbonCore.framework/Headers/MacTypes.h>
#include <tokend/Adornment.h>

using CssmClient::AclFactory;

class EstEIDTokenPriv {
	PCSCManager cardMgr;
	EstEidCard *eCard;
public:
	EstEIDTokenPriv() : eCard(NULL) {}
	EstEidCard &card() {
		if (!eCard) throw std::runtime_error("not connected");
		return *eCard;
		}
	
	bool connectCard(const char *readerName) {
		disconnect();
		for(uint i = 0;i < cardMgr.getReaderCount();i++) {
			secdebug("esteid.tokend", "reader %s", cardMgr.getReaderName(i).c_str() );
			if ( cardMgr.getReaderName(i) != readerName ) continue;
			
			secdebug("esteid.tokend","we have our reader");
			eCard = new EstEidCard(cardMgr);
			if (eCard->isInReader(i)) {
				eCard->connect(i);
				secdebug("esteid.tokend","card id: %s", eCard->readCardID().c_str());
				return true;
				}
			}
		return false;
		}
	void disconnect() {
		if (eCard) {
			delete eCard;
			eCard = NULL;
		}
};

EstEIDToken::EstEIDToken() : d(NULL) 
{
	try {
		d = new EstEIDTokenPriv;
	} catch(std::exception &) {} 
}

EstEIDToken::~EstEIDToken()
{
	if (d) 
		delete d;
}

uint32 EstEIDToken::probe(SecTokendProbeFlags flags,
	char tokenUid[TOKEND_MAX_UID])
{
	uint32 retCode = 0;
	secdebug("esteid.tokend", "EstEIDToken::probe() begin");
	const SCARD_READERSTATE &readerState = *(*startupReaderInfo)();
	if (!d) { 
		secdebug("esteid.tokend", "PCSSMgr uninitialized");
		return 0;
		}
	secdebug("esteid.tokend", "given reader was '%s'",readerState.szReader);
	try {
		if (d->connectCard(readerState.szReader)) {
			secdebug("esteid.tokend","card id: %s", d->card().readCardID().c_str());
			retCode = 10;
			d->disconnect();
			}
	} catch(std::runtime_error &err) {
		secdebug("esteid.tokend","exception: %s" , err.what() );
		}
	secdebug("esteid.tokend", "EstEIDToken::probe() end");
	return retCode;
}

void EstEIDToken::establish(const CSSM_GUID *guid, uint32 subserviceId,
	SecTokendEstablishFlags flags, const char *cacheDirectory,
	const char *workDirectory, char mdsDirectory[PATH_MAX],
	char printName[PATH_MAX])
{
	secdebug("esteid.tokend", "EstEIDToken::establish() begin");
	const SCARD_READERSTATE &readerState = *(*startupReaderInfo)();
	
	if (!d) { 
		secdebug("esteid.tokend", "PCSSMgr uninitialized");
		return ;
		}

	try {
		if (d->connectCard(readerState.szReader)) {
			std::string cardId = d->card().readCardID();
			secdebug("esteid.tokend","card id: %s", cardId.c_str());
			::strncpy(printName, cardId.c_str(), PATH_MAX);
			}

	} catch(std::runtime_error &err) {
		secdebug("esteid.tokend","exception: %s" , err.what() );
		}

	secdebug("esteid.tokend", "name: %s", printName);

	mSchema = new EstEIDSchema();
	mSchema->create();

	populate();
}


void EstEIDToken::authenticate(CSSM_DB_ACCESS_TYPE mode, const AccessCredentials *cred)
{
	CssmError::throwMe(CSSM_ERRCODE_ACL_SUBJECT_TYPE_NOT_SUPPORTED);
}


void EstEIDToken::getOwner(AclOwnerPrototype &owner)
{
	// we don't really know (right now), so claim we're owned by PIN #0
	if (!mAclOwner) {
		mAclOwner.allocator(Allocator::standard());
		mAclOwner = AclFactory::PinSubject(Allocator::standard(), 0);
	}
	owner = mAclOwner;
}


void EstEIDToken::getAcl(const char *tag, uint32 &count, AclEntryInfo *&acls)
{
	// we don't (yet) support queries by tag
	if (tag)
		CssmError::throwMe(CSSM_ERRCODE_INVALID_ACL_ENTRY_TAG);

	Allocator &alloc = Allocator::standard();
	// get pin list, then for each pin
	if (!mAclEntries) {
		mAclEntries.allocator(alloc);
        // Anyone can read any record from this db.
        // We don't support insertion modification or deletion yet.
        mAclEntries.add(CssmClient::AclFactory::AnySubject(mAclEntries.allocator()),
                        AclAuthorizationSet(CSSM_ACL_AUTHORIZATION_DB_READ, 0));
	}

	// return the ACL vector
	count = mAclEntries.size();
	acls = mAclEntries.entries();
}


void EstEIDToken::populate()
{
	secdebug("esteid.tokend", "EstEIDToken::populate() begin");
	Tokend::Relation &certRelation =
		mSchema->findRelation(CSSM_DL_DB_RECORD_X509_CERTIFICATE);
	Tokend::Relation &privateKeyRelation =
		mSchema->findRelation(CSSM_DL_DB_RECORD_PRIVATE_KEY);
	Tokend::Relation &dataRelation =
		mSchema->findRelation(CSSM_DL_DB_RECORD_GENERIC);
		
	RefPointer<Tokend::Record> eAuthCert(new EstEIDCertRecord( "Authentication Certificate"));
	RefPointer<Tokend::Record> eSignCert(new EstEIDCertRecord( "Signing Certificate"));

	certRelation.insertRecord(eAuthCert);
	certRelation.insertRecord(eSignCert);

	RefPointer<Tokend::Record> eAuthKey(new EstEIDKeyRecord(
		"Authentication Key",
		privateKeyRelation.metaRecord(), true));
	RefPointer<Tokend::Record> eSignKey(new EstEIDKeyRecord(
		 "Signature Key",
		privateKeyRelation.metaRecord(), false));

	privateKeyRelation.insertRecord(eAuthKey);
	privateKeyRelation.insertRecord(eSignKey);

	eAuthKey->setAdornment(mSchema->publicKeyHashCoder().certificateKey(),
                          new Tokend::LinkedRecordAdornment(eAuthCert));
	eSignKey->setAdornment(mSchema->publicKeyHashCoder().certificateKey(),
                          new Tokend::LinkedRecordAdornment(eSignCert));


	secdebug("esteid.tokend", "EstEIDToken::populate() end");
}

