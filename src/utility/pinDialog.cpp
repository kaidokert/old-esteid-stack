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

struct pinDialogPriv_a {
	HINSTANCE m_hInst;
	WORD m_resourceID;
};

struct pinDialogPriv {
	pinDialogPriv_a params;
/*	HINSTANCE m_hInst;
	WORD m_resourceID;*/
	HWND m_hwnd;
	char m_buffer[20];
	std::string m_prompt;
	virtual LRESULT on_message(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT on_command(WPARAM wParam, LPARAM lParam);
	virtual void on_init_dlg();
};


pinDialog::pinDialog(const void * opsysParam,std::string prompt) {
	d = new pinDialogPriv;
	d->params = *((pinDialogPriv_a*) opsysParam);
/*	d->m_hInst = ((pinDialogPriv * )opsysParam)->m_hInst;
	d->m_resourceID = ((pinDialogPriv * )opsysParam)->m_resourceID;*/
	d->m_prompt = prompt;
	}

pinDialog::~pinDialog() {
	}
#define IDC_PININPUT 105 //hack
#define IDC_STATIC              65535

void pinDialogPriv::on_init_dlg() {
	SetDlgItemTextA(m_hwnd,IDC_STATIC, m_prompt.c_str() );
	SendDlgItemMessage( m_hwnd, IDC_PININPUT , EM_SETLIMITTEXT, 12, 0 );
	SetFocus(GetDlgItem(m_hwnd, IDC_PININPUT));
}

LRESULT pinDialogPriv::on_command(WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
		case IDC_PININPUT: {
			if (HIWORD(wParam) == EN_CHANGE) {
				GetDlgItemTextA(m_hwnd,IDC_PININPUT,m_buffer,sizeof(m_buffer));
				if (lstrlenA(m_buffer) >= (LONG )4)
					EnableWindow(GetDlgItem(m_hwnd,IDOK),TRUE);
				else
					EnableWindow(GetDlgItem(m_hwnd,IDOK),FALSE);
				}
			break;
			}
		case IDOK:
		case IDCANCEL:
			GetDlgItemTextA(m_hwnd,IDC_PININPUT,m_buffer,sizeof(m_buffer));
			EndDialog (m_hwnd,wParam );
			return TRUE;
		}
	return FALSE;
	}

LRESULT pinDialogPriv::on_message(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
	case WM_INITDIALOG:
		on_init_dlg();
		return TRUE;
	case WM_COMMAND:
		return on_command(wParam,lParam);
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) EndDialog (hwnd, IDCANCEL );
	}
  return FALSE;
}

LRESULT CALLBACK dialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	pinDialog *dlg = NULL;

	if (message == WM_INITDIALOG) {
		SetWindowLongPtr(hwnd, DWLP_USER, lParam);
		dlg = (pinDialog *)lParam;
		if (!dlg) return TRUE;
		dlg->d->m_hwnd = hwnd;
		}
	dlg = (pinDialog *)GetWindowLongPtr(hwnd, DWLP_USER);
	if (!dlg)
	    return FALSE;
	return dlg->d->on_message(hwnd, message, wParam, lParam);
	}

bool pinDialog::doDialog() {
	if (IDOK == DialogBoxParam(d->params.m_hInst,MAKEINTRESOURCE(d->params.m_resourceID)
		,GetForegroundWindow(),
		(DLGPROC)dialogProc, (LPARAM) this)) return true;
	return false;
	}

std::string pinDialog::getPin() {
	return std::string(d->m_buffer,d->m_buffer+strlen(d->m_buffer));
	}

#endif

#ifdef linux
#include <gtkmm/inputdialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/stock.h>
#include <string.h>

struct pinDialogPriv : public Gtk::Dialog {
    Gtk::Entry m_textInput;
    Gtk::Label m_label;
    pinDialogPriv(const void *opsysParam) :
        Gtk::Dialog("inputDialog",true), m_label("pin entry")
        {
        m_textInput.set_activates_default(true);
        get_vbox()->pack_start(m_label);
        m_label.set_alignment(0.1,0.5);
        get_vbox()->pack_start(m_textInput);
        m_textInput.set_visibility(false);
        set_has_separator(true);
        add_button(Gtk::Stock::OK,Gtk::RESPONSE_OK);
        add_button(Gtk::Stock::CANCEL ,Gtk::RESPONSE_CANCEL);
        set_default_response(Gtk::RESPONSE_CANCEL);
        show_all_children();
        }
    std::string getPin() {
        return m_textInput.get_text().c_str();
        }
    std::string m_prompt;
    char m_buffer[20];
    bool doDialog();
};


bool pinDialogPriv::doDialog() {
    if (run()==Gtk::RESPONSE_OK) return true;
    return false;
    }

pinDialog::pinDialog(const void * opsysParam,std::string prompt) {
	d = new pinDialogPriv(opsysParam);
	d->m_prompt = prompt;
	}

bool pinDialog::doDialog() {
    return d->doDialog();
	}

std::string pinDialog::getPin() {
    return d->getPin();
	}

pinDialog::~pinDialog() {
	}

#endif
