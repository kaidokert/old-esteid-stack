#include "precompiled.h"
#include "PKCS11Context.h"
#include "cardlib/PCSCManager.h"
#include "cardlib/EstEidCard.h"
#include "cardlib/helperMacro.h"
#include "utility/asnCertificate.h"
#include "utility/logger.h"
#include "utility/monitorThread.h"
#include <string.h>

#undef min

enum ObjID {
	OBJ_INVALID = -1,
	OBJ_NONE,
	OBJ_CERT,
	OBJ_PRIVKEY,
	OBJ_PUBKEY
} ;

class PKCS11Object;
class searchTerm;

typedef std::vector<PKCS11Object >::iterator objectIter;
typedef std::vector<CK_ATTRIBUTE >::iterator attrIter;

class ObjHandle {
	CK_OBJECT_HANDLE h;
public:
	ObjHandle &operator=(const PKCS11Object &obj);
	ObjHandle(const ObjID &handle) : h(handle) {}
	ObjHandle(const CK_OBJECT_HANDLE &handle) : h(handle) {}
	ObjHandle() : h(OBJ_INVALID) {}
	bool operator==(const ObjID& other) {
		return h == (CK_OBJECT_HANDLE)other;
		}
	bool operator==(const ObjHandle &other) {return h == other.h ; }
	};

class PKCS11Object {
protected:
	ObjHandle handle;
	std::vector<CK_ATTRIBUTE> attrib;
	friend class ObjHandle;
	friend class searchTerm;
public:
	PKCS11Object(ObjID id,CK_ATTRIBUTE *att,size_t count) :
		handle(id),attrib(att,att + count) {}
	bool operator==(const ObjHandle &objHandle) {
		return handle == objHandle;
		}
	attrIter findAttrib(CK_ATTRIBUTE_PTR att) {
		return find(attrib.begin(),attrib.end(),att->type);
		}
	void appendAttrib(CK_ATTRIBUTE att) {
		attrib.push_back(att);
		}
	bool noAttrib(attrIter &ref) {return ref == attrib.end();}
	};

ObjHandle & ObjHandle::operator=(const PKCS11Object &obj) {
	h = obj.handle.h;
	return *this;
	}

CK_OBJECT_CLASS pubkey_class = CKO_PUBLIC_KEY;
CK_OBJECT_CLASS privkey_class = CKO_PRIVATE_KEY;
CK_OBJECT_CLASS cert_class = CKO_CERTIFICATE;
CK_CERTIFICATE_TYPE cert_type = CKC_X_509;
CK_BBOOL _true = TRUE;
CK_KEY_TYPE keyType = CKK_RSA;
CK_UTF8CHAR pubKeyLabel[] = "RSA pubkey";
CK_UTF8CHAR privKeyLabel[] = "RSA privkey";
CK_UTF8CHAR certLabel[] = "RSA cert";
CK_BYTE modulus[128] = {1};
CK_BYTE id1[] = {123};
CK_BYTE id2[] = {123};
CK_BYTE id3[] = {123};
CK_ULONG modBits = 1024;
CK_ATTRIBUTE publicKeyTemplate[] = {
	{CKA_CLASS, &pubkey_class, sizeof(pubkey_class)},
	{CKA_TOKEN, &_true, sizeof(_true)},
	{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
	{CKA_LABEL,pubKeyLabel,sizeof(pubKeyLabel)-1},
	{CKA_ID, id1, sizeof(id1)},
	{CKA_ENCRYPT, &_true, sizeof(_true)},
//	{CKA_VERIFY, &_true, sizeof(_true)},
	{CKA_MODULUS,modulus,sizeof(modulus)},
	{CKA_MODULUS_BITS,&modBits,sizeof(modBits)},
};
CK_ATTRIBUTE privateKeyTemplate[] = {
	{CKA_CLASS, &privkey_class, sizeof(privkey_class)},
	{CKA_TOKEN, &_true, sizeof(_true)},
	{CKA_KEY_TYPE,&keyType,sizeof(keyType)},
	{CKA_PRIVATE,&_true,sizeof(_true)},
	{CKA_LABEL,privKeyLabel,sizeof(privKeyLabel)-1},
	{CKA_ID, id2, sizeof(id2)},
	{CKA_MODULUS,modulus,sizeof(modulus)},
	{CKA_SIGN,&_true,sizeof(_true)}
};
CK_ATTRIBUTE certificateTemplate[] = {
	{CKA_CLASS, &cert_class, sizeof(cert_class)},
	{CKA_TOKEN, &_true, sizeof(_true)},
	{CKA_CERTIFICATE_TYPE, &cert_type, sizeof(cert_type)},
	{CKA_LABEL,certLabel,sizeof(certLabel)-1},
	{CKA_TRUSTED,&_true,sizeof(_true)},
	{CKA_ID, id3, sizeof(id3)},
};

class PKCS11Session {
	friend class PKCS11Context;
//	friend struct SessionChangeState;
	CK_SESSION_HANDLE session;
	CK_SLOT_ID slotID;
	CK_FLAGS flags;
	CK_VOID_PTR pApplication;
	CK_NOTIFY   notify;
	std::vector<CK_ATTRIBUTE > searchParam;
	std::vector<ObjHandle> searchHandles;
	std::vector<PKCS11Object > objects;
	ByteVec authCert;
	ByteVec pubKey;
	ByteVec iss;
	ByteVec ser;
	ByteVec sub;
	std::string pin;
	CK_ULONG state;
	CK_MECHANISM   sigMechanism;
	CK_OBJECT_HANDLE  sigKey;
public:
	struct changeState {
		CK_ULONG mState;
		CK_SLOT_ID mSlot;
		std::string mPin;
		changeState(CK_SLOT_ID slot, CK_ULONG state,std::string pin)
			: mState(state), mSlot(slot),  mPin(pin) {}
		void operator() (PKCS11Session &obj) {
			if (obj.slotID == mSlot) {
				obj.state = mState;
				obj.pin = mPin;
				}
		}
	};
	PKCS11Session(CK_SESSION_HANDLE ,CK_SLOT_ID , CK_FLAGS,CK_VOID_PTR,CK_NOTIFY);
	~PKCS11Session();
	bool operator==(const CK_SESSION_HANDLE& other) {
		return session == other;
		}
};

bool operator==(CK_ATTRIBUTE x,CK_ATTRIBUTE_TYPE a) {
	return x.type == a;
	}

class PKCS11ContextPriv : public monitorObserver {
	mutexObj monitorMutex;
	monitorThread *m_monitorThread;
	friend class PKCS11Context;
	PCSCManager mgr;
	uint readerCount;
	CK_SESSION_HANDLE nextSession;
	std::vector<PKCS11Session > sessions;
	typedef std::vector<PKCS11Session >::iterator sessIter;
	PKCS11ContextPriv() : 
	  nextSession(303),
	  monitorMutex("monitorMutex") {
	  log << "PKCS11Context created" << std::endl;
	  try {
	      log << "creating thread.." << std::endl;
//	      m_monitorThread = new monitorThread(*this,monitorMutex);
	      log << "creating thread done" << std::endl;
	  } catch(std::exception &err) {
	    log << "error when creating monitor:" << err.what() << std::endl;
	    }
	  }
	logger log;
	void onEvent(monitorEvent eType,int param);
};

void PKCS11ContextPriv::onEvent(monitorEvent eType,int param) {
	
}

PKCS11Context::PKCS11Context(void)
{
	d = new PKCS11ContextPriv();
}

PKCS11Context::~PKCS11Context(void)
{
	delete d;
}

PKCS11Session::	PKCS11Session(CK_SESSION_HANDLE sh,CK_SLOT_ID s,
	  CK_FLAGS f,CK_VOID_PTR app,CK_NOTIFY n) :
	session(sh),slotID(s),flags(f),pApplication(app),notify(n),
	state(CKS_RO_PUBLIC_SESSION)
{
}

PKCS11Session::~PKCS11Session(void)
{
}

void PKCS11Context::refreshReaders() {
	d->readerCount = d->mgr.getReaderCount(true);
	}

CK_SESSION_HANDLE PKCS11Context::getNextSessionHandle() {
	return ++d->nextSession;
	}

void padString(CK_UTF8CHAR *s,int len,std::string rstr) {
	memset(s,' ',len);
	memcpy(s,rstr.c_str(),std::min((int)rstr.length(),len));
	}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_GetInfo(CK_INFO_PTR   pInfo  )) {
	FUNC_LOG();

	pInfo->cryptokiVersion.major = 2;
	pInfo->cryptokiVersion.minor = 11;
	padString(pInfo->manufacturerID, sizeof(pInfo->manufacturerID)
		,"EstEID ( pkcs11 opensource )");
	pInfo->flags = 0;
	padString(pInfo->libraryDescription,sizeof(pInfo->libraryDescription)
		,"EstEID PKCS#11 Library");
	pInfo->libraryVersion.major = 0;
	pInfo->libraryVersion.minor = 1;
	return CKR_OK;
	}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_GetSlotList(
		CK_BBOOL       tokenPresent,  /* only slots with tokens? */
		CK_SLOT_ID_PTR pSlotList,     /* receives array of slot IDs */
		CK_ULONG_PTR   pulCount       /* receives number of slots */
		)) {
	FUNC_LOG();

	try {
	uint inBuffer = *pulCount;
	*pulCount = 0;
	refreshReaders();
	*pulCount = d->readerCount;
	d->log << "readerCount:" << d->readerCount;

	if (pSlotList == NULL )
		return CKR_OK;
	if (d->readerCount > inBuffer)
		return CKR_BUFFER_TOO_SMALL;

	CK_SLOT_ID_PTR pSlot = pSlotList;
	for(uint i = 0; i < d->readerCount ; i++ ) {
		EstEidCard card(d->mgr);
		if (!tokenPresent || card.isInReader(i))
			*pSlot++ = i;
		}

	return CKR_OK;
	}catch(std::runtime_error &err) {
		d->log << "exception:" << err.what() << std::endl;
		return CKR_GENERAL_ERROR;
		}
}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_GetTokenInfo(
	CK_SLOT_ID        slotID,  /* ID of the token's slot */
	CK_TOKEN_INFO_PTR pInfo    /* receives the token information */
	)) {
	FUNC_LOG();

	try {
		refreshReaders();
		if (slotID > d->readerCount ) return CKR_SLOT_ID_INVALID;
		EstEidCard card(d->mgr);
		if (!card.isInReader(slotID)) return CKR_DEVICE_REMOVED;
		card.connect(slotID);
		std::string id = card.readCardID();
		std::string name = card.readCardName();

		memset(pInfo, 0, sizeof(*pInfo));
		padString(pInfo->label,sizeof(pInfo->label),name);
		padString(pInfo->manufacturerID, sizeof(pInfo->manufacturerID), "EstEid");
		padString(pInfo->model, sizeof(pInfo->model), "original");
		padString(pInfo->serialNumber, sizeof(pInfo->serialNumber), id);
		pInfo->flags = CKF_WRITE_PROTECTED |CKF_LOGIN_REQUIRED | CKF_USER_PIN_INITIALIZED|
			CKF_TOKEN_INITIALIZED |0;

		pInfo->ulMaxSessionCount = 10;     /* max open sessions */
		pInfo->ulSessionCount = 0;        /* sess. now open */
		pInfo->ulMaxRwSessionCount = 10;   /* max R/W sessions */
		pInfo->ulRwSessionCount = 0;      /* R/W sess. now open */
		pInfo->ulMaxPinLen = 12;           /* in bytes */
		pInfo->ulMinPinLen = 4;           /* in bytes */
		pInfo->ulTotalPublicMemory = 1024;   /* in bytes */
		pInfo->ulFreePublicMemory  = 0;    /* in bytes */
		pInfo->ulTotalPrivateMemory = 1024;  /* in bytes */
		pInfo->ulFreePrivateMemory = 0;   /* in bytes */

		CK_VERSION nulVer = {1,0};
		pInfo->hardwareVersion = nulVer;
		pInfo->firmwareVersion = nulVer;
		return CKR_OK;
	}catch(std::runtime_error &err) {
		d->log << "exception:" << err.what() << std::endl;
		return CKR_GENERAL_ERROR;
		}
}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_GetSlotInfo(
	CK_SLOT_ID        slotID,  /* ID of the token's slot */
	CK_SLOT_INFO_PTR pInfo    /* receives the slot information */
	)) {
	FUNC_LOG();

	try {
		refreshReaders();
		if (slotID > d->readerCount ) return CKR_SLOT_ID_INVALID;
		EstEidCard card(d->mgr);

		memset(pInfo, 0, sizeof(*pInfo));
		std::ostringstream buf;
		buf << "EstSlot " << slotID;
		padString(pInfo->slotDescription,sizeof(pInfo->slotDescription), buf.str()) ;
		padString(pInfo->manufacturerID,sizeof(pInfo->manufacturerID),"EstEID");
		pInfo->flags = CKF_REMOVABLE_DEVICE | CKF_HW_SLOT;
		if (card.isInReader(slotID)) pInfo->flags |= CKF_TOKEN_PRESENT;

		CK_VERSION nulVer = {1,0};
		pInfo->hardwareVersion = nulVer;
		pInfo->firmwareVersion = nulVer;
		return CKR_OK;
	}catch(std::runtime_error &err) {
		d->log << "exception:" << err.what() << std::endl;
		return CKR_GENERAL_ERROR;
		}
}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_GetMechanismList(
	CK_SLOT_ID            slotID,          /* ID of token's slot */
	CK_MECHANISM_TYPE_PTR pMechanismList,  /* gets mech. array */
	CK_ULONG_PTR          pulCount         /* gets # of mechs. */
	)) {
	FUNC_LOG();

	CK_ULONG count = *pulCount;
	refreshReaders();
	if (slotID > d->readerCount ) return CKR_SLOT_ID_INVALID;
	*pulCount = 1;
	if (pMechanismList == NULL )
		return CKR_OK;
	if (count < 1 )
		return CKR_BUFFER_TOO_SMALL;
	pMechanismList[0] = CKM_RSA_PKCS;
	/*pMechanismList[1] = CKM_MD5_RSA_PKCS;
	pMechanismList[2] = CKM_SHA1_RSA_PKCS;*/
	return CKR_OK;
	}

/*
CK_ULONG    ulMinKeySize;
    CK_ULONG    ulMaxKeySize;
    CK_FLAGS    flags;
*/
CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_GetMechanismInfo(
		CK_SLOT_ID            slotID,  /* ID of the token's slot */
		CK_MECHANISM_TYPE     type,    /* type of mechanism */
		CK_MECHANISM_INFO_PTR pInfo    /* receives mechanism info */
		)) {
	FUNC_LOG();

	pInfo->ulMinKeySize = 1024;
	pInfo->ulMaxKeySize = 1024;
	pInfo->flags = CKF_HW | CKF_SIGN /*| CKF_VERIFY*/;
	return CKR_OK;
	}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_InitToken(
		CK_SLOT_ID      slotID,    /* ID of the token's slot */
		CK_UTF8CHAR_PTR pPin,      /* the SO's initial PIN */
		CK_ULONG        ulPinLen,  /* length in bytes of the PIN */
		CK_UTF8CHAR_PTR pLabel     /* 32-byte token label (blank padded) */
		)) {
	FUNC_LOG();

	return CKR_OK;
	}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_OpenSession(
	CK_SLOT_ID            slotID,        /* the slot's ID */
	CK_FLAGS              flags,         /* from CK_SESSION_INFO */
	CK_VOID_PTR           pApplication,  /* passed to callback */
	CK_NOTIFY             Notify,        /* callback function */
	CK_SESSION_HANDLE_PTR phSession      /* gets session handle */
	)) {
	FUNC_LOG();

	try {
		refreshReaders();
		if (slotID > d->readerCount ) return CKR_SLOT_ID_INVALID;
		if (!(flags & CKF_SERIAL_SESSION)) return CKR_SESSION_PARALLEL_NOT_SUPPORTED;

		*phSession = getNextSessionHandle();
		d->sessions.push_back(PKCS11Session(*phSession,slotID,
			flags,pApplication,Notify
			));
		PKCS11ContextPriv::sessIter sessI,sess = --d->sessions.end();
		for(sessI = d->sessions.begin() ; sessI != d->sessions.end() ; sessI++ ) {
			if (sessI->slotID == slotID ) {
				sess->state = sessI->state;
				sess->pin = sessI->pin;
				break;
				}
			}
		{
			EstEidCard card(d->mgr);
			if (!card.isInReader(sess->slotID))
				return CKR_TOKEN_NOT_PRESENT;
			card.connect(sess->slotID);
			sess->authCert = card.getAuthCert();
			std::stringstream dummy;
			asnCertificate cert(sess->authCert,dummy);
			sess->authCert.resize(cert.size + (cert.body_start - cert.start));
			sess->pubKey = cert.getPubKey();
			sess->iss = cert.getIssuerBlob();
			sess->ser = cert.getSerialBlob();
			sess->sub = cert.getSubjectBlob();

			CK_ATTRIBUTE valAttCert= {CKA_VALUE,&sess->authCert[0],(CK_ULONG)sess->authCert.size()};
			CK_ATTRIBUTE valAttPubKey = {CKA_VALUE,&sess->pubKey[0],(CK_ULONG)sess->pubKey.size()};
			CK_ATTRIBUTE valIssuer = {CKA_ISSUER,&sess->iss[0],(CK_ULONG) sess->iss.size()};
			CK_ATTRIBUTE valSerial = {CKA_SERIAL_NUMBER,&sess->ser[0],(CK_ULONG) sess->ser.size()};
			CK_ATTRIBUTE valSubject = {CKA_SUBJECT,&sess->sub[0],(CK_ULONG) sess->sub.size()};
			sess->objects.push_back(PKCS11Object(OBJ_CERT,certificateTemplate,LENOF(certificateTemplate)));
			(--sess->objects.end())->appendAttrib(valAttCert);
			(--sess->objects.end())->appendAttrib(valIssuer);
			(--sess->objects.end())->appendAttrib(valSerial);
			(--sess->objects.end())->appendAttrib(valSubject);
			sess->objects.push_back(PKCS11Object(OBJ_PUBKEY,publicKeyTemplate,LENOF(publicKeyTemplate)));
			(--sess->objects.end())->appendAttrib(valAttPubKey);
			sess->objects.push_back(PKCS11Object(OBJ_PRIVKEY,privateKeyTemplate,LENOF(privateKeyTemplate)));
		}
		return CKR_OK;
	}catch(std::runtime_error &err) {
		d->log << "exception:" << err.what() << std::endl;
		return CKR_GENERAL_ERROR;
		}
}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_GetSessionInfo(
		CK_SESSION_HANDLE   hSession,
		CK_SESSION_INFO_PTR pInfo
		)) {
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;

	pInfo->slotID = sess->slotID ;
	pInfo->state = sess->state;
	pInfo->flags = sess->flags;
	pInfo->ulDeviceError = 0;
	return CKR_OK;
	}

class searchTerm {
	std::vector<CK_ATTRIBUTE > param;
	CK_ULONG mState; //public/private
	friend class PKCS11Object;
public:
	searchTerm(std::vector<CK_ATTRIBUTE > searchParam,CK_ULONG state) :
	  param(searchParam),mState(state) {}
	bool operator() (const PKCS11Object& value) {
		std::vector<CK_ATTRIBUTE >::const_iterator objAtt;
		if (mState == CKS_RO_PUBLIC_SESSION) {//check if CK_PRIVATE is set
			objAtt  =  find(value.attrib.begin(),value.attrib.end(),CKA_PRIVATE);
			if (objAtt != value.attrib.end() ) //private was found
				return true;
			}
		for(attrIter att = param.begin(); att != param.end(); att++) {
			objAtt = find(value.attrib.begin(),value.attrib.end(),att->type);
			if (value.attrib.end() == objAtt) //object does not have required attribute
				return true;
			if (memcmp(objAtt->pValue,att->pValue,std::min(objAtt->ulValueLen,att->ulValueLen )))
				return true; //has attribute but does not match
			}
		return false;
		}
	bool operator() (const ObjHandle& value) {
		return true;
		}
};

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_FindObjectsInit(
	CK_SESSION_HANDLE hSession,   /* the session's handle */
	CK_ATTRIBUTE_PTR  pTemplate,  /* attribute values to match */
	CK_ULONG          ulCount     /* attrs in search template */
	)) {
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;
	sess->searchParam.clear();

	for(uint i = 0; i < ulCount ; i++ )
		sess->searchParam.push_back(*(pTemplate + i));

	sess->searchHandles.clear();
	sess->searchHandles.resize(sess->objects.size());
	std::vector<ObjHandle >::iterator copy =
		remove_copy_if(sess->objects.begin(),sess->objects.end(),
		sess->searchHandles.begin(),searchTerm(sess->searchParam,sess->state));
	sess->searchHandles.resize(copy - sess->searchHandles.begin());

	return CKR_OK;
	}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_FindObjects(
	CK_SESSION_HANDLE    hSession,          /* session's handle */
	CK_OBJECT_HANDLE_PTR phObject,          /* gets obj. handles */
	CK_ULONG             ulMaxObjectCount,  /* max handles to get */
	CK_ULONG_PTR         pulObjectCount     /* actual # returned */
	)) {
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;

	CK_ULONG returnCount = std::min(ulMaxObjectCount, (CK_ULONG)sess->searchHandles.size());
	if (returnCount)
		memcpy(phObject,&sess->searchHandles[0],returnCount * sizeof(*phObject));
	*pulObjectCount = returnCount;
	sess->searchHandles.erase(sess->searchHandles.begin(),sess->searchHandles.begin() + returnCount);
	return CKR_OK;
}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_FindObjectsFinal(
	CK_SESSION_HANDLE hSession  /* the session's handle */
	)) {
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;
	sess->searchParam.clear();
	sess->searchHandles.clear();
	return CKR_OK;
}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_Login(
	CK_SESSION_HANDLE hSession,  /* the session's handle */
	CK_USER_TYPE      userType,  /* the user type */
	CK_UTF8CHAR_PTR   pPin,      /* the user's PIN */
	CK_ULONG          ulPinLen   /* the length of the PIN */
	)) {
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;

    std::string pinIn((const char *) pPin,(size_t)ulPinLen);

    byte retriesLeft;
	try {
        EstEidCard card(d->mgr);
	if (!card.isInReader(sess->slotID))
		return CKR_DEVICE_REMOVED;
	card.connect(sess->slotID);
        card.validateAuthPin(pinIn,retriesLeft);
	}catch(AuthError &ae) {
		std::ostringstream buf;
		buf << ae.what();
		if (retriesLeft)
            return CKR_PIN_INVALID;
        else
            return CKR_PIN_LOCKED;
	}catch(std::runtime_error &err) {
		d->log << "exception:" << err.what() << std::endl;
		return CKR_GENERAL_ERROR;
		}

	sess->pin = pinIn;
	for_each(d->sessions.begin(),d->sessions.end(),
		PKCS11Session::changeState(sess->slotID,CKS_RO_USER_FUNCTIONS,sess->pin));
	return CKR_OK;
	}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_Logout(
	   CK_SESSION_HANDLE hSession  /* the session's handle */ )) {
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;
	for_each(d->sessions.begin(),d->sessions.end(),
		PKCS11Session::changeState(sess->slotID,CKS_RO_USER_FUNCTIONS,sess->pin));
	sess->pin.clear();
	return CKR_OK;
}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_GetAttributeValue(
		CK_SESSION_HANDLE hSession,   /* the session's handle */
		CK_OBJECT_HANDLE  hObject,    /* the object's handle */
		CK_ATTRIBUTE_PTR  pTemplate,  /* specifies attrs; gets vals */
		CK_ULONG          ulCount     /* attributes in template */
		)) {
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;
	objectIter obj = find(sess->objects.begin(),sess->objects.end(),
		ObjHandle(hObject));
	if (obj == sess->objects.end())
		return CKR_OBJECT_HANDLE_INVALID;
	for(uint i = 0; i < ulCount ; i++) {
		CK_ATTRIBUTE_PTR attrib = pTemplate + i;
		attrIter objAttrib = obj->findAttrib(attrib);
		attrib->ulValueLen = -1;
		if (obj->noAttrib(objAttrib))
			continue;
		if (attrib->pValue == NULL) {
			attrib->ulValueLen = objAttrib->ulValueLen;
			continue;
			}
		if (attrib->ulValueLen >= objAttrib->ulValueLen) {
			memcpy(attrib->pValue,objAttrib->pValue,std::min(attrib->ulValueLen,objAttrib->ulValueLen));
			attrib->ulValueLen = objAttrib->ulValueLen;
			continue;
			}
		}
	return CKR_OK;
	}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_SignInit(
	CK_SESSION_HANDLE hSession,    /* the session's handle */
	CK_MECHANISM_PTR  pMechanism,  /* the signature mechanism */
	CK_OBJECT_HANDLE  hKey )) {    /* handle of signature key */
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;
	if (sess->state != CKS_RO_USER_FUNCTIONS)
		return CKR_USER_NOT_LOGGED_IN;

	sess->sigMechanism = *pMechanism;
	sess->sigKey = hKey;
	switch(sess->sigMechanism.mechanism) {
		case CKM_RSA_PKCS:
/*		case CKM_MD5_RSA_PKCS:
		case CKM_SHA1_RSA_PKCS: */break;
		default: return CKR_MECHANISM_INVALID;
		}
	return CKR_OK;
}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_Sign(
	CK_SESSION_HANDLE hSession,        /* the session's handle */
	CK_BYTE_PTR       pData,           /* the data to sign */
	CK_ULONG          ulDataLen,       /* count of bytes to sign */
	CK_BYTE_PTR       pSignature,      /* gets the signature */
	CK_ULONG_PTR      pulSignatureLen )) {/* gets signature length */
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;
	if (sess->state != CKS_RO_USER_FUNCTIONS)
		return CKR_USER_NOT_LOGGED_IN;
	try {
		CK_ULONG len = *pulSignatureLen;
		refreshReaders();
		EstEidCard card(d->mgr);
		if (!card.isInReader(sess->slotID))
			return CKR_DEVICE_REMOVED;
		card.connect(sess->slotID);

		ByteVec result = card.calcSSL(ByteVec(pData,pData + ulDataLen),sess->pin);
		*pulSignatureLen = (CK_ULONG)result.size();
		if (len < result.size() )
			return CKR_BUFFER_TOO_SMALL;
		if (pSignature == NULL)
			return CKR_OK;
		memcpy(pSignature,&result[0],result.size());
		return CKR_OK;
	}catch(AuthError &ae) {
		d->log << "exception:" << ae.what() << std::endl;
	    return CKR_PIN_INCORRECT;
	}catch(CardError &ce) {
		d->log << "exception:" << ce.what() << std::endl;
		return CKR_GENERAL_ERROR;
	}catch(std::runtime_error &err) {
		d->log << "exception:" << err.what() << std::endl;
		return CKR_GENERAL_ERROR;
		}
	return CKR_OK;
}

CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_SignUpdate(
		CK_SESSION_HANDLE hSession,  /* the session's handle */
		CK_BYTE_PTR       pPart,     /* the data to sign */
		CK_ULONG          ulPartLen )) {/* count of bytes to sign */
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;
	if (sess->state != CKS_RO_USER_FUNCTIONS)
		return CKR_USER_NOT_LOGGED_IN;

	return CKR_FUNCTION_NOT_SUPPORTED;
}
CK_DECLARE_FUNCTION(CK_RV,PKCS11Context::C_SignFinal(
		CK_SESSION_HANDLE hSession,        /* the session's handle */
		CK_BYTE_PTR       pSignature,      /* gets the signature */
		CK_ULONG_PTR      pulSignatureLen )) { /* gets signature length */
	FUNC_LOG();

	PKCS11ContextPriv::sessIter sess = find(d->sessions.begin(),d->sessions.end(),hSession);
	if (d->sessions.end() == sess)
		return CKR_SESSION_HANDLE_INVALID;
	if (sess->state != CKS_RO_USER_FUNCTIONS)
		return CKR_USER_NOT_LOGGED_IN;

	return CKR_FUNCTION_NOT_SUPPORTED;
}

