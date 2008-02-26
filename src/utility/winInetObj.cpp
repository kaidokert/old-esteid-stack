/*!
	\file		winInetObj.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#include "precompiled.h"
#include "netObj.h"
#include "winInetObj.h"

#ifdef WIN32

using std::string;
using std::vector;

class inetError:public std::runtime_error {
public:
	DWORD error;
	std::string desc;
	inetError(string op) : 
		error(GetLastError()), std::runtime_error("WinInet error") {
		std::ostringstream buf;
		buf << "WinInet exception:'" << op << 
			"' code:'0x" <<std::hex << std::setfill('0') <<
			std::setw(8) << error << "'";
		desc = buf.str();
		}
	virtual const char * what() const throw() {	return desc.c_str();} 
	void static check(string op,HINTERNET handle) {
		if (handle == NULL) 
			throw inetError(op);
		}
	void static check(string op,BOOL result) {
		if (result == FALSE) 
			throw inetError(op);
		}
};

winInetObj::winInetObj(void *) : DynamicLibrary("wininet") {
	pInternetOpenA = (HINTERNET (STD *)(
			IN LPCSTR lpszAgent,IN DWORD dwAccessType,
			IN LPCSTR lpszProxy OPTIONAL,IN LPCSTR lpszProxyBypass OPTIONAL,
			IN DWORD dwFlags))
		getProc("InternetOpenA");
	pInternetCloseHandle = (BOOL(STD *)(IN HINTERNET hInternet))
		getProc("InternetCloseHandle");
	pInternetConnectA = (HINTERNET (STD *)(
			IN HINTERNET hInternet, IN LPCSTR lpszServerName,
			IN INTERNET_PORT nServerPort, IN LPCSTR lpszUserName OPTIONAL, 
			IN LPCSTR lpszPassword OPTIONAL,
			IN DWORD dwService, IN DWORD dwFlags, IN DWORD_PTR dwContext))
		getProc("InternetConnectA");
	pInternetReadFile = (BOOL (STD *)(
			IN HINTERNET hFile,   IN LPVOID lpBuffer,
			IN DWORD dwNumberOfBytesToRead,OUT LPDWORD lpdwNumberOfBytesRead))
		getProc("InternetReadFile");
	pInternetErrorDlg = (DWORD (STD *)(
			IN HWND hWnd,IN OUT HINTERNET hRequest,
			IN DWORD dwError,IN DWORD dwFlags,
			IN OUT LPVOID * lppvData))
		getProc("InternetErrorDlg");
	pHttpSendRequestA = (BOOL (STD *)(
			IN HINTERNET hRequest,IN LPCSTR lpszHeaders OPTIONAL,IN DWORD dwHeadersLength,
			IN LPVOID lpOptional OPTIONAL,IN DWORD dwOptionalLength))
		getProc("HttpSendRequestA");
	pHttpQueryInfoA = (BOOL (STD *)(
			IN HINTERNET hRequest,IN DWORD dwInfoLevel,
			IN OUT LPVOID lpBuffer OPTIONAL,IN OUT LPDWORD lpdwBufferLength,
			IN OUT LPDWORD lpdwIndex OPTIONAL))
		getProc("HttpQueryInfoA");
	pHttpOpenRequestA = (HINTERNET (STD *)(
			IN HINTERNET hConnect,IN LPCSTR lpszVerb,IN LPCSTR lpszObjectName,IN LPCSTR lpszVersion,
			IN LPCSTR lpszReferrer OPTIONAL,IN LPCSTR FAR * lplpszAcceptTypes OPTIONAL,IN DWORD dwFlags,
			IN DWORD_PTR dwContext ))
		getProc("HttpOpenRequestA");
	pInternetSetOptionA = (BOOL (STD *)(
			HINTERNET hInternet, DWORD dwOption, 
			LPVOID lpBuffer, DWORD dwBufferLength))
		getProc("InternetSetOptionA");

	inetError::check("openSession",hSession = pInternetOpenA(
                        "Wininet Client App",
                        PRE_CONFIG_INTERNET_ACCESS,
                        NULL, 
                        INTERNET_INVALID_PORT_NUMBER,
                        0));
	}

winInetObj::~winInetObj() {
	pInternetCloseHandle(hSession);
}

inetConnect::inetConnect(winInetObj &net,string site,connType type,void *auth) :
		mNet(net),authCert((PCCERT_CONTEXT)auth),authenticated(false) {
	int port;
	DWORD service;
	if (type == HTTP) {
		port = INTERNET_DEFAULT_HTTP_PORT;
		service = INTERNET_SERVICE_HTTP;
	} else {
		port = INTERNET_DEFAULT_HTTPS_PORT;
		service = INTERNET_SERVICE_HTTP;
		}
	inetError::check(string("InternetConnect '") + site + "'",
		connect = mNet.pInternetConnectA(
							net.hSession,
							site.c_str(),
							port,"","",
							service,0,0));
	}

inetConnect::~inetConnect() {
		mNet.pInternetCloseHandle(connect);
		}

#define SECUREFLAGS INTERNET_FLAG_SECURE | \
					INTERNET_FLAG_IGNORE_CERT_CN_INVALID | \
                    INTERNET_FLAG_IGNORE_CERT_DATE_INVALID | \
					INTERNET_FLAG_KEEP_CONNECTION

struct inetRequest {
	HINTERNET file;
	inetConnect &mConnect;
	inetRequest(inetConnect &conn,
			const char *verb,string url) : mConnect(conn) {
		inetError::check(string("OpenRequest '") + verb + " " + url + "'",
			file = mConnect.mNet.pHttpOpenRequestA(mConnect,
				verb,url.c_str(),NULL,"",NULL,SECUREFLAGS,0));
		}
	~inetRequest() {
		mConnect.mNet.pInternetCloseHandle(file);
		}
	operator HINTERNET() const {return file;}
	};

#define DIALOGOPT FLAGS_ERROR_UI_FILTER_FOR_ERRORS | \
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | \
				FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS

bool inetConnect::getHttpsFile(
	  string url,vector<byte> &buffer) {
	inetRequest req(*this,"GET",url);

	if (!authenticated) 
		inetError::check("InternetSetOption(authCert)",
		mNet.pInternetSetOptionA(req,INTERNET_OPTION_CLIENT_CERT_CONTEXT,
			(void *) authCert,sizeof(CERT_CONTEXT)));

	//authenticated = true;

	while(! mNet.pHttpSendRequestA(req,NULL,0,0,0)) {
		if (GetLastError() == ERROR_INTERNET_CLIENT_AUTH_CERT_NEEDED) {
			mNet.pInternetErrorDlg(NULL,req,
				ERROR_INTERNET_CLIENT_AUTH_CERT_NEEDED,DIALOGOPT,NULL);
			}
		else 
			throw inetError(string("SendRequest '") + url +"'");
		}

	DWORD dwCode,dwSize = sizeof(DWORD);
	inetError::check(string("HttpQueryInfo '") + url +"'",
		mNet.pHttpQueryInfoA(req,HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
			&dwCode,&dwSize,NULL));
	if (dwCode != 200 ) {
		std::ostringstream buf;
		buf << url << " not found:" << dwCode;
		throw std::runtime_error(buf.str());
		}

	DWORD bytesTotal = 0;
    DWORD bytesRead;
	DWORD readChunk = 4096;

	buffer.resize(readChunk * 4);
	do {
		if (bytesTotal > (buffer.size() - readChunk))
			buffer.resize(buffer.size() + (readChunk * 4));
		inetError::check(string("pInternetReadFile '") + url +"'",
			mNet.pInternetReadFile(req, 
			&buffer[bytesTotal],readChunk ,&bytesRead));
		bytesTotal +=bytesRead;
	} while(bytesRead != 0 );
	buffer[bytesTotal] = 0;
	buffer.erase(buffer.begin() + bytesTotal,buffer.end());
	return true;
	}
#endif
