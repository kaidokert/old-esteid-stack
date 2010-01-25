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
