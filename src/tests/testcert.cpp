#include "precompiled.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include "utility/asnCertificate.h"

using std::ios;
using std::endl;

int err(const char *e) {
    std::cout << e << endl;
    return 1;
    }

int main(int argc,char **argv) {
	if (argc < 2)
	    return err("cert file param required");

    std::ifstream sin(argv[1],ios::binary);
    if (!sin.good())
        return err("couldnt open the file");
    sin.seekg (0, ios::end);
    byteVec buf(sin.tellg(),0);
    sin.seekg (0, ios::beg);
    sin.read((char *)&buf[0],buf.size());
    try {
        asnCertificate cer(buf,std::cout);
        std::cout << "subject: " << cer.getSubject() << endl;
        std::cout << "subjectAltName: " << cer.getSubjectAltName() << endl;
        std::cout << "validFrom: " << cer.getValidFrom() << endl;
        std::cout << "validTo: " << cer.getValidTo() << endl;
        std::cout << "epired: " << (cer.isTimeValid(0) ? "no" : "yes") << endl;
    } catch(std::runtime_error e) {
        return err(e.what());
        }
}
