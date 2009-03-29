#include "precompiled.h"
#include <iostream>
#include <string>
#include "utility/monitorThread.h"

struct testFix : public monitorObserver {
    void onEvent(monitorEvent eType,int param) {
        std::string event;
        switch(eType) {
            case CARD_INSERTED:event="cardInserted";break;
            case CARD_REMOVED:event="cardRemoved";break;
            case READERS_CHANGED:event="readersChanged";break;
            }
        std::cout << "thread event " << event << " param " << param << std::endl;
        }
    };

int main(int argc,char **argv) {
    testFix oneOb;
    monitorThread mthread(oneOb);
    mthread.start();
    while(1) {
        sleep(1);
        std::cout << "main prog tick" << std::endl;
        }
}
