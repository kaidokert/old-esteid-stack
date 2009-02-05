#include "precompiled.h"
#include <iostream>
#include "cardlib/PCSCManager.h"
#include "cardlib/EstEidCard.h"

int main(int argc,char **argv) {
	try {
	PCSCManager cardMgr;

	for(uint i = 0;i < cardMgr.getReaderCount();i++) {

		std::cout << cardMgr.getReaderName(i) << std::endl;

		EstEidCard card(cardMgr);

		if (card.isInReader(i)) {
			std::cout << "esteid card in here" << std::endl;
			card.connect(i);
			std::cout << "card id:" << card.readCardID() << std::endl;
			}
		}
	} catch(std::runtime_error &err) {
		std::cout << "exception: " << err.what() << std::endl;
		}
	return 0;
	}
