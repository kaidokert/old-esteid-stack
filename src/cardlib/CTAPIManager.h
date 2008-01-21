/*!
	\file		CTAPIManager.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-26 12:27:14 +0200 (Mon, 26 Nov 2007) $
*/
// Revision $Revision: 149 $
#pragma once
#include "ManagerInterface.h"
#include "DynamicLibrary.h"

#ifdef WIN32
#define CTAPI __stdcall
#else
#define CTAPI
#endif

struct CTDriver {
	struct CTPort {
		CTPort(CTDriver *d,ushort n) : 
			dri(d),portNum(n),isConnected(false),mCtn(0) {}
		CTDriver *dri;
		ushort portNum;
		bool isConnected;
		ushort mCtn;
		void performCmd(byte target,ByteVec cmd,ByteVec &resp,bool consumeStatus = true);
		void performCmd(byte target,ByteVec cmd);
		bool init(bool nothrow = false);
		void close();
		void resetCT(byte unit);
/*		CTPort(const CTPort& ref) : dri(ref.dri),portNum(ref.portNum),
			isConnected(ref.isConnected) {}
	private:
		CTPort& operator=(const CTPort &c) {
			dri = c.dri;
			}*/
	};
	CTDriver(const char *libName,std::vector<ushort>);
	ushort nextCtn; // next connection CTN
	DynamicLibrary lib;
	char (CTAPI *pCTInit)(ushort ctn,ushort pn);
	char (CTAPI *pCTClose)(ushort ctn);
	char (CTAPI *pCTData)(
		ushort ctn,byte * dad,byte * sad,ushort lenc,
		byte * command,ushort * lenr,byte * response);
	std::vector<CTPort> mPorts;
private: //disable object copying
	CTDriver(const CTDriver &ref);
	CTDriver& operator=(const CTDriver &);
	};

typedef CTDriver::CTPort cPort;

/// Holds connection parameters for CTAPI card connection
struct CTAPIConnection : public ConnectionBase {
	CTAPIConnection(ManagerInterface &iface,unsigned int index,bool force) : 
		ConnectionBase(iface,index,force),isT1(false) {}
	CTDriver::CTPort * dri;
	bool wasConnected;
	bool isT1;
	virtual bool isSecure() {return true;}
};

/// Collection of CTAPI libraries
/** CTAPIManager represents a collection of CTAPI libraries for smart card readers, with
 a few builtin known library names and ports for OmniKey and SCM libraries. The libraries 
 are loaded at runtime, to avoid linked-in dependencies */
class CTAPIManager :
	public ManagerInterface 
{
private: //disable object copying
	CTAPIManager(const CTAPIManager &);
	CTAPIManager& operator=(const CTAPIManager &);

	std::vector<CTDriver *> mDrivers;
	std::vector<cPort *> mPorts;

protected:
	void makeConnection(ConnectionBase *c,uint idx);
	void deleteConnection(ConnectionBase *c);
	void beginTransaction(ConnectionBase *c);
	void endTransaction(ConnectionBase *c);
	void execCommand(ConnectionBase *c,std::vector<byte> &cmd,
		std::vector<byte> &recv,unsigned int &recvLen);
	void execPinEntryCommand(ConnectionBase *c,std::vector<byte> &cmd);
	void execPinChangeCommand(ConnectionBase *c,std::vector<byte> &cmd
		,size_t oldPinLen,size_t newPinLen);

	bool isT1Protocol(ConnectionBase *c);

public:
	CTAPIManager(void);
	~CTAPIManager(void);
	uint getReaderCount();
	std::string getReaderName(uint index);
	std::string getReaderState(uint index);
	std::string getATRHex(uint index);
	CTAPIConnection * connect(uint index,bool forceT0);
	/// Gets a first working CTAPI reader library and port
	void getDefaultLibAndPort(std::string &lib,uint &port);
};

