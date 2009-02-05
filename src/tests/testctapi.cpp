#include "precompiled.h"
#include <iostream>
#include "cardlib/CTAPIManager.h"

int main(int argc,char **argv) {
	try {
	CTAPIManager cardMgr(&std::cout);
	for(uint i = 0;i < cardMgr.getReaderCount();i++) {
		std::cout << cardMgr.getReaderName(i) << std::endl;
		}
	} catch(std::runtime_error &err) {
		std::cout << "exception: " << err.what() << std::endl;
		}
	return 0;
	}
