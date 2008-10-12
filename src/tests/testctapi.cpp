#include "precompiled.h"
#include <iostream>
#include "cardlib/CTAPIManager.h"

int main(int argc,char **argv) {
	CTAPIManager cardMgr;
	for(uint i = 0;i < cardMgr.getReaderCount();i++) {
		std::cout << cardMgr.getReaderName(i) << std::endl;
		}
	return 0;
	}
