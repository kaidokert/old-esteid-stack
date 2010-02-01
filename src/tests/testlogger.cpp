#include <smartcard++/common.h>
#include <smartcard++/helperMacro.h>
#include "utility/logger.h"
#include <iostream>

using std::endl;

int main(int argc,char **argv) {
  unsigned char bytebuf[] = {0x01,0x02,0xDE,0xAD};
  ByteVec buffer(MAKEVECTOR(bytebuf));
  logger buf;
std::cout << "--1" << std::endl;
  buf << "mushrooms ";
  buf << "badger " << 12*2 << " and a snake " << endl;
std::cout << "--2" << std::endl;
  buf << "football" << endl;
std::cout << "--3" << std::endl;
  buf << buffer << endl;
std::cout << "--4" << std::endl;
  buf << "dead cow:" << buffer << endl;
  return 0;
}