/*!
	\file		EstEID_utility.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author: kaidokert $
	\date		$Date: 2008-11-16 19:12:33 +0200 (Sun, 16 Nov 2008) $
*/
// Revision $Revision: 146 $

class blockLog {
	const char *m_block;
	const char *m_param;
public:
	blockLog(const char *logBlock,const char *param);
	blockLog(const char *logBlock);
	~blockLog();
};

#define FLOG blockLog _fLog(__PRETTY_FUNCTION__,"function ")
