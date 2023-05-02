#pragma comment(lib, "WS2_32.lib") 
#pragma warning(disable: 4996)
#include "Winsock2.h"
#include "stdafx.h"

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;
int countServers = 0;

bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen);
bool PutAnswerToClient(char* name, short port, struct sockaddr* to, int* lto);
void GetServer(char* call, short port, struct sockaddr* from, int* flen);



int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	SOCKET sS;
	WSADATA wsaData;

	SOCKADDR_IN client;
	int clientSize = sizeof(client);
	char name[] = "Hello";

	try	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());

		cout << "SERVER #3" << endl;
		cout << "Checking for other servers..." << endl;
		GetServer(name, 2000, (sockaddr*)&client, &clientSize);


		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());


		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = inet_addr("127.0.0.4");

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw  SetErrorMsgText("bind:", WSAGetLastError());

		SOCKADDR_IN clientB;
		int clientSize = sizeof(clientB);

		while (true)
		{
			if (GetRequestFromClient(name, sS, (SOCKADDR*)&clientB, &clientSize))
			{
				cout << endl;
				cout << "Client socket:" << endl;
				cout << "IP: " << inet_ntoa(clientB.sin_addr) << endl;
				cout << "Port: " << htons(clientB.sin_port) << endl;
				cout << endl;

				if (PutAnswerToClient(name, sS, (SOCKADDR*)&clientB, &clientSize))
				{
					cout << "Success!" << endl;
				}
			}
			else
			{
				cout << "Wrong call name!" << endl;
			}
		}

		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());

	}
	catch (string errorMsgText)
	{
		std::cout << endl << "WSAGetLastError: " << errorMsgText;
	}

}


bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen)
{
	char buf[50] = "";
	int lenght;
	cout << "\nWait message..." << endl;

	while (true)
	{
		if (lenght = recvfrom(port, buf, sizeof(buf), NULL, from, flen) == SOCKET_ERROR)
		{
			cout << "Error:" << endl;
			if (WSAGetLastError() == WSAETIMEDOUT)
				return false;
			else
				throw SetErrorMsgText("Recvfrom: ", WSAGetLastError());
		}
		return strcmp(buf, name) == 0;
	}
}

bool PutAnswerToClient(char* name, short port, struct sockaddr* to, int* lto)
{
	int lenghts = strlen(name);
	return sendto(port, name, lenghts + 1, NULL, to, *lto);
}

void GetServer(char* call, short port, struct sockaddr* from, int* flen)
{
	SOCKET cC;
	SOCKADDR_IN all;

	int timeout = 5000;

	int optval = 1;
	char buf[50];

	try
	{
		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());


		if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
			throw  SetErrorMsgText("setsocketopt:", WSAGetLastError());
		if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)) == SOCKET_ERROR)
			throw  SetErrorMsgText("setsocketopt:", WSAGetLastError());

		all.sin_family = AF_INET;
		all.sin_port = htons(port);
		all.sin_addr.s_addr = inet_addr("127.255.255.255");

		if (sendto(cC, call, strlen(call) + 1, NULL, (sockaddr*)&all, sizeof(all)) == SOCKET_ERROR)
			throw SetErrorMsgText("sendto:", WSAGetLastError());

		while (true)
		{
			if (recvfrom(cC, buf, sizeof(buf), NULL, from, flen) == SOCKET_ERROR)
				throw  SetErrorMsgText("recvfrom:", WSAGetLastError());

			if (strcmp(call, buf) == 0)
			{
				countServers++;
				cout << "There's a server with the same callsign!" << endl;
				cout << "IP: " << inet_ntoa(((SOCKADDR_IN*)from)->sin_addr) << endl;
				cout << "Port: " << ntohs(((struct sockaddr_in*)from)->sin_port) << endl;
			}
		}
	}
	catch (string errorMsgText)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
		{
			cout << "Number of servers with the same callsign: " << countServers << endl;
			if (closesocket(cC) == SOCKET_ERROR)
				throw SetErrorMsgText("closesocket: ", WSAGetLastError());
		}
		else throw SetErrorMsgText("GetServer:", WSAGetLastError());
	}
}


