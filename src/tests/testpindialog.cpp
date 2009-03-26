#include "precompiled.h"
#include "utility/pinDialog.h"
#ifdef WIN32
#include <windows.h>
#include "testpindialog_resource.h"
#endif

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,int nCmdShow ) {
	struct {
		HINSTANCE m_hInst;
		WORD m_resourceID;
	} params = { hInstance, IDD_PIN_DIALOG_ENG};
#else
int main(int argc,char **argv) {
#endif
	pinDialog dlg(&params,"pls enter PIN");
	dlg.doDialog();
}