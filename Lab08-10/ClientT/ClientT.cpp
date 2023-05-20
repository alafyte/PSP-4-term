#include "stdafx.h"

#define IP_SERVER "127.0.0.1"


char* get_message(int msg)
{
    switch (msg)
    {
    case 1: return (char*)"Echo";
    case 2: return (char*)"Time";
    case 3: return (char*)"Random";
    case 4: return (char*)"close";
    default:
        return (char*)"close";
    }
}

int _tmain(int argc, char* argv[])
{
    char* error = (char*)"close";
    int count = 0;
    SOCKET cS;
    WSADATA wsaData;
    setlocale(0, "rus");
    try
    {

        if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        if ((cS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());

        SOCKET cC;
        if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = inet_addr(IP_SERVER);
        if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
            throw SetErrorMsgText("connect:", WSAGetLastError());


        while (true)
        {
            char message[50];
            int libuf = 0,
                lobuf = 0;

            puts("Choose");
            int service;
            puts("1 - Echo\n2 - Time\n3 - Random\n4 - close socket");

            scanf("%d", &service);
            char* outMessage = new char[5];
            strcpy(outMessage, get_message(service));

            if ((lobuf = send(cC, outMessage, strlen(outMessage) + 1, NULL)) == SOCKET_ERROR)
                throw SetErrorMsgText("send:", WSAGetLastError());

            printf("sended: %s\n", outMessage);

            if ((libuf = recv(cC, message, sizeof(message), NULL)) == SOCKET_ERROR)
                throw SetErrorMsgText("recv:", WSAGetLastError());

            if (service != 1 && service != 2 && service != 3 && service != 4)
            {
                error = (char*)"ErrorInQuery";
                break;
            }
            if (service == 4)
            {
                break;
            }

            if (!strcmp(message, "TimeOUT"))
            {
                puts("time out");
                return -1;
            }
            if (service == 1)
            {
                for (int j = 30; j >= 0; --j)
                {
                    Sleep(1000);
                    sprintf(outMessage, "%d", j);
                    if ((lobuf = send(cC, outMessage, strlen(outMessage) + 1, NULL)) == SOCKET_ERROR)
                        throw SetErrorMsgText("send:", WSAGetLastError());
                    printf("send: %s\n", outMessage);
                    if ((libuf = recv(cC, message, sizeof(message), NULL)) == SOCKET_ERROR)
                        throw SetErrorMsgText("recv:", WSAGetLastError());
                    printf("receive: %s\n", message);
                }
            }
            else if (service == 2 || service == 3)
            {
                printf("receive: %s\n", message);
            }
        }

        if (closesocket(cS) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        printf("\n%s\n", errorMsgText.c_str());
    }
    printf("%s\n", error);
    system("pause");
    return 0;
}
