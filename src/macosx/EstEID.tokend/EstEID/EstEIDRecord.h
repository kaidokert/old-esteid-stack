/*!
	\file		EstEIDRecord.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/

/*
 *  EstEIDRecord.h
 */

#ifndef _ESTEIDRECORD_H_
#define _ESTEIDRECORD_H_

#include <tokend/Record.h>

class EstEIDRecord : public Tokend::Record
{
	NOCOPY(EstEIDRecord)
public:
	EstEIDRecord(const char *description);
	~EstEIDRecord();

	virtual const char *description();
	
protected:
	const char *mDescription;
};

class EstEIDKeyRecord : public EstEIDRecord
{
	NOCOPY(EstEIDKeyRecord)
public:
    EstEIDKeyRecord(const char *description,
		const Tokend::MetaRecord &metaRecord, bool signOnly);
	virtual ~EstEIDKeyRecord();
	size_t sizeInBits() const { return 1024; }
    virtual void getAcl(const char *tag, uint32 &count,
		AclEntryInfo *&aclList);
private:
	bool mSignOnly;
	AutoAclEntryInfoList mAclEntries;

};

class EstEIDCertRecord : public EstEIDRecord 
{
	NOCOPY(EstEIDCertRecord)
public:
    EstEIDCertRecord(const char *description) : EstEIDRecord(description)  {}
	virtual ~EstEIDCertRecord();
	
	virtual Tokend::Attribute *getDataAttribute(Tokend::TokenContext *tokenContext);
};

#endif /* !_ESTEIDRECORD_H_ */


