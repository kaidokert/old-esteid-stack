#include "precompiled.h"
#include <iostream>
#include "cardlib/SmartCardManager.h"
#include "cardlib/EstEidCard.h"

int main(int argc,char **argv) {
#if defined(WIN32) && defined(DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF |_CRTDBG_CHECK_ALWAYS_DF |
			_CRTDBG_CHECK_CRT_DF |_CRTDBG_DELAY_FREE_MEM_DF );
#endif

	SmartCardManager cardMgr;
	for(uint i = 0;i < cardMgr.getReaderCount();i++) {
		std::cout << cardMgr.getReaderName(i) << std::endl;

		EstEidCard card(cardMgr);

		if (card.isInReader(i)) {
			std::cout << "esteid card in here" << std::endl;
			card.connect(i);
			std::cout << "card id:" << card.readCardID() << std::endl;
			}

	}

	return 0;
	}
