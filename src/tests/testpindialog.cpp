#include "precompiled.h"
#include "utility/pinDialog.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,int nCmdShow ) {
	struct {
		HINSTANCE m_hInst;
		WORD m_resourceID;
	} params = { hInstance, IDD_PIN_DIALOG_ENG};
#else
#include <gtkmm/main.h>
int main(int argc,char **argv) {
    Gtk::Main kit(&argc, &argv);
	struct {
	} params;
#endif
	pinDialog dlg(&params,"pls enter PIN");
	dlg.doDialog();
	std::cout << "pin:" << dlg.getPin() << std::endl;
	pinDialog dlg1(&params, EstEidCard::AUTH );
	dlg1.doDialog();
	pinDialog dlg2(&params, EstEidCard::SIGN );
	dlg2.doDialog();
	dlg.doNonmodalNotifyDlg(true);
}
