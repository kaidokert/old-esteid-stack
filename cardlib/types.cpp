/*!
	\file		types.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#include "precompiled.h"

std::ostream& operator<<(std::ostream &out,const ByteVec &vec) {
  out << "[ ";
  for(ByteVec::const_iterator i = vec.begin();i!=vec.end();i++) 
    out << std::hex << std::setfill('0') << std::setw(2) << (int)*i << " ";
  out << "]";
  return out;
  }
