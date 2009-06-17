/*!
	\file		monitorThread.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#include "threadObj.h"

enum monitorEvent {
    CARD_INSERTED,  //esteid card was inserted
    CARD_REMOVED,	//esteid card was removed
    READERS_CHANGED, //reader config/count changed
	NO_CARDS_LEFT	//no esteid cards anymore, last one removed
    };

struct monitorObserver {
    virtual void onEvent(monitorEvent eType,int param) = 0;
    };

class monitorThread : public threadObj {
	const monitorThread &operator=(const monitorThread &);
    monitorObserver &observer;
    mutexObj &lock;
public:
    monitorThread(monitorObserver &ref,mutexObj &lockRef) :
        observer(ref),threadObj("card monitor thread"),lock(lockRef) {}
    virtual void execute();
    };
