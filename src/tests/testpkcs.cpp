#include "precompiled.h"
#include <iostream>
#include "utility/logger.h"
#include "utility/pkcs11module.h"

int tryLoad(std::string libname) {
	std::cout << "-----loading " << libname << std::endl;
 	try {
		pkcs11module mod2(libname.c_str());
		mod2.test(std::cout);
	} catch(std::runtime_error &err) {
		std::cout << "exception: " << err.what() << std::endl;
	}
	std::cout << "----done" << std::endl;
    }

int main(int argc,char **argv) {
	logger log("",log_to_CONSOLE);
	tryLoad("esteidpkcs11-1.0");
	tryLoad("esteidpkcs11");
	return 0;
	}
