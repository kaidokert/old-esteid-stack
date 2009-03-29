#include "precompiled.h"
#include <iostream>
#include <fstream>
#include "utility/monitorThread.h"

struct testFix : public monitorObserver {
    void onEvent(monitorEvent eType,int param) {
        std::cout << "thread tick" << std::endl;
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
