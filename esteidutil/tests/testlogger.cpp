#include "cardlib/common.h"
#include "cardlib/helperMacro.h"
#include "cardlib/ManagerInterface.h"
#include "utility/logger.h"

using std::endl;

int main(int argc,char **argv) {
  unsigned char bytebuf[] = {0x01,0x02,0xDE,0xAD};
  ByteVec buffer(MAKEVECTOR(bytebuf));
  logger buf;
printf("--1\n");
  buf << "mushrooms ";
  buf << "badger " << 12*2 << " and a snake " << endl;
printf("--2\n");
  buf << "football" << endl;
printf("--3\n");
  buf << buffer << endl;
printf("--4\n");
  buf << "dead cow:" << buffer << endl;
  return 0;
}