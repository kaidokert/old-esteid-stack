/*!
	\file		PCSCManager.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "SmartCardManager.h"
#include "PCSCManager.h"
#include "CTAPIManager.h"

enum {
	MANAGER_PCSC,
	MANAGER_CTAPI
	} mgrType;

struct SmartCardConnectionPriv {
	PCSCConnection *pcscConn;
	CTAPIConnection *ctConn;
	SmartCardConnectionPriv(int manager,ManagerInterface &iface,unsigned int index,bool force) {
		if (manager = MANAGER_PCSC)
			pcscConn = new PCSCConnection(iface,index,force);
		if (manager= MANAGER_CTAPI)
			ctConn = new CTAPIConnection(iface,index,force);
		}
};

SmartCardConnection::SmartCardConnection(int manager,ManagerInterface &iface,unsigned int index,bool force) 
	:ConnectionBase(iface,index,force) {
		d = new SmartCardConnectionPriv(manager,iface,index,force);
		}

SmartCardConnection::~SmartCardConnection() {
	delete d;
	}

struct SmartCardManagerPriv {
	PCSCManager pcscMgr;
	CTAPIManager ctMgr;
	uint pcscCount;
	uint ctCount;
	SmartCardManagerPriv() {
		pcscCount = pcscMgr.getReaderCount();
		ctCount = ctMgr.getReaderCount();
		}

	ManagerInterface & getIndex(uint &i) {
		if (i <= pcscCount ) 
			return pcscMgr;
		i-= pcscCount;
		return ctMgr;
		}
	};

SmartCardManager::SmartCardManager(void) {
	d = new SmartCardManagerPriv();
}

SmartCardManager::~SmartCardManager(void) {
	delete d;
}

void SmartCardManager::makeConnection(ConnectionBase *c,uint idx) {
	}

void SmartCardManager::deleteConnection(ConnectionBase *c) {
	}

void SmartCardManager::beginTransaction(ConnectionBase *c) {
	}

void SmartCardManager::endTransaction(ConnectionBase *c,bool forceReset) {
	}

void SmartCardManager::execCommand(ConnectionBase *c,std::vector<BYTE> &cmd,std::vector<BYTE> &recv,
								   unsigned int &recvLen) {
}

bool SmartCardManager::isT1Protocol(ConnectionBase *c) {
	return true;
	}

uint SmartCardManager::getReaderCount() {
	return d->ctMgr.getReaderCount() + d->pcscMgr.getReaderCount();
	}

std::string SmartCardManager::getReaderName(uint idx) {
	uint t_idx = idx;
	return d->getIndex(t_idx).getReaderName(t_idx);
	}

std::string SmartCardManager::getReaderState(uint idx) {
	uint t_idx = idx;
	return d->getIndex(t_idx).getReaderState(t_idx);
	}

std::string SmartCardManager::getATRHex(uint idx) {
	uint t_idx = idx;
	return d->getIndex(t_idx).getATRHex(t_idx);
	}

SmartCardConnection * SmartCardManager::connect(uint idx,bool forceT0) {
	return 0;
	}

SmartCardConnection * SmartCardManager::reconnect(ConnectionBase *c,bool forceT0) {
	return 0;
	}
