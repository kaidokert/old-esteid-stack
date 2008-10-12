#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include "cardlib/SmartCardManager.h"

int main(int argc,char **argv) {
	SmartCardManager cardMgr;
	for(uint i = 0;i < cardMgr.getReaderCount();i++) {
		std::cout << cardMgr.getReaderName(i) << std::endl;
		}

	return 0;
	}
