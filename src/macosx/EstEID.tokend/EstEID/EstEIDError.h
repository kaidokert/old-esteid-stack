/*!
	\file		EstEIDError.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/

/*
 *  EstEIDError.h
 */

#ifndef _ESTEIDERROR_H_
#define _ESTEIDERROR_H_

#include <tokend/SCardError.h>


class EstEIDError : public Tokend::SCardError
{
protected:
    EstEIDError(uint16_t sw);
	virtual ~EstEIDError() throw ();
public:
    virtual const char *what () const throw ();

    static void check(uint16_t sw)	{ if (sw != SCARD_SUCCESS) throwMe(sw); }
    static void throwMe(uint16_t sw) __attribute__((noreturn));
protected:
    IFDEBUG(void debugDiagnose(const void *id) const;)
};

#endif /* !_ESTEIDERROR_H_ */

