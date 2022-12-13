#include "Server.h"

Server* Server::ptr = NULL;


Server::Server(void)
{
	ptr = this;
}

Server::~Server(void)
{
	closesocket(acceptSocket);
	closesocket(_socket);
	//Освобождаем ресурсы
	WSACleanup();
}

void Server::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL Server::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hChat = GetDlgItem(hwnd, IDC_LIST1);
	hStart = GetDlgItem(hwnd, IDC_BUTTON2);
	hMessage = GetDlgItem(hwnd, IDC_EDIT2);
	return TRUE;
}


void Server::Print_chat()
{
	TCHAR buff[258];
	
	mbstowcs(buff, buff_Server, 256);

	
	SendMessage(hChat, LB_ADDSTRING, 0, LPARAM(buff));

}
void Server::Send()
{

	
	send(acceptSocket, buff_Server, strlen(buff_Server), 0);

	Print_chat();
}

DWORD WINAPI GetM(LPVOID lp)
{
	Server* p = (Server*)lp;
	while (true) {
		
		int i = recv(p->acceptSocket, p->buff_Server, p->MAXSTRLEN, 0);
		p->buff_Server[i ] = '-';
		p->buff_Server[i + 1] = 'C';
		p->buff_Server[i+2] = '\0';
		
		p->Print_chat();
	}
	return 0;
}

//void Server::Recive()
//{
//
//	
//
//}


void Server::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUTTON2)
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		//создаем потоковый сокет, протокол TCP
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		//Семейство адресов IPv4
		addr.sin_family = AF_INET;
		/*Преобразуем адрес в правильную
		структуру хранения адресов, результат сохраняем в поле sin_addr */
		inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
		//Указываем порт. 
		//Функиця htons выполняет преобразование числа в
		//сетевой порядок байт, т.к. стандартным сетевым 
		//является порядок от старшего к младшему
		addr.sin_port = htons(20000);
		//биндим сокет к указанному адресу и порту
		bind(_socket, (SOCKADDR*)&addr, sizeof(addr));
		//открываем сокет в режиме прослушивания входящих подключений
		listen(_socket, 1);
		
	
		EnableWindow(hStart, FALSE);
		//ожидаем подключения клиента.
		//в случае успешного подключения - получаем новый сокет
		//через который будет происходить обмен сообщениями
		//с клиентом.
		//Функция является блокирующей.
		acceptSocket = accept(_socket, NULL, NULL);
		CreateThread(NULL, 0, GetM, this, 0, NULL);
		//Recive();
	}
	else if (id == IDC_BUTTON1)
	{
		TCHAR buff[256];
		GetWindowText(hMessage, buff, 256);

		size_t outputSize = wcslen(buff) + 1;
		wcstombs(buff_Server, buff, 256);
		
		Send();
	}
}

BOOL CALLBACK Server::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}