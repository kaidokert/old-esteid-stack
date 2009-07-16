/*!
	\file		EstEIDToken.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/

/*
 *  EstEIDToken.h
 */

#ifndef _ESTEIDTOKEN_H_
#define _ESTEIDTOKEN_H_

#include <tokend/Token.h>

#include <security_utilities/pcsc++.h>
#include <cardlib/common.h>
#include <cardlib/EstEidCard.h>

class EstEIDSchema;

class EstEIDTokenPriv;
//
// "The" token
//
class EstEIDToken : public Tokend::ISO7816Token
{
	friend class EstEIDRecord;
	friend class EstEIDCertRecord;
	friend class EstEIDKeyHandle;
	NOCOPY(EstEIDToken)
public:
	EstEIDToken();
	~EstEIDToken();

    virtual uint32 probe(SecTokendProbeFlags flags, char tokenUid[TOKEND_MAX_UID]);
	virtual void establish(const CSSM_GUID *guid, uint32 subserviceId,
		SecTokendEstablishFlags flags, const char *cacheDirectory, const char *workDirectory,
		char mdsDirectory[PATH_MAX], char printName[PATH_MAX]);
	virtual void getOwner(AclOwnerPrototype &owner);
	virtual void getAcl(const char *tag, uint32 &count, AclEntryInfo *&acls);

	virtual uint32_t pinStatus(int pinNum);
	virtual void verifyPIN(int pinNum,
		const unsigned char *pin, size_t pinLength);
	virtual void unverifyPIN(int pinNum);

protected:

	void populate();
	EstEidCard &getCard();
	
private:
	void checkPrivate();
public:
	void *mConnection;
	EstEIDTokenPriv *d;

	uint32_t mPinStatus;
	
	// temporary ACL cache hack - to be removed
	AutoAclOwnerPrototype mAclOwner;
	AutoAclEntryInfoList mAclEntries;
};


#endif /* !_ESTEIDTOKEN_H_ */
