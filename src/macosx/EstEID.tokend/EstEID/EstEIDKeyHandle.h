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
 *  EstEIDKeyHandle.h
 */

#ifndef _ESTEIDKEYHANDLE_H_
#define _ESTEIDKEYHANDLE_H_

#include "tokend/KeyHandle.h"

class EstEIDToken;
class EstEIDKeyRecord;


class EstEIDKeyHandle: public Tokend::KeyHandle
{
	NOCOPY(EstEIDKeyHandle)
public:
    EstEIDKeyHandle(EstEIDToken &token,
		const Tokend::MetaRecord &metaRecord,Tokend::Record &record);
    ~EstEIDKeyHandle();

    virtual void getKeySize(CSSM_KEY_SIZE &keySize);
    virtual uint32 getOutputSize(const Context &context, uint32 inputSize,
		bool encrypting);
    virtual void generateSignature(const Context &context,
		CSSM_ALGORITHMS signOnly, const CssmData &input, CssmData &signature);
    virtual void verifySignature(const Context &context,
		CSSM_ALGORITHMS signOnly, const CssmData &input,
		const CssmData &signature);
    virtual void generateMac(const Context &context, const CssmData &input,
		CssmData &output);
    virtual void verifyMac(const Context &context, const CssmData &input,
		const CssmData &compare);
    virtual void encrypt(const Context &context, const CssmData &clear,
		CssmData &cipher);
    virtual void decrypt(const Context &context, const CssmData &cipher,
		CssmData &clear);

	virtual void exportKey(const Context &context,
		const AccessCredentials *cred, CssmKey &wrappedKey);

private:
	EstEIDToken &mToken;
};


//
// A factory that creates EstEIDKeyHandle objects.
//
class EstEIDKeyHandleFactory : public Tokend::KeyHandleFactory
{
	NOCOPY(EstEIDKeyHandleFactory)
public:
	EstEIDKeyHandleFactory() {}
	virtual ~EstEIDKeyHandleFactory();

	virtual Tokend::KeyHandle *keyHandle(Tokend::TokenContext *tokenContext,
		const Tokend::MetaRecord &metaRecord, Tokend::Record &record) const;
};


#endif /* !_ESTEIDKEYHANDLE_H_ */


