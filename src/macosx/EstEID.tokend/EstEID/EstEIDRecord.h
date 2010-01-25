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


