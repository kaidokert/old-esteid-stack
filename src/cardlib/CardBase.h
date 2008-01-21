/*!
	\file		CardBase.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-12-05 00:09:38 +0200 (Wed, 05 Dec 2007) $
*/
// Revision $Revision: 215 $
#pragma once
#include "ManagerInterface.h"

using std::vector;

/// Exception class for smart card errors, keeps the SW1 SW2 codes
class CardError: public std::runtime_error {
public:
	const byte SW1,SW2;
	std::string desc;
	CardError(byte a,byte b);
	virtual const char * what() const throw() {	return desc.c_str();} 
	virtual ~CardError() throw(){};
	};

/// Exception class thrown when unexpected or inconistent data is read from card
class CardDataError: public std::runtime_error {
public:
	CardDataError( std::string p):runtime_error(p) {}
	};

/// Exception class for authentication errors, like wrong PIN input etc.
class AuthError :public CardError {
public:
	AuthError(byte a,byte b) : CardError(a,b) {};
	AuthError(CardError _base) : CardError(_base) {}
};

/// Represents basic ISO7816-4 smartcard
/** Represents a basic smart card, with basic ISO7816-4 command set implemented
 Concrete instances of smart cards with appropriate services can be derived from it
 or it can be used directly with basic command set. */
class CardBase
{
protected:
	/// File Control Info structure, parsed
	struct FCI {
		word	fileID;
		dword	fileLength;
		dword	recCount;
		dword	recMaxLen;
	} LPFCI;
	ManagerInterface &mManager;
	ConnectionBase	*mConnection;
	std::ostream	*mLogger;

	/// helper to parse returned TLVs from card
	ByteVec getTag(int identTag,int len,ByteVec &arr);
	/// Parses a File Control Infromation block from select file commands
	FCI parseFCI(ByteVec fci);
	/// Selects the Master File on card
	FCI selectMF(void);
	/// Selects Data File given by two-byte fileID
	int selectDF(int fileID);
	/// Selects Elementary File given by two-byte fileID
	FCI selectEF(int fileID);
	/// Reads a record from record-based Elementary File
	ByteVec readRecord(int numrec);
	/// Read entire binary Elementary File
	ByteVec readEF(unsigned int fileLen);
	/// perform a basic APDU command. noreply indicates that command expects no reply data apart from status bytes
	virtual ByteVec execute(ByteVec cmd,bool noreply=false);
	/// perform pin entry command. this is only useful if card manager supports direct pin entry from reader like CTAPI
	virtual void executePinEntry(ByteVec cmd);
	/// perform pin change command. useful if card manager supports direct pin entry
	virtual void executePinChange(ByteVec cmd, size_t oldPinLen,size_t newPinLen);
public:
	/// Constructor, call connect() to connect card to a reader
	CardBase(ManagerInterface &ref);
	/// Constructor, connects the card instance to the reader at index idx
	CardBase(ManagerInterface &ref,unsigned int idx);
	/// Constructor, connects the card instance to existing connection
	CardBase(ManagerInterface &ref,ConnectionBase *conn);

	~CardBase(void);
	/// connects the card instance to the reader at index idx
	void connect(unsigned int idx,bool forceT0=false);
	/// virtual to be overridden by concrete cards, that can check for ATR or other card specific data
	virtual bool isInReader(unsigned int idx) {return false;}
	/// set logging stream. set to NULL for no logging ( default )
	void setLogging(std::ostream *logStream);
};
