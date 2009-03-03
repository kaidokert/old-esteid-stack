/*!
	\file		EstEIDToken.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2008-11-14 03:36:37 +0200 (Fri, 14 Nov 2008) $
*/

/*
 *  EstEIDToken.h
 */

#ifndef _ESTEIDTOKEN_H_
#define _ESTEIDTOKEN_H_

#include <tokend/Token.h>

#include <security_utilities/pcsc++.h>

class EstEIDSchema;

class EstEIDTokenPriv;
//
// "The" token
//
class EstEIDToken : public Tokend::Token
{
	NOCOPY(EstEIDToken)
public:
	EstEIDToken();
	~EstEIDToken();

    virtual uint32 probe(SecTokendProbeFlags flags, char tokenUid[TOKEND_MAX_UID]);
	virtual void establish(const CSSM_GUID *guid, uint32 subserviceId,
		SecTokendEstablishFlags flags, const char *cacheDirectory, const char *workDirectory,
		char mdsDirectory[PATH_MAX], char printName[PATH_MAX]);
	virtual void authenticate(CSSM_DB_ACCESS_TYPE mode, const AccessCredentials *cred);
	virtual void getOwner(AclOwnerPrototype &owner);
	virtual void getAcl(const char *tag, uint32 &count, AclEntryInfo *&acls);

protected:

	void populate();

public:
	void *mConnection;
	EstEIDTokenPriv *d;
	
	// temporary ACL cache hack - to be removed
	AutoAclOwnerPrototype mAclOwner;
	AutoAclEntryInfoList mAclEntries;
};


#endif /* !_ESTEIDTOKEN_H_ */

