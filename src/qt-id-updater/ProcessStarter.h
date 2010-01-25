/*
* qt-id-updater
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

// http://www.codeproject.com/KB/vista-security/interaction-in-vista.aspx

#ifndef _PROCESS_STARTER_H_
#define _PROCESS_STARTER_H_

#ifndef _DEBUG
#define NOLOG
#endif

#include <string>
#include <sstream>
#include <fstream>

class ProcessStarter
{
	typedef void *_handle;
	typedef _handle *_phandle;
public:
    ProcessStarter(const std::string& processPath, const std::string& arguments = "");
	~ProcessStarter();
    bool Run(bool forceRun = false);
    
	_phandle GetCurrentUserToken();
	_phandle GetCurrentUserTokenOld();
	unsigned long getShellProcessPID();

private:
    std::string processPath_;
    std::string arguments_;
#ifdef NOLOG
	std::stringstream log;
#else
	std::ofstream log;
#endif
};

#endif //_PROCESS_STARTER_H_