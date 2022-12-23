#pragma comment (lib, "Ws2_32.lib")
#include <Winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "urlmon.lib")
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
string Normal;

string buff[9] = { "\"id\"" ,"\"name\"","\"country\"","\"lon\"","\"lat\"","\"temp_min\"","\"temp_max\"","\"sunrise\"","\"sunset\"" };

void MakeNormal(string n) {

    int Index;
    for (int i = 0; i < 9; i++)
    {
        if (i == 5 || i == 6) {

        }


        Index = n.find(buff[i]);
        if (Index == -1) {
            Normal += buff[i];
            Normal += ": No info found";
        }
        else
        {
            int z = Index;
            while (n[z] != ',')
            {
                Normal += n[z];
                z++;
            }
            Normal += "\r\n";
        }
    }


}
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //1. инициализация "Ws2_32.dll" для текущего процесса
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);

    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {

        cout << "WSAStartup failed with error: " << err << endl;
        return 1;
    }

    //инициализация структуры, для указания ip адреса и порта сервера с которым мы хотим соединиться

    char hostname[255] = "api.openweathermap.org";

    addrinfo* result = NULL;

    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int iResult = getaddrinfo(hostname, "http", &hints, &result);
    if (iResult != 0) {
        cout << "getaddrinfo failed with error: " << iResult << endl;
        WSACleanup();
        return 3;
    }


    SOCKET connectSocket = INVALID_SOCKET;
    addrinfo* ptr = NULL;

    //Пробуем присоединиться к полученному адресу
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        //2. создание клиентского сокета
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        //3. Соединяемся с сервером
        iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    //4. HTTP Request
    string City = "Odessa";
    cin >> City;
    string uri = "/data/2.5/weather?q=";
    string uri2 = "&appid=75f6e64d49db78658d09cb5ab201e483&mode=json&units=metric&lang=de";
    uri += City;
    uri += uri2;
   
    //// URLDownloadToFile returns S_OK on success 

    string request = "GET " + uri + " HTTP/1.1\n";
    request += "Host: " + string(hostname) + "\n";
    request += "Accept: */*\n";
    request += "Accept-Encoding: gzip, deflate, br\n";
    request += "Connection: close\n";
    request += "\n";

    //отправка сообщения
    if (send(connectSocket, request.c_str(), request.length(), 0) == SOCKET_ERROR) {
        cout << "send failed: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 5;
    }
    cout << "send data" << endl;

    //5. HTTP Response

    string response;

    const size_t BUFFERSIZE = 1024;
    char resBuf[BUFFERSIZE];

    int respLength;

    do {
        respLength = recv(connectSocket, resBuf, BUFFERSIZE, 0);
        if (respLength > 0) {
            response += string(resBuf).substr(0, respLength);
        }
        else {
            cout << "recv failed: " << WSAGetLastError() << endl;
            closesocket(connectSocket);
            WSACleanup();
            return 6;
        }

    } while (respLength == BUFFERSIZE);
    MakeNormal(response);
    cout << Normal << endl << endl;
    cout << response << endl;

    ofstream in;
    in.open("file.txt", ios::app);
    in <<Normal;

    //отключает отправку и получение сообщений сокетом
    iResult = shutdown(connectSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown failed: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 7;
    }

    closesocket(connectSocket);
    WSACleanup();
  
       
}