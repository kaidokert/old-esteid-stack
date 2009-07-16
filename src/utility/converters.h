/*!
	\file		converters.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

std::string toHex(const std::vector<unsigned char> & byteVec);
std::vector<unsigned char> fromHex(const std::string & hexStr);
