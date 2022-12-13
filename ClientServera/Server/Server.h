#pragma once
#include "header.h"

class Server
{
public:
	Server(void);
public:
	~Server(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Server* ptr;
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Recive();
	void Send();
	void Print_chat();

	HWND hStart, hStop, hSend;
	HWND hChat,hMessage;

	char buff_Server[256];

	const int MAXSTRLEN = 255;
	WSADATA wsaData;//структура для хранения информацию
						//о инициализации сокетов
	SOCKET _socket; //дескриптор слушающего сокета
	SOCKET acceptSocket;//дескриптор сокета, который связан с клиентом 
	sockaddr_in addr; //локальный адрес и порт


};
