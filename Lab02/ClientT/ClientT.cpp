#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <clocale>
#include <ctime>


#include "stdafx.h"
#include "Winsock2.h"                
#pragma comment(lib, "WS2_32.lib")   

int main()
{
    setlocale(LC_ALL, "rus");

    SOCKET sS;
    WSADATA wsaData;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");

        if ((connect(sS, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
            throw SetErrorMsgText("connect:", WSAGetLastError());

        char ibuf[50] = "server: принято ";
        int  libuf = 0, lobuf = 0;

        int message_amount = 100;

        const clock_t start = clock();
        for (int i = 1; i <= message_amount; i++) 
        {
            string obuf = "Hello from Client " + to_string(i);

            if ((lobuf = send(sS, obuf.c_str(), strlen(obuf.c_str()) + 1, NULL)) == SOCKET_ERROR) 
                throw SetErrorMsgText("send: ", WSAGetLastError());

            if ((libuf = recv(sS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR) 
                throw SetErrorMsgText("recv: ", WSAGetLastError());

            cout << ibuf << '\n';
        }
        const clock_t end = clock();
        const string obuf = "";

        if ((lobuf = send(sS, obuf.c_str(), strlen(obuf.c_str()) + 1, NULL)) == SOCKET_ERROR) 
            throw SetErrorMsgText("send: ", WSAGetLastError());

        cout << "Время обмена: " << (static_cast<double>(end - start) / CLK_TCK) << " c\n";

        if (closesocket(sS) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << endl << errorMsgText;
    }

    cout << endl;
    system("pause");
    return 0;
}