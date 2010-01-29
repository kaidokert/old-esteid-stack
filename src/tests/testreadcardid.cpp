#include "precompiled.h"
#include <iostream>
#include <smartcard++/PCSCManager.h>
#include <smartcard++/esteid/EstEidCard.h>
#include "utility/logger.h"

int main(int argc,char **argv) {
	logger log("",log_to_CONSOLE);
	try {
	PCSCManager cardMgr;
	cardMgr.setLogging(&log);

	for(uint i = 0;i < cardMgr.getReaderCount();i++) {

		std::cout << cardMgr.getReaderName(i) << std::endl;

		EstEidCard card(cardMgr);

		if (card.isInReader(i)) {
			std::cout << "esteid card in here" << std::endl;
			card.connect(i);
			std::cout << "card id:" << card.readCardID() << std::endl;
			std::cout << "card name:" << card.readCardName() << std::endl;
			}
		}
	} catch(std::runtime_error &err) {
		std::cout << "exception: " << err.what() << std::endl;
		}
	return 0;
	}
