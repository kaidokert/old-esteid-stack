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
public:
    ProcessStarter(const std::string& processPath, const std::string& arguments = "");
    bool Run();
    
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