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
	char buffer[20];
	std::string m_prompt;
};

pinDialog::pinDialog(const void * opsysParam,std::string prompt) {
	d = new pinDialogPriv;
	d->m_hInst = ((pinDialogPriv * )opsysParam)->m_hInst;
	d->m_resourceID = ((pinDialogPriv * )opsysParam)->m_resourceID;
	d->m_prompt = prompt;
	}

pinDialog::~pinDialog() {
	}
#define IDC_PININPUT 105 //hack
#define IDC_STATIC              65535

LRESULT CALLBACK dialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static pinDialog * th = NULL;

	switch (message) {
		case WM_INITDIALOG:
			th = (pinDialog *) lParam;
			SetDlgItemTextA(hwnd,IDC_STATIC, th->d->m_prompt.c_str() );
			SendDlgItemMessage( hwnd, IDC_PININPUT , EM_SETLIMITTEXT, 12, 0 );
			SetFocus(GetDlgItem(hwnd, IDC_PININPUT)); 

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDC_PININPUT: {
					if (HIWORD(wParam) == EN_CHANGE) {
						GetDlgItemTextA(hwnd,IDC_PININPUT,th->d->buffer,sizeof(th->d->buffer));
						if (lstrlenA(th->d->buffer) >= (LONG )4)
							EnableWindow(GetDlgItem(hwnd,IDOK),TRUE);
						else
							EnableWindow(GetDlgItem(hwnd,IDOK),FALSE);
						}
					break;
					}
				case IDOK: {
					GetDlgItemTextA(hwnd,IDC_PININPUT,th->d->buffer,sizeof(th->d->buffer));
					}
				case IDCANCEL:
					EndDialog (hwnd,wParam );
					return TRUE;
				}
		case WM_SYSCOMMAND:
			if (wParam == SC_CLOSE) EndDialog (hwnd, IDCANCEL );
			break;
		}
	return FALSE;
	}

bool pinDialog::doDialog() {
	if (IDOK == DialogBoxParam(d->m_hInst,MAKEINTRESOURCE(d->m_resourceID) 
		,GetForegroundWindow(), 
		(DLGPROC)dialogProc, (LPARAM) this)) return true;
	return false;
	}

std::string pinDialog::getPin() {

	return std::string(d->buffer,d->buffer+strlen(d->buffer));
	}

#endif