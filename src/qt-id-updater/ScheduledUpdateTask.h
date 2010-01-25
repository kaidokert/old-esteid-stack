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

#pragma once

#if defined(_WIN32)
#include <atlbase.h>
#include <comutil.h>
#include <Mstask.h>
#endif

#include <string>

class ScheduledUpdateTask
{
#if defined(_WIN32)
	CComPtr<ITaskScheduler> pITS;
	CComQIPtr<ITask> pITask;
	CComQIPtr<IPersistFile> pIPersistFile;
#endif
	std::wstring m_command;
	std::wstring m_name;
public:
	enum Interval {
		DAILY,
		WEEKLY,
		MONTHLY,
		NEVER
	} ;
	ScheduledUpdateTask(std::wstring command,std::wstring name);
	bool configure(Interval interval,bool autoupdate);
	bool remove();
};

