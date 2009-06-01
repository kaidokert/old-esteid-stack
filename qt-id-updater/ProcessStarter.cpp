// http://www.codeproject.com/KB/vista-security/interaction-in-vista.aspx
//#define NO_PROCESS
#include "ProcessStarter.h"

#include <windows.h>
#include <winbase.h>
#include "userenv.h"
#include "wtsapi32.h"
#include "winnt.h"
#include "DynamicLibrary.h"

#include <iomanip>

ProcessStarter::ProcessStarter(const std::string& processPath, const std::string& arguments)
: processPath_(processPath), arguments_(arguments) 
{
#ifndef NOLOG
	char tpath[MAX_PATH];
	GetTempPathA(sizeof(tpath),tpath);
	strcat(tpath,"id-updater.log");
	log.open(tpath);
#endif
}
#if !defined(WIN32) || defined(NO_PROCESS)
bool ProcessStarter::Run() {return false;}
#else

//#pragma comment(lib,"WtsApi32")
#pragma comment(lib,"Userenv")

HANDLE curTok;
HANDLE primTok;

PHANDLE GetCurrentUserToken()
{
	DynamicLibrary wts("Wtsapi32");
	BOOL (__stdcall * pWTSQueryUserToken)(ULONG SessionId,PHANDLE phToken) =
		(BOOL(__stdcall *)(ULONG,PHANDLE)) wts.getProc("WTSQueryUserToken");
	BOOL (__stdcall * pWTSEnumerateSessionsW)(HANDLE hServer,DWORD Reserved,DWORD Version,
			PWTS_SESSION_INFOW * ppSessionInfo,DWORD * pCount) = 
		(BOOL(__stdcall *)(HANDLE, DWORD,DWORD,PWTS_SESSION_INFOW *, DWORD *))
			wts.getProc("WTSEnumerateSessionsW");

    PHANDLE currentToken = &curTok;
    PHANDLE primaryToken = &primTok;

    int dwSessionId = 0;

    PWTS_SESSION_INFOW pSessionInfo = 0;
    DWORD dwCount = 0;

    pWTSEnumerateSessionsW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &dwCount);

    for (DWORD i = 0; i < dwCount; ++i)
    {
        WTS_SESSION_INFO si = pSessionInfo[i];
        if (WTSActive == si.State)
        {
            dwSessionId = si.SessionId;
            break;
        }
    }

    BOOL bRet = pWTSQueryUserToken(dwSessionId, currentToken);
    int errorcode = GetLastError();
    if (bRet == false)
    {
        return 0;
    }

    bRet = DuplicateTokenEx(*currentToken, TOKEN_ASSIGN_PRIMARY | TOKEN_ALL_ACCESS, 0, SecurityImpersonation, TokenPrimary, primaryToken);
    errorcode = GetLastError();
    if (bRet == false)
    {
        return 0;
    }

    return primaryToken;
}

bool ProcessStarter::Run()
{
	OSVERSIONINFO version = {sizeof(OSVERSIONINFO)};

	GetVersionEx(&version);
	if (version.dwMajorVersion <= 5) //dont need this on XP/Win2K
		return false;

	char winDir[260];
	GetSystemDirectoryA(winDir,sizeof(winDir));

	PHANDLE primaryToken = 0;
	try {
		primaryToken = GetCurrentUserToken();
	} catch(...) {}
    if (primaryToken == 0)
    {
		log << "primtok = 0" << std::endl;
        return false;
    }

    STARTUPINFOA StartupInfo;
    PROCESS_INFORMATION processInfo;
	memset(&StartupInfo,0,sizeof(StartupInfo));
    StartupInfo.cb = sizeof(StartupInfo);
#if 0
	std::string command = std::string("\"") + winDir + 
			"\\cmd.exe\" /C \""+ processPath_ + " " + arguments_ + "\"";
#else
    std::string command = "\"" + processPath_ + "\"";
    if (arguments_.length() != 0)
    {
        command += " " + arguments_;
    }
#endif
	log << "command:" << command << std::endl;

    void* lpEnvironment = NULL;
    BOOL resultEnv = CreateEnvironmentBlock(&lpEnvironment, primaryToken, FALSE);
    if (resultEnv == 0)
    {                                
        long nError = GetLastError();                                
    }
	log << "CreateEnvironmentBlock ok" << std::endl;

    BOOL result = CreateProcessAsUserA(*primaryToken, 0, (LPSTR)(command.c_str()), 
		NULL,NULL,
		FALSE, CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT, 
		lpEnvironment, 0, &StartupInfo, &processInfo);

	log << "CreateProcessAsUserA " << result << " err 0x" 
		<< std::hex << std::setfill('0') << std::setw(8) << GetLastError() << std::endl;
	if (result != FALSE)
		log << "launched" << std::endl;
	else
		log << "didnt launch" << std::endl;
    CloseHandle(primaryToken);
    return (result != FALSE);
}
#endif