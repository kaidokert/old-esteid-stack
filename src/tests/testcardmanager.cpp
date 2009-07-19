#include "precompiled.h"
#include <iostream>
#include <smartcard++/SmartCardManager.h>
#include <smartcard++/esteid/EstEidCard.h>
#include "utility/logger.h"

int main(int argc,char **argv) {
	logger log("",log_to_CONSOLE);
#if defined(_WIN32) && defined(DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF |_CRTDBG_CHECK_ALWAYS_DF |
			_CRTDBG_CHECK_CRT_DF |_CRTDBG_DELAY_FREE_MEM_DF );
#endif
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
			}

	}
	} catch(std::runtime_error &err) {
		std::cout << "exception: " << err.what() << std::endl;
		}

	return 0;
	}
