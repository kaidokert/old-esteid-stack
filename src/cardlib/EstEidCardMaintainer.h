/*!
	\file		EstEidCardMaintainer.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )    
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$
#pragma once
#include "EstEidCard.h"

class EstEidCardMaintainer {
	EstEidCard card;
public:
	EstEidCardMaintainer(EstEidCard &ref) : card(ref) {}
	~EstEidCardMaintainer() {}
	void performGenerateNewKeys();
	};