#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include "utility/netObj.h"

using std::string;
typedef std::vector<unsigned char> ByteVec;

void getPassword(void *ptr,std::string prompt,int maxinput,std::string &pin) {
	std::cout << "prompt:" << prompt ;
	std::cin >> pin;
	std::cout << std::endl << " pin input:'" << pin << "'" << std::endl;
	}

void saveTemp(ByteVec &contents,string iname) {
	std::ofstream strm(iname.c_str(),std::ios::binary);
	strm.write((const char*)&contents[0],std::streamsize(contents.size()));
	}

int main(int argc,char **argv) {
	void *clientCert = 0;
	try {
		netObj net(0);
		net.init(0);
		string appUrl("/x/kodanik/");
		netConnect conn(net,"portaal-id.riik.ee", HTTPS ,clientCert );

		std::cout << "Retrieving data.." << std::endl;
		ByteVec page1,page2,picBytes;

	conn.getHttpsFile(appUrl + "index.php?fp=querylist",page1);
	saveTemp(page1,"page_dl.html");

	} catch(std::runtime_error &err) {
		std::cout << "exception: " << err.what() << std::endl;
		}
	return 0;
	}
