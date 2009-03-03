/*!
	\file		EstEIDKeyHandle.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2008-11-14 03:36:37 +0200 (Fri, 14 Nov 2008) $
*/

/*
 *  EstEIDKeyHandle.cpp
 */

#include "EstEIDKeyHandle.h"

#include "EstEIDRecord.h"
#include "EstEIDToken.h"

#include <security_utilities/debugging.h>
#include <security_utilities/utilities.h>
#include <security_cdsa_utilities/cssmerrors.h>
#include <Security/cssmerr.h>


//
// EstEIDKeyHandle
//
EstEIDKeyHandle::EstEIDKeyHandle(const Tokend::MetaRecord &metaRecord,
	Tokend::Record &record) :
	Tokend::KeyHandle(metaRecord, &record)
{
}

EstEIDKeyHandle::~EstEIDKeyHandle()
{
}

void EstEIDKeyHandle::getKeySize(CSSM_KEY_SIZE &keySize)
{
	secdebug("esteid.tokend", "EstEIDKeyHandle::getKeySize");
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

uint32 EstEIDKeyHandle::getOutputSize(const Context &context,
	uint32 inputSize, bool encrypting)
{
	secdebug("esteid.tokend", "EstEIDKeyHandle::getOutputSize");
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::generateSignature(const Context &context,
	CSSM_ALGORITHMS signOnly, const CssmData &input, CssmData &signature)
{
	secdebug("esteid.tokend", "EstEIDKeyHandle::generateSignature alg: %u signOnly: %u",
		context.algorithm(), signOnly);
	IFDUMPING("esteid.tokend", context.dump("signature context"));
	secdebug("esteid.tokend", "EstEIDKeyHandle::exportKey");
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::verifySignature(const Context &context,
	CSSM_ALGORITHMS signOnly, const CssmData &input, const CssmData &signature)
{
	secdebug("esteid.tokend", "EstEIDKeyHandle::verifySignature");
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::generateMac(const Context &context,
	const CssmData &input, CssmData &output)
{
	secdebug("esteid.tokend", "EstEIDKeyHandle::generateMac");
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::verifyMac(const Context &context,
	const CssmData &input, const CssmData &compare)
{
	secdebug("esteid.tokend", "EstEIDKeyHandle::verifyMac");
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::encrypt(const Context &context,
	const CssmData &clear, CssmData &cipher)
{
	secdebug("esteid.tokend", "EstEIDKeyHandle::encrypt");
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}

void EstEIDKeyHandle::decrypt(const Context &context,
	const CssmData &cipher, CssmData &clear)
{
	secdebug("esteid.tokend", "EstEIDKeyHandle::decrypt alg: %u", context.algorithm());
	CssmError::throwMe(CSSMERR_CSP_KEY_USAGE_INCORRECT);
}

void EstEIDKeyHandle::exportKey(const Context &context,
	const AccessCredentials *cred, CssmKey &wrappedKey)
{
	secdebug("esteid.tokend", "EstEIDKeyHandle::exportKey");
	CssmError::throwMe(CSSM_ERRCODE_FUNCTION_NOT_IMPLEMENTED);
}


//
// EstEIDKeyHandleFactory
//
EstEIDKeyHandleFactory::~EstEIDKeyHandleFactory()
{
}


Tokend::KeyHandle *EstEIDKeyHandleFactory::keyHandle(
	Tokend::TokenContext *tokenContext, const Tokend::MetaRecord &metaRecord,
	Tokend::Record &record) const
{
	EstEIDKeyRecord &keyRecord = dynamic_cast<EstEIDKeyRecord &>(record);			
	return new EstEIDKeyHandle(metaRecord, record);
}

