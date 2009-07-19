/*!
	\file		pinDialog.cpp
	\copyright	(c) Kaido Kert ( kaidokert@gmail.com )
	\licence	BSD
	\author		$Author$
	\date		$Date$
*/
// Revision $Revision$

#include "precompiled.h"
#include "pinDialog.h"
#include "threadObj.h"

#ifdef _WIN32
#include <windows.h>
#include <crtdbg.h>

struct iconHandle {
	HMODULE m_module;
	HICON m_icon;
	iconHandle(const char *modName,int id) {
		m_module = LoadLibraryExA(modName,NULL,LOAD_LIBRARY_AS_DATAFILE);
		m_icon = LoadIcon(m_module,MAKEINTRESOURCE(id));
		}
	~iconHandle() {
		if (m_icon) DestroyIcon(m_icon);
		if (m_module) FreeLibrary(m_module);
		}
	operator const HANDLE() { return m_icon;}
	operator const LPARAM() { return (LPARAM) m_icon;}
	};

struct pinDialogPriv {
	struct _icontag {
		const char *module;
		int id;
	} static iconSet[2];
	struct pinDialogPriv_a {
		HINSTANCE m_hInst;
		WORD m_resourceID;
	} params;
	pinDialog &m_dlg;
	HWND m_hwnd;
	char m_buffer[20];
	iconHandle *dlgIcon,*appIcon;
	pinDialogPriv(pinDialog &ref,const void * opsysParam) : m_dlg(ref),dlgIcon(NULL),appIcon(NULL) {
		params = *((pinDialogPriv_a*) opsysParam);
		}
	~pinDialogPriv() {
		if (dlgIcon) delete dlgIcon;
		if (appIcon) delete appIcon;
		}
	virtual LRESULT on_message(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT on_command(WPARAM wParam, LPARAM lParam);
	virtual LRESULT on_init_dlg(WPARAM wParam);
	static LRESULT CALLBACK dialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool doDialog();
	bool showPrompt(std::string,bool allowRetry=false);
	PinString getPin();
private:
	const pinDialogPriv &operator=(const pinDialogPriv &o);
};

pinDialogPriv::_icontag pinDialogPriv::iconSet[2] = {{"cryptui.dll", 4998},{"cryptui.dll" ,3425}};

LRESULT pinDialogPriv::on_init_dlg(WPARAM wParam) {
	SetWindowPos(m_hwnd, HWND_TOPMOST, 0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	SetDlgItemTextA(m_hwnd,IDC_STATIC, m_dlg.m_prompt.c_str() );
	SendDlgItemMessage( m_hwnd, IDC_PININPUT , EM_SETLIMITTEXT, 12, 0 );

	dlgIcon = new iconHandle(iconSet[m_dlg.m_key].module,iconSet[m_dlg.m_key].id);
	appIcon = new iconHandle("shell32",48);
	SendDlgItemMessage(m_hwnd,IDI_DLGICON,STM_SETIMAGE,IMAGE_ICON,(LPARAM)*dlgIcon);
	SendMessage(m_hwnd,WM_SETICON,(WPARAM) ICON_SMALL,(LPARAM) *appIcon);
	SendMessage(m_hwnd,WM_SETICON,(WPARAM) ICON_BIG,(LPARAM) *appIcon);
	if (GetDlgCtrlID((HWND) wParam) != IDC_PININPUT) {
		SetFocus(GetDlgItem(m_hwnd, IDC_PININPUT));
		return FALSE;
		}
	return TRUE;
}

LRESULT pinDialogPriv::on_command(WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
		case IDC_PININPUT: {
			if (HIWORD(wParam) == EN_CHANGE) {
				GetDlgItemTextA(m_hwnd,IDC_PININPUT,m_buffer,sizeof(m_buffer));
				if (lstrlenA(m_buffer) >= (LONG ) m_dlg.m_minLen) {
					EnableWindow(GetDlgItem(m_hwnd,IDOK),TRUE);
					SendMessage(m_hwnd,DM_SETDEFID,IDOK,0);
					}
				else {
					EnableWindow(GetDlgItem(m_hwnd,IDOK),FALSE);
					SendMessage(m_hwnd,DM_SETDEFID,IDCANCEL,0);
					}
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
		return on_init_dlg(wParam);
	case WM_COMMAND:
		return on_command(wParam,lParam);
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) EndDialog (hwnd, IDCANCEL );
	}
  return FALSE;
}

LRESULT CALLBACK pinDialogPriv::dialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	pinDialogPriv *dlg = NULL;

	if (message == WM_INITDIALOG) {
		SetWindowLongPtr(hwnd, DWLP_USER, (LONG)lParam);
		dlg = (pinDialogPriv *)lParam;
		if (!dlg) return TRUE;
		dlg->m_hwnd = hwnd;
		}
	dlg = (pinDialogPriv *)GetWindowLongPtr(hwnd, DWLP_USER);
	if (!dlg)
	    return FALSE;
	return dlg->on_message(hwnd, message, wParam, lParam);
	}

bool pinDialogPriv::doDialog() {
	if (IDOK == DialogBoxParam(params.m_hInst,MAKEINTRESOURCE(params.m_resourceID)
		,GetForegroundWindow(),
		(DLGPROC)dialogProc, (LPARAM) this)) return true;
	return false;
	}

bool pinDialogPriv::showPrompt(std::string prompt,bool allowRetry) {
	int code = MessageBoxA(GetForegroundWindow(),prompt.c_str(),
		"PIN message", (allowRetry ? MB_RETRYCANCEL : MB_OK ) | MB_ICONHAND );
	return (IDRETRY == code  || IDOK == code);
	}

PinString pinDialogPriv::getPin() {
	return PinString(m_buffer,m_buffer+strlen(m_buffer));
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
    pinDialogPriv(pinDialog &,const void *opsysParam) :
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
    PinString getPin() {
        return m_textInput.get_text().c_str();
        }
    std::string m_prompt;
    char m_buffer[20];
    bool doDialog();
	bool showPrompt(std::string prompt,bool allowRetry=false) {
		}
};

bool pinDialogPriv::doDialog() {
    if (run()==Gtk::RESPONSE_OK) return true;
    return false;
    }

#endif

pinDialog::pinDialog(const void * opsysParam,std::string prompt) : m_minLen(4),
	m_key((EstEidCard::KeyType)0) {
	d = new pinDialogPriv(*this,opsysParam);
	m_prompt = prompt;
	}

pinDialog::pinDialog(const void * opsysParam,EstEidCard::KeyType key) : m_key(key) {
	d = new pinDialogPriv(*this,opsysParam);
	if (m_key == EstEidCard::AUTH) {
		m_prompt = "Enter authentication PIN";
		m_minLen = 4;
		}
	else if (m_key == EstEidCard::SIGN) {
		m_prompt = "Enter signature PIN";
		m_minLen = 5;
		}
	else
		throw std::invalid_argument("pinDialog:Invalid keytype specified");
	}

pinDialog::~pinDialog() {
	delete d;
	}

bool pinDialog::doDialog() {
	return d->doDialog();
	}

bool pinDialog::showPrompt(std::string prompt,bool allowRetry) {
	return d->showPrompt(prompt,allowRetry);
	}

bool pinDialog::doDialogInloop(pinOpInterface &operation,PinString &authPinCache) {
	for(;;) {
		byte retries = 0;
		try {
				PinString pin;
				if (authPinCache.empty()) {
					if (!doDialog())
						throw std::runtime_error("User cancelled");
					pin = getPin();
				} else
					pin = authPinCache;
				mutexObjLocker lock(operation.m_mutex);
				if (m_key == EstEidCard::AUTH)
					operation.m_card.validateAuthPin(pin,retries);
				else
					operation.m_card.validateSignPin(pin,retries);
				operation.call(operation.m_card,pin,m_key);
				authPinCache = pin;
				return true;
		} catch(AuthError &auth) {
			authPinCache.clear();
			if (auth.m_blocked) {
				showPrompt("Wrong pin entered, PIN is blocked");
				throw std::runtime_error("PIN is blocked");
				}
			std::stringstream buf;
			buf << "Wrong pin entered, " << (int)retries << " retries left";
			if (!showPrompt(buf.str(),true))
				throw std::runtime_error("User cancelled");
			}
		}
	}

PinString pinDialog::getPin() {
	return d->getPin();
	}
