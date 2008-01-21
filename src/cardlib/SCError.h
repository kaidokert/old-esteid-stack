/*!
	\file		SCError.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author: kaido $
	\date		$Date: 2007-11-26 12:27:14 +0200 (Mon, 26 Nov 2007) $
*/
// Revision $Revision: 149 $
#pragma once

/// Exception class for smartcard subsystem errors
/** Smartcard subsystem errors, like reader busy etc. Currently these are only
 thrown for PCSCManager, but CTAPI should derive its own from here and throw them
 as well */
class SCError :
	public std::runtime_error
{
	std::string desc;
public:
    const long error; //SC Api returns longs
	SCError(long err);
    	virtual ~SCError() throw() {}
	virtual const char * what() const throw() {	return desc.c_str();} 
	static void check(long err);
};
