#include "Keyboard.h"
#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER
Keyboard* Keyboard::ptr = nullptr;

Keyboard::Keyboard(void)
{
	ptr = this;
	pNID = new NOTIFYICONDATA;
}

Keyboard::~Keyboard(void)
{

	ReleaseMutex(hMutex);	
	delete pNID;
}

void Keyboard::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

void Keyboard::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	if (state == SIZE_MINIMIZED)
	{
		ShowWindow(hwnd, SW_HIDE); // Прячем окно
		Shell_NotifyIcon(NIM_ADD, pNID); // Добавляем иконку в трэй
	}
}

DWORD WINAPI Time1(LPVOID lp)
{
	Keyboard* p = (Keyboard*)lp;
	

	
	
	while (TRUE)
	{

		SendMessage(p->hProgress, PBM_SETPOS, WPARAM(p->word), 0);
		TCHAR buff[256];
		wsprintf(buff, TEXT("%d:%d Left"), p->Time.Minutes, p->Time.Seconds);
		
		if (p->Pause) {
			SetWindowText(p->hTime, buff);
			if (p->Time.Seconds <= 0)
			{
				p->Time.Seconds = 60;
				if (p->Time.Minutes <= 0)
				{
					p->Time.Seconds--;
					MessageBox(p->hDialog, L"Time out", L"You lose", MB_OKCANCEL | MB_ICONINFORMATION);
					ExitThread;
					return 0;
				}
				else
				{
					p->Time.Seconds--;
					p->Time.Minutes--;
					p->WPM = 0;
				}
			}
			else
			{
				p->Time.Seconds--;
			}
			if (p->Max_WPM <p-> WPM)
			{
				p->Max_WPM = p->WPM;
			}
			
			Sleep(1000);
		}
	}

	return 0;
}
DWORD WINAPI Time2(LPVOID lp)
{
	Keyboard* p = (Keyboard*)lp;
	while (TRUE)
	{

		SendMessage(p->hProgress, PBM_SETPOS, WPARAM(p->word), 0);
		TCHAR buff[256];
		wsprintf(buff, TEXT("%d:%d now"), p->Time.Minutes, p->Time.Seconds);

		if (p->Pause) {
			SetWindowText(p->hTime, buff);
			if (p->Time.Seconds > 59)
			{
				p->Time.Minutes++;
				p->Time.Seconds = 0;
			}
			else
			{
				p->Time.Seconds++;
			}
			if (p->Max_WPM < p->WPM)
			{
				p->Max_WPM = p->WPM;
			}

			Sleep(1000);
		}
	}



}
DWORD WINAPI thread_CPS(LPVOID lp) {
	Keyboard* p = (Keyboard*)lp;
	TCHAR buff[256];
	while (true)
	{
		wsprintf(buff, TEXT("%d"), p->WPM);
		SetWindowText(p->hCPS, buff);
		Sleep(1000);
		
	}
}




void Keyboard::Words(WPARAM wp, LPARAM lp)   //отказатся от потока на функцию , удобней передавать из нажатия на кнопки
{
	
		TCHAR buff[2048];
		if (Game == true) {
			if (wp == toupper(a[0])) {
				a.erase(0, 1);

			}
			if (a[0] == ' ')
			{
				WPM++;
				word++;

			}
			else if (a.length() < 1)
			{
				word++;
			}


			while (a[0] == ' ' || a[0] == ',' || a[0] == '.' || a[0] == '\r' || a[0] == '\n')
			{
				a.erase(0, 1);


			}


			if (a.length() < 1)
			{
				Level++;

				if (word >= MaxWords)
				{
					TCHAR cps[256];
					if (Limit) {
						
						wsprintf(cps, TEXT("WPM : %d"), Max_WPM);
						if (Level < Max_Levels) {
							Pause = false;
							TCHAR NewGame[256];
							wsprintf(NewGame, TEXT("Начать игру Level : %d"), Level + 1);
							TCHAR lvl[256];
							wsprintf(lvl, TEXT("Level: %d"), Level + 1);
							SetWindowText(hDialog, lvl);
							int res = MessageBox(hDialog, NewGame, cps, MB_OKCANCEL | MB_ICONINFORMATION);

							if (res == IDOK) {

								Pause = true;
								Max();
								SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, MaxWords));
								SendMessage(hProgress, PBM_SETPOS, 0, 0);

								word = 0;
								a = words[Level];


							}
							else
							{
								Game = false;
							}

						}
					}

					else
					{
						Game = false;
						Pause = false;
						int res = MessageBox(hDialog, TEXT("You win"), cps, MB_OK | MB_ICONINFORMATION);
					}
				}



			}
			if (Level < Max_Levels) {
				lf.lfHeight = 46;
				hFont = CreateFontIndirect(&lf);
				SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);


				SetTextColor(HDC(hEdit), color);
				wsprintf(buff, a.c_str());
				SetWindowText(hEdit, buff);
			}

		}
	
}
	



void Keyboard::Max() {

	MaxWords = 0;
	int i = 0;
	if (Limit) {
		while (true)
		{
			if (words[Level][i] == '\0') {
				if (words[Level][i - 1] != ' ')
				{
					MaxWords++;
				}
				break;
			}
			else if (words[Level][i] == ' ' && words[Level][i - 1] != ' ') {
				MaxWords++;
			}

			i++;
		}
	}
	else
	{
		while (true)
		{
			if (Infinite_words[i] == '\0') {
				if (Infinite_words[i - 1] != ' ')
				{
					MaxWords++;
				}
				break;
			}
			else if (Infinite_words[i] == ' ' && Infinite_words[i - 1] != ' ') {
				MaxWords++;
			}

			i++;
		}
	}
}


BOOL Keyboard::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{

	
	HMENU hMenu = GetMenu(hDialog);

	SetWindowText(hwnd, TEXT("Keyboard game"));

	WPM = 0;
	TCHAR GUID[] = TEXT("<2D0D4B98-3601-495A-9E62-9829492C6AD5>");
	hMutex = CreateMutex(NULL, FALSE, GUID);
	DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
	if (dwAnswer == WAIT_TIMEOUT)
	{
		MessageBox(hwnd, TEXT("Нельзя запускать более одной копии приложения!!!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
		EndDialog(hwnd, 0);
	}


	for (int i = 0; i < 26; i++)
	{
		hButton[i] = GetDlgItem(hwnd, IDC_BUTTON1 + i);
		lf.lfHeight = 16;

		hFont = CreateFontIndirect(&lf);
		SendMessage(hButton[i], WM_SETFONT, (WPARAM)hFont, TRUE);
	}
	hCPS = GetDlgItem(hwnd, IDC_EDIT2);
	hTime = GetDlgItem(hwnd, IDC_EDIT3);
	hProgress = GetDlgItem(hwnd, IDC_PROGRESS1);
	


	hEdit = GetDlgItem(hwnd, IDC_EDIT1);
	
	CreateThread(NULL, 0, thread_CPS, this, 0, NULL);
	

	hDialog = hwnd;
	
	HINSTANCE hInst = GetModuleHandle(NULL);
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); // загружаем иконку
	SetClassLong(hDialog, GCL_HICON, LONG(hIcon)); // устанавливаем иконку в главном окне приложения
	memset(pNID, 0, sizeof(NOTIFYICONDATA)); //Обнуление структуры
	pNID->cbSize = sizeof(NOTIFYICONDATA); //размер структуры
	pNID->hIcon = hIcon; //загружаем пользовательскую иконку
	pNID->hWnd = hwnd; //дескриптор окна, которое будет получать уведомляющие сообщения,
	// ассоциированные с иконкой в трэе.	
	

	pNID->uCallbackMessage = WM_ICON;

	pNID->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;

	pNID->uID = ID_TRAYICON; // предопределённый идентификатор иконки

	return TRUE;
}

void Keyboard::OnTrayIcon(WPARAM wp, LPARAM lp)
{
	// WPARAM - идентификатор иконки
	// LPARAM - сообщение от мыши или клавиатурное сообщение
	if (lp == WM_LBUTTONDBLCLK)
	{
		Shell_NotifyIcon(NIM_DELETE, pNID); // Удаляем иконку из трэя
		ShowWindow(hDialog, SW_NORMAL); // Восстанавливаем окно
		SetForegroundWindow(hDialog); // устанавливаем окно на передний план
	}
}


void Keyboard::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

	if (id == ID_MENU_LEADERBOARD)
	{
		Leaderboard dlg(Max_WPM,Time.Minutes,Time.Seconds);
		TCHAR buffer[200];

		DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwnd, Leaderboard::DlgProc);
	

	}


	if (id == ID_MENU_NEWGAME)
	{
		Pause = false;
		
		
		int res = MessageBox(hwnd, TEXT("Начать игру Level 1"), TEXT("Начать?"), MB_OKCANCEL | MB_ICONINFORMATION);

		if (res == IDOK) {

			TCHAR lvl[256];
			wsprintf(lvl, TEXT("Level: %d"), Level);
			SetWindowText(hDialog, lvl);
			a = words[Level];
			Game = true;
			lf.lfHeight = 16;
			hFont = CreateFontIndirect(&lf);
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);


			SetTextColor(HDC(hEdit), color);
			SetWindowText(hEdit, TEXT("Press any ctrl to start"));
		}
		
		

	}
	
	if (!Pause) {
	
		if (id == ID_Limit)
		{
			if (Limit) {
				// Получим дескриптор главного меню
				HMENU hMenu = GetMenu(hDialog);
				// Снимем отметку с пункта меню "Строка состояния"
				CheckMenuItem(hMenu, ID_Limit, MF_BYCOMMAND | MF_UNCHECKED);
				// Скроем строку состояния
				Limit = false;
			}

			else
			{
				Limit = true;
				HMENU hMenu = GetMenu(hDialog);

				CheckMenuItem(hMenu, ID_Limit, MF_BYCOMMAND | MF_CHECKED);

			}
		}
	}
	else
	{
		if (id == ID_MENU_RESTART)
		{
			int res = MessageBox(hwnd, TEXT("Начать игру"), TEXT("Начать?"), MB_OKCANCEL | MB_ICONINFORMATION);
			Pause = false;
			if (res == IDOK) {

				TCHAR lvl[256];
				wsprintf(lvl, TEXT("Level: %d"), Level);
				SetWindowText(hDialog, lvl);
		
				Game = true;
				lf.lfHeight = 16;
				hFont = CreateFontIndirect(&lf);
				SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
				word = 0;
				SendMessage(hProgress, PBM_SETPOS, 0, 0);

				SetTextColor(HDC(hEdit), color);
				SetWindowText(hEdit, TEXT("Press any ctrl to start"));
			}
			else
			{
				Pause = true;
			}

		}
	}
}


void Keyboard::Cls_OnCheck(WPARAM wp, LPARAM lp) {


	if (Game) {
		if (wp == VK_CONTROL && Pause == false)
		{
			TerminateThread(h, 0);
			Pause = true;
			if (Limit) {
				Max();
				SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, MaxWords));
				SendMessage(hProgress, PBM_SETPOS, 0, 0);
				h = CreateThread(NULL, 0, Time1, this, 0, NULL);
				Time.Minutes = 1;
				Time.Seconds = 0;
				a = words[Level];
			}
			else
			{
				Max();
				WPM = 0;
				SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, MaxWords));
				SendMessage(hProgress, PBM_SETPOS, 0, 0);
				h = CreateThread(NULL, 0, Time2, this, 0, NULL);
				Time.Minutes = 0;
				a = Infinite_words;
				Time.Seconds = 0;
				
			}

		}
	}


	if (Pause) {
		Words(wp, lp);
		
		for (int i = 0; i < 26; i++)
		{
			//EnableWindow(hButton[i], FALSE);
			if (wp == key[i])
			{
				
				lf.lfHeight = 46;
				hFont = CreateFontIndirect(&lf);
				SendMessage(hButton[i], WM_SETFONT, (WPARAM)hFont, TRUE);
				//EnableWindow(hButton[i], TRUE);
			}
			else {

				lf.lfHeight = 16;

				hFont = CreateFontIndirect(&lf);
				SendMessage(hButton[i], WM_SETFONT, (WPARAM)hFont, TRUE);
			}

		}
	}



	

}



BOOL CALLBACK Keyboard::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog,);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);

		HANDLE_MSG(hwnd, WM_SIZE, ptr->Cls_OnSize);
		
	}
	
	if (message == WM_ICON)
	{
		ptr->OnTrayIcon(wParam, lParam);
		return TRUE;
	}
	if (message == WM_KEYDOWN) {
		ptr->Cls_OnCheck(wParam, lParam);
		
		return TRUE;
	}
	if(message == WM_CTLCOLOREDIT) { //сообщение приходит при необходимости покраски текстового поля
		SetTextColor((HDC)ptr->hButton[0], ptr->color);//установим цвет текста
		
	}

	return FALSE;
}