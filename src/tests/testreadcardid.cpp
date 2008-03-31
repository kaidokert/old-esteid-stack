#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include "cardlib/PCSCManager.h"
#include "cardlib/EstEidCard.h"

int main(int argc,char **argv) {
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

	return 0;
	}
