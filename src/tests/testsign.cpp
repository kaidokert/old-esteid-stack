#include "precompiled.h"
#include <iostream>
#include <smartcard++/SmartCardManager.h>
#include <smartcard++/esteid/EstEidCard.h>
#include <smartcard++/helperMacro.h>
#include "utility/logger.h"

int main(int argc,char **argv) {
	logger log("",log_to_CONSOLE);
	try {
	SmartCardManager cardMgr;
	cardMgr.setLogging(&log);

	for(uint i = 0;i < cardMgr.getReaderCount();i++) {
		std::cout << cardMgr.getReaderName(i) << " <" << cardMgr.getReaderState(i) << ">" << std::endl;

		EstEidCard card(cardMgr);

		if (card.isInReader(i)) {
			std::cout << "esteid card in here" << std::endl;
			card.connect(i);
			std::cout << "card id:" << card.readCardID() << std::endl;

			unsigned char hash[] = { 0x1,0x2,0x3,0x4};
			ByteVec testHash(MAKEVECTOR(hash));

			PinString pin;
			if (!card.hasSecurePinEntry()) {
				std::cout << "enter pin:";
				std::cin >> pin;
				}
			card.calcSSL(testHash,pin);
			}

	}
	} catch(std::runtime_error &err) {
		std::cout << "exception: " << err.what() << std::endl;
		}

	return 0;
	}
