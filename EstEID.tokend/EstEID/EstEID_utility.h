/*!
	\file		EstEID_utility.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

class blockLog {
	const char *m_block;
	const char *m_param;
public:
	blockLog(const char *logBlock,const char *param);
	blockLog(const char *logBlock);
	~blockLog();
};

#define FLOG blockLog _fLog(__PRETTY_FUNCTION__,"function ")
