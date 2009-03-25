/*!
	\file		pkcs11module.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/

///forward-declared opsys-specific components
struct pinDialogPriv;

class pinDialog {
public:
	pinDialogPriv *d;
	pinDialog(const void * opsysParam,std::string prompt);
	~pinDialog();
	bool doDialog();
	std::string getPin();
};