#pragma once
#include "header.h"

class Client
{
public:
	Client(void);
public:
	~Client(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Client* ptr;
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void PrintChat();
	
	void Send();
	HWND hConnect, hDisconnect, hSend;
	HWND hChat, hMessage;
	HWND hIp;

	

	char buff_client[512];


	const int MAXSTRLEN = 255;
	WSADATA wsaData;//структура для хранения информацию
					//о инициализации сокетов
	SOCKET _socket; //дескриптор сокета
	sockaddr_in addr; //локальный адрес и порт


};
