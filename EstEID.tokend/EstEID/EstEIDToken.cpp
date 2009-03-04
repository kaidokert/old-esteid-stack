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
#include "EstEID_utility.h"

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
			secdebug("tok_esteid", "reader %s", cardMgr.getReaderName(i).c_str() );
			if ( cardMgr.getReaderName(i) != readerName ) continue;
			
			secdebug("tok_esteid","we have our reader");
			eCard = new EstEidCard(cardMgr);
			if (eCard->isInReader(i)) {
				eCard->connect(i);
				secdebug("tok_esteid","card id: %s", eCard->readCardID().c_str());
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
		}
};

EstEIDToken::EstEIDToken() : 
	d(NULL) ,
	mPinStatus(0)
{
	mTokenContext = this;
	try {
		d = new EstEIDTokenPriv;
	} catch(std::exception &) {} 
}

EstEIDToken::~EstEIDToken()
{
	if (d) 
		delete d;
}

void EstEIDToken::checkPrivate() {
	if (!d) { 
		secdebug("tok_esteid", "PCSSMgr uninitialized");
		CssmError::throwMe(CSSM_ERRCODE_SELF_CHECK_FAILED);
		}
	}
	
uint32 EstEIDToken::probe(SecTokendProbeFlags flags,
	char tokenUid[TOKEND_MAX_UID])
{
	FLOG;
	checkPrivate();
	uint32 retCode = 0;
	const SCARD_READERSTATE &readerState = *(*startupReaderInfo)();
	secdebug("tok_esteid", "given reader was '%s'",readerState.szReader);
	try {
		if (d->connectCard(readerState.szReader)) {
			secdebug("tok_esteid","card id: %s", d->card().readCardID().c_str());
			retCode = 10;
			d->disconnect();
			}
	} catch(std::runtime_error &err) {
		secdebug("tok_esteid","exception: %s" , err.what() );
		}
	return retCode;
}

void EstEIDToken::establish(const CSSM_GUID *guid, uint32 subserviceId,
	SecTokendEstablishFlags flags, const char *cacheDirectory,
	const char *workDirectory, char mdsDirectory[PATH_MAX],
	char printName[PATH_MAX])
{
	FLOG;
	checkPrivate();

	const SCARD_READERSTATE &readerState = *(*startupReaderInfo)();
	

	try {
		if (d->connectCard(readerState.szReader)) {
			std::string cardId = d->card().readCardID();
			::strncpy(printName, cardId.c_str(), PATH_MAX);
			}

	} catch(std::runtime_error &err) {
		secdebug("tok_esteid","exception: %s" , err.what() );
		}

	secdebug("tok_esteid", "name: %s", printName);

	mSchema = new EstEIDSchema();
	mSchema->create();

	populate();
}

std::vector<byte> EstEIDToken::getAuthCert()  {
	FLOG;
	checkPrivate();
	return d->card().getAuthCert();
	}

uint32_t EstEIDToken::pinStatus(int pinNum)
{
	FLOG;

	if (pinNum != 1)
		CssmError::throwMe(CSSM_ERRCODE_SAMPLE_VALUE_NOT_SUPPORTED);
	return mPinStatus;
}

void EstEIDToken::verifyPIN(int pinNum,
	const unsigned char *pin, size_t pinLength)
{
	FLOG;
	checkPrivate();

	secdebug("tok_esteid", "EstEIDToken::verifyPIN: pin num %d pin len %d", pinNum,pinLength);
	std::vector<unsigned char > pinV(pin,pin + pinLength);
	std::string pinStr(pinV.begin(),pinV.end());
	byte retries=0;
	mPinStatus = SCARD_AUTHENTICATION_FAILED;
	switch (pinNum) {
		case 1:
			try {
				secdebug("tok_esteid","pin was '%s'",pinStr.c_str());
				d->card().validateAuthPin(pinStr, retries);
				mPinStatus = SCARD_SUCCESS;
			} catch(std::exception &e) {
				secdebug("tok_esteid","authentication failed, %d retries left");
				}
			break;
		default:
			CssmError::throwMe(CSSM_ERRCODE_SAMPLE_VALUE_NOT_SUPPORTED);
		}
}

void EstEIDToken::unverifyPIN(int pinNum)
{
	FLOG;
	switch(pinNum) {
		case 1:
			mPinStatus = SCARD_AUTHENTICATION_FAILED;
			break;
		default:
			CssmError::throwMe(CSSM_ERRCODE_SAMPLE_VALUE_NOT_SUPPORTED);
		}
}


void EstEIDToken::getOwner(AclOwnerPrototype &owner)
{
	FLOG;

	// we don't really know (right now), so claim we're owned by PIN #0
	if (!mAclOwner) {
		mAclOwner.allocator(Allocator::standard());
		mAclOwner = AclFactory::PinSubject(Allocator::standard(), 0);
	}
	owner = mAclOwner;
}


void EstEIDToken::getAcl(const char *tag, uint32 &count, AclEntryInfo *&acls)
{
	FLOG;

	Allocator &alloc = Allocator::standard();

	// we don't (yet) support queries by tag
	if (unsigned pin = pinFromAclTag(tag, "?")) {
		static AutoAclEntryInfoList acl;
		acl.clear();
		acl.allocator(alloc);
		uint32_t status = this->pinStatus(pin);
		if (status == SCARD_SUCCESS) {
			secdebug("tok_esteid","pin %d status AUTHORIZED",pin);
			acl.addPinState(pin, CSSM_ACL_PREAUTH_TRACKING_AUTHORIZED);
			}
		else {
			secdebug("tok_esteid","pin %d status AUTHORIZED",pin);
			acl.addPinState(pin, CSSM_ACL_PREAUTH_TRACKING_UNKNOWN);
			}
		count = acl.size();
		acls = acl.entries();
		return;
	}

	// get pin list, then for each pin
	if (!mAclEntries) {
		mAclEntries.allocator(alloc);
        // Anyone can read any record from this db.
        // We don't support insertion modification or deletion yet.
        mAclEntries.add(CssmClient::AclFactory::AnySubject(mAclEntries.allocator()),
                        AclAuthorizationSet(CSSM_ACL_AUTHORIZATION_DB_READ, 0));
        // We support PIN1 with either a passed in password
        // subject or a prompted password subject.
		mAclEntries.addPin(AclFactory::PWSubject(alloc), 1);
		mAclEntries.addPin(AclFactory::PromptPWSubject(alloc, CssmData()), 1);
	}

	// return the ACL vector
	count = mAclEntries.size();
	acls = mAclEntries.entries();
}


void EstEIDToken::populate()
{
	FLOG;

	Tokend::Relation &certRelation =
		mSchema->findRelation(CSSM_DL_DB_RECORD_X509_CERTIFICATE);
	Tokend::Relation &privateKeyRelation =
		mSchema->findRelation(CSSM_DL_DB_RECORD_PRIVATE_KEY);
	Tokend::Relation &dataRelation =
		mSchema->findRelation(CSSM_DL_DB_RECORD_GENERIC);
		
	RefPointer<Tokend::Record> eAuthCert(new EstEIDCertRecord( "Authentication Certificate"));
//	RefPointer<Tokend::Record> eSignCert(new EstEIDCertRecord( "Signing Certificate"));

	certRelation.insertRecord(eAuthCert);
//	certRelation.insertRecord(eSignCert);
	RefPointer<Tokend::Record> eAuthKey(new EstEIDKeyRecord(
		"Authentication Key",
		privateKeyRelation.metaRecord(), true));
/*	RefPointer<Tokend::Record> eSignKey(new EstEIDKeyRecord(
		 "Signature Key",
		privateKeyRelation.metaRecord(), false));*/

	privateKeyRelation.insertRecord(eAuthKey);
//	privateKeyRelation.insertRecord(eSignKey);

	eAuthKey->setAdornment(mSchema->publicKeyHashCoder().certificateKey(),
                          new Tokend::LinkedRecordAdornment(eAuthCert));
/*	eSignKey->setAdornment(mSchema->publicKeyHashCoder().certificateKey(),
                          new Tokend::LinkedRecordAdornment(eSignCert)); */
}

