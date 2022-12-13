#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <queue>
#include <string>
#include <iostream>
using namespace std;


WSADATA wsadata;

string food[3] = { "humburger", "potato", "sprite" };
queue<string> que;
SOCKET udpSocket;

sockaddr_in addr;

const size_t receiveBufSize = 1024;
char receiveBuf[receiveBufSize];



sockaddr_in senderAddr;
vector<sockaddr_in> sendAddr;
int senderAddrSize = sizeof(senderAddr);
int Kol = 0;


int q = 0;
int bytesReceived;
DWORD WINAPI Que(LPVOID lp)
{
    while (true) {
        bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);
        Kol++;
        char buff2[256];
        sendAddr.push_back(senderAddr);
        sprintf_s(buff2, "You are in %d\0", Kol);
        int sendResult = sendto(udpSocket, buff2, strlen(buff2), 0, (SOCKADDR*)&sendAddr[Kol-1], senderAddrSize);
      
    }

}

int main()
{
    
    
    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

     udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

  
    addr.sin_family = AF_INET;    
    addr.sin_port = htons(23000);   
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }


    CreateThread(NULL, NULL, Que, NULL, NULL, NULL);
    while (true) {
        if (Kol > 0&&Kol<30) {
            cout << "Receiving data...." << endl;

           
           
            int times = 0;
            int time[3] = { 5,3,1 };




            if (bytesReceived == SOCKET_ERROR)
            {
                cout << "recvfrom failed with error " << WSAGetLastError() << endl;
                return 4;
            }

            receiveBuf[bytesReceived] = '\0';
            cout << "Received from " << senderAddr.sin_addr.s_host << endl;
            cout << "Data: " << receiveBuf << endl;
            que.push(receiveBuf);
            size_t found = 0;
            for (int i = 0; i < 3; i++)
            {
                found = que.front().find(food[i]);
                if (found > 0)
                {
                    times += time[i];
                }
            }
            const size_t sendBufSize = 1024;
            cout << "Waiting..." << times << "Sec\n";

            for (int i = times; i >= 0; i--)
            {
                char sendBuf[sendBufSize];
                sprintf_s(sendBuf, "Waiting for %d left sec%d \0", times, i);

                int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&sendAddr[q], senderAddrSize);
                
                if (sendResult == SOCKET_ERROR)
                {
                    cout << "sendto failed with error " << WSAGetLastError() << endl;
                    return 4;
                }

                Sleep(1000);



            }
            char buff[256];
            Kol--;
            sprintf_s(buff, "Your order is ready %d People in Que\0", Kol);
            
            int sendResult = sendto(udpSocket, buff, strlen(buff), 0, (SOCKADDR*)&sendAddr[q], senderAddrSize);

            q++;


        }
    }
    closesocket(udpSocket);
    WSACleanup();
}

