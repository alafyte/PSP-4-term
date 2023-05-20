#include "stdafx.h"

SOCKET  cS;

bool  GetServer(
	char* call, //[in] позывной сервера  
	short            port, //[in] номер порта сервера    
	SOCKADDR_IN* from, //[out] указатель на SOCKADDR_IN
	int* flen  //[out] указатель на размер from 
)
{

	SOCKADDR_IN server;               // параметры  сокета клиента
	memset(&server, 0, sizeof(server));   // обнулить память
	char message[50];
	int lc = sizeof(server);


	int  lb = 0;
	if ((lb = recvfrom(cS, message, sizeof(message), NULL, (sockaddr*)&server, &lc)) == SOCKET_ERROR)
		throw  SetErrorMsgText("recfrom:", WSAGetLastError());

	if (GetLastError() == WSAETIMEDOUT) return false;

	cout << "Ответ: " << message << endl;

	*from = server;
	*flen = lc;


	return true;
}

int main(int argc, char* argv[])
{
	setlocale(0, "rus");
	// дескриптор сокета 
	WSADATA wsaData;
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());

		if ((cS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		int optval = 1;

		if (setsockopt(cS, SOL_SOCKET, SO_BROADCAST,
			(char*)&optval, sizeof(int)) == SOCKET_ERROR)
			throw  SetErrorMsgText("opt:", WSAGetLastError());



		SOCKADDR_IN all;                        // параметры  сокета sS
		all.sin_family = AF_INET;               // используетс¤ IP-адресаци 
		all.sin_port = htons(2000);             // порт 2000
		all.sin_addr.s_addr = INADDR_BROADCAST; // всем  
		char buf[] = "Hello";

		if ((sendto(cS, buf, sizeof(buf), NULL,
			(sockaddr*)&all, sizeof(all))) == SOCKET_ERROR)
			throw  SetErrorMsgText("sendto:", WSAGetLastError());

		SOCKADDR_IN server;

		int lc = sizeof(server);

		GetServer((char*)"Hello", 2000, &server, &lc);

		cout << "\nПараметры сервера: " << endl;
		cout << "port: " << htons(server.sin_port) << endl;
		cout << "adress: " << inet_ntoa(server.sin_addr) << endl;


		if (closesocket(cS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());


		system("pause");
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText << endl;
	}
	return 0;
}
