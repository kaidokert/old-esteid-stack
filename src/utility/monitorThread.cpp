/*!
	\file		monitorThread.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2009-03-29 17:48:54 +0300 (Sun, 29 Mar 2009) $
*/
#include "precompiled.h"
#include "monitorThread.h"
#include "cardlib/SmartCardManager.h"
#include "cardlib/EstEidCard.h"

#include <iostream>

struct checker {
    monitorObserver &observer;
	SmartCardManager mgr;
    EstEidCard card;
	std::vector<bool> cardPresent;
	checker(monitorObserver &ref) : observer(ref),card(mgr) {}
	void executeCheck();
    bool readerHasCard(EstEidCard &card,int i);
    };

bool checker::readerHasCard(EstEidCard &card,int i) {
    std::string state = mgr.getReaderState(i);
    if (state.find("PRESENT") == std::string::npos ) return false;
    if (cardPresent[i]) //caching/optimization
        return true;
    return card.isInReader(i);
    }

void checker::executeCheck() {
    int readers = mgr.getReaderCount(true);
    if (readers != cardPresent.size()) {
        cardPresent.resize(readers);
        observer.onEvent(READERS_CHANGED,0);
        }
    for (size_t i = 0; i < cardPresent.size() ; i++ ) {
        if (readerHasCard(card,i) && !cardPresent[i]) {
            cardPresent[i] = true;
            observer.onEvent(CARD_INSERTED,i);
            }
        if (!readerHasCard(card,i) && cardPresent[i]) {
            cardPresent[i] = false;
            observer.onEvent(CARD_REMOVED,i);
            }
        }
    }

void monitorThread::execute() {
    std::cout << "monitorThread::execute"<<std::endl;
    checker check(observer);
    while(1) {
        mutexObjLocker lock_(lock);
        threadObj::wait(500);
        check.executeCheck();
        }
    }