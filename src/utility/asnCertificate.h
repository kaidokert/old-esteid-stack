/*!
	\file		AsnCertificate.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-26 20:56:37 +0200 (Mon, 26 Nov 2007) $
*/
// Revision $Revision: 150 $
#include "asnObject.h"

class asnCertificate : public asnObject {
	asnObject * signatureValue;
	asnObject * signatureAlgorithm;

	asnObject * version;
	asnObject * serialNumber;
	asnObject * signatureAlg;
	asnObject * issuerName;
	asnObject * validityPeriod;
	asnObject * subjectName;
	asnObject * publicKeyInfo;
	asnObject * extensions;

	void init();
public:
	asnCertificate(byteVec &in,std::ostream &pout);
	asnObject *findExtension(std::string ext);
	string getSubjectAltName();
	bool isTimeValid();
	string getValidFrom();
	string getValidTo();
	string getSubject();
	vector<byte> getSubjectCN();
	vector<byte> getSubjectO();
	vector<byte> getSubjectOU();
	vector<byte> getIssuerCN();
	vector<byte> getIssuerO();
	vector<byte> getIssuerOU();
	bool checkKeyUsage(std::string keyUsageId);
	bool hasExtKeyUsage();

};
