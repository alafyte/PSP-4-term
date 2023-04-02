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

    SOCKET cC; // дескриптор сокета
    WSADATA wsaData;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());

        SOCKADDR_IN serv; // параметры сокета sS
        serv.sin_family = AF_INET; // используется IP-адресация
        serv.sin_port = htons(2000); // порт 2000
        serv.sin_addr.s_addr = INADDR_ANY;

        if (bind(cC, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind:", WSAGetLastError());

        SOCKADDR_IN clnt; // параметры сокета клиента
        memset(&clnt, 0, sizeof(clnt)); // обнулить память
        int lc = sizeof(clnt);
        int lclnt = sizeof(clnt); // размер SOCKADDR_IN
        char ibuf[50];
        int lb = 0, lobuf = 0;
        clock_t start, end;
        bool flag = true;

        while (true)
        {

            if ((lb = recvfrom(cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lc)) == SOCKET_ERROR)
            {
                throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
            }
            if (flag)
            {
                flag = false;
                start = clock();
            }

            if ((lobuf = sendto(cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, lc)) == SOCKET_ERROR)
            {
                throw SetErrorMsgText("sendto: ", WSAGetLastError());
            }

            if (strcmp(ibuf, "") == 0)
            {
                end = clock();
                flag = true;
                cout << "\nВремя обмена: " << ((double)(end - start) / CLK_TCK) << " c\n\n";
            }
            else
                cout << ibuf << endl;
        }

        if (closesocket(cC) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << endl << errorMsgText;
    }

    system("pause");
    return 0;
}

