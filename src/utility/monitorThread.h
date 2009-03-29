/*!
	\file		monitorThread.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2009-03-29 17:48:54 +0300 (Sun, 29 Mar 2009) $
*/
#include "threadObj.h"

enum monitorEvent {
    CARD_INSERTED,
    CARD_REMOVED,
    READERS_CHANGED
    };

struct monitorObserver {
    virtual void onEvent(monitorEvent eType,int param) = 0;
    };

class monitorThread : public threadObj {
    monitorObserver &observer;
    mutexObj &lock;
public:
    monitorThread(monitorObserver &ref,mutexObj &lockRef) :
        observer(ref),threadObj("card monitor thread"),lock(lockRef) {}
    virtual void execute();
    };
