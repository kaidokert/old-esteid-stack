/*!
	\file		EstEID_utility.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#include "EstEID_utility.h"
#include <security_utilities/debugging.h>

blockLog::blockLog(const char *logBlock,const char *param) : m_block(logBlock), m_param(param){
	secdebug("tok_esteid","->: %s%s",m_param,m_block);
	}
blockLog::blockLog(const char *logBlock) : m_block(logBlock),m_param("") {
	secdebug("tok_esteid","->: %s",m_block);
	}
blockLog::~blockLog() {
	secdebug("tok_esteid","<-: %s%s",m_param,m_block);
	}
