#include "Client.h"


int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow) {

	Client dlg;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Client::DlgProc);
}

