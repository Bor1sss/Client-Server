#include "Keyboard.h"

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	Keyboard dlg;
	INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX) };
	icc.dwICC = ICC_WIN95_CLASSES;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Keyboard::DlgProc);
}