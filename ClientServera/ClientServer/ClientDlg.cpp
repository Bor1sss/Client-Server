#include "Client.h"

Client* Client::ptr = NULL;


Client::Client(void)
{
	ptr = this;
}

Client::~Client(void)
{
	closesocket(_socket);
	//ќсвобождаем ресурсы
	WSACleanup();
}

void Client::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL Client::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hChat = GetDlgItem(hwnd, IDC_LIST1);
	hMessage = GetDlgItem(hwnd, IDC_EDIT1);
	hConnect = GetDlgItem(hwnd, IDC_BUTTON3);
	hIp = GetDlgItem(hwnd, IDC_EDIT3);
	return TRUE;
}



void Client::PrintChat()
{
	TCHAR buff[256];

	
	mbstowcs(buff, buff_client, 256);

	SendMessage(hChat, LB_ADDSTRING, 0, LPARAM(buff));

}
DWORD WINAPI GetM(LPVOID lp)
{
	Client *p = (Client*)lp;
	
	while (true) {
		int i = recv(p->_socket, p->buff_client, p->MAXSTRLEN, 0);
		p->buff_client[i+2] = '\0';
		p->buff_client[i] = '-';
		p->buff_client[i+1] = 'S';
		p->PrintChat();
	}
	return 0;
}


void Client::Send()
{
	

	send(_socket, buff_client, strlen(buff_client), 0);
	PrintChat();

}


void Client::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUTTON3)
	{
		TCHAR buff[256];
		GetWindowText(hIp, buff, 256);
		char buff2[256];
		size_t outputSize = wcslen(buff)+ 1;
		wcstombs(buff2, buff,256);
		
		
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		//создаем потоковый сокет, протокол TCP
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		//—емейство адресов IPv4
		addr.sin_family = AF_INET;
		/*ѕреобразуем адрес в правильную
		структуру хранени€ адресов, результат сохран€ем в поле sin_addr */
		inet_pton(AF_INET, buff2, &addr.sin_addr);
		//”казываем порт. 
		//‘ункци€ htons выполн€ет преобразование числа в
		//сетевой пор€док байт, т.к. стандартным сетевым 
		//€вл€етс€ пор€док от старшего к младшему
		addr.sin_port = htons(20000);
		//выполн€ем подключение к серверу, указыва€
		//дескриптор сокета, адрес и размер структуры
		connect(_socket, (SOCKADDR*)&addr, sizeof(addr));
		CreateThread(NULL, 0, GetM, this, 0, NULL);

	}
	else if (id == IDC_BUTTON2)
	{
		TCHAR buff[256];
		GetWindowText(hMessage, buff, 256);
	
		
		wcstombs(buff_client, buff, 256);

		Send();
	}
}

BOOL CALLBACK Client::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}