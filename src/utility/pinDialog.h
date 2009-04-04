/*!
	\file		pinDialog.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/

///forward-declared opsys-specific components
struct pinDialogPriv;
#include <cardlib/EstEidCard.h>
#include "pinDialog_res.h"

class pinDialog {
	pinDialogPriv *d;
protected:
	EstEidCard::KeyType m_key;
	std::string m_prompt;
	int m_minLen;
	friend struct pinDialogPriv;
public:
	pinDialog(const void * opsysParam,std::string prompt);
	pinDialog(const void * opsysParam,EstEidCard::KeyType key);
	~pinDialog();
	bool doDialog();
	std::string getPin();
};