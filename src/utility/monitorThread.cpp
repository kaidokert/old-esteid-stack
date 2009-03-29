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
void monitorThread::execute() {
	SmartCardManager mgr;
	std::vector<bool> cardPresent;
    while(1) {
        threadObj::wait(500);
        observer.onEvent(READERS_CHANGED,0);
        }
    }
