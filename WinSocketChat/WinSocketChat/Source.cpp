//Main.cpp
#include <iostream>
#include "Socket.h"
#include <string>
using namespace std;

int main()
{

	int Phone = 7800;
	int Laptop = 3200;
	int TV = 3800;

	char buff2[3][256] = { "Phone","Laptop","TV" };

	int nChoice;
	int port = 24242; //выбираем порт
	string ipAddress; //Адрес сервера
	char receiveMessage[MAXSTRLEN];
	char sendMessage[MAXSTRLEN];
	cout << "1) Start server" << endl;
	cout << "2) Connect to server" << endl;
	cout << "3) Exit" << endl;
	cin >> nChoice;
	if (nChoice == 1)
	{
		ServerSocket server;
		cout << "Starting server..." << endl;
		//Запускаем сервер
		server.StartHosting(port);
		while (true)
		{
			cout << "\tWaiting..." << endl;
			//Получаем данные от клиента
			//и сохраняем в переменной receiveMessage
			server.ReceiveData(receiveMessage, MAXSTRLEN);
			cout << "Received: " << receiveMessage << endl;
			//Отправляем данные клиенту
			if (strcmp(receiveMessage, "1")==0 ){
				char buff[256];

				sprintf(buff, "Phone %d----Type how much you want",Phone);
				
				server.SendDataResult(buff, MAXSTRLEN);
				server.ReceiveData(receiveMessage, MAXSTRLEN);

				char buff3[256];
				int buf;
				string bf;
				bf = receiveMessage;

				buf = stoi(bf);
				buf *= Phone;
				sprintf(buff, "Phone %d", buf);
				server.SendDataResult(buff, MAXSTRLEN);
			}
			if (strcmp(receiveMessage, "2") == 0) {
				char buff[256];

				sprintf(buff, "Laptop %d ----Type how much you want", Laptop);

				server.SendDataResult(buff, MAXSTRLEN);
				server.ReceiveData(receiveMessage, MAXSTRLEN);

				char buff3[256];
				int buf;
				string bf;
				bf = receiveMessage;

				buf = stoi(bf);
				buf *= Laptop;
				sprintf(buff, "Laptop %d", buf);
				server.SendDataResult(buff, MAXSTRLEN);
			}
			if (strcmp(receiveMessage, "3") == 0) {
				char buff[256];

				sprintf(buff, "TV %d----Type how much you want", TV);

				server.SendDataResult(buff, MAXSTRLEN);
				server.ReceiveData(receiveMessage, MAXSTRLEN);
				
				char buff3[256];
				int buf;
				string bf;
				bf = receiveMessage;

				buf = stoi(bf);
				buf *= TV;
				sprintf(buff, "TV %d - you need", buf);
				server.SendDataResult(buff, MAXSTRLEN);
			}

			//Если есть сообщение "end", завершаем работу
			if (strcmp(receiveMessage, "end") == 0 ||
				strcmp(sendMessage, "end") == 0)
				break;
		}
	}
	else if (nChoice == 2)
	{
		cout << "Enter an IP address: " << endl;
		//Запрашиваем IP сервера
		cin >> ipAddress;
		ClientSocket client;
		//подключаемся к серверу
		client.ConnectToServer(ipAddress.c_str(), port);
		cout << "\tType what you want\n 1-Telephon 7800\n 2-Laptop 3200\n 3-TV 3900" << endl;
		while (true)
		{
			//отправляем сообщение
			
			
			//получаем ответ
			client.SendDataMessage();
			client.ReceiveData(receiveMessage, MAXSTRLEN);
			cout << "Received: " << receiveMessage << endl;
			if (strcmp(receiveMessage, "end") == 0 ||
				strcmp(sendMessage, "end") == 0)
				break;
		}
		//Закрываем соединение
		client.CloseConnection();
	}
	else if (nChoice == 3)
		return 0;
}










