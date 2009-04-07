#include "precompiled.h"
#include <iostream>
#include "cardlib/CTAPIManager.h"
#include "utility/logger.h"

int main(int argc,char **argv) {
	logger log(logTarget::log_to_CONSOLE,"");
	try {
	CTAPIManager cardMgr(&std::cout);
	cardMgr.setLogging(&log);

	for(uint i = 0;i < cardMgr.getReaderCount();i++) {
		std::cout << cardMgr.getReaderName(i) << std::endl;
		}
	} catch(std::runtime_error &err) {
		std::cout << "exception: " << err.what() << std::endl;
		std::cout << "log:" << log.str() << std::endl;
		}
	return 0;
	}
