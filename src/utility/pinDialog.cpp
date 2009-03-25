/*!
	\file		pkcs11module.h
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
#include "precompiled.h"
#include "pinDialog.h"

#ifdef WIN32
#include <windows.h>

struct pinDialogPriv{
	HINSTANCE m_hInst;
	WORD m_resourceID;
};

pinDialog::pinDialog(const void * opsysParam,std::string prompt) {
	d = new pinDialogPriv;
	d->m_hInst = ((pinDialogPriv * )opsysParam)->m_hInst;
	d->m_resourceID = ((pinDialogPriv * )opsysParam)->m_resourceID;
	}

pinDialog::~pinDialog() {
	}

LRESULT CALLBACK dialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return 0;
	}

bool pinDialog::doDialog() {
	if (IDOK == DialogBoxParam(d->m_hInst,MAKEINTRESOURCE(d->m_resourceID) 
		,GetForegroundWindow(), 
		(DLGPROC)dialogProc, (LPARAM) 0)) return true;
	return false;
	}

std::string pinDialog::getPin() {
	return "";
	}

#endif