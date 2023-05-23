#include "stdafx.h"

#define NAME L"\\\\.\\mailslot\\Box"
//#define NAME L"\\\\*\\mailslot\\Box"
//Если при рассылке на несколько компьютеров на сервере дублируются сообщения, 
//отключите неиспользуемые адаптеры беспроводной сети в настройках Network & Internet

int main()
{
	setlocale(LC_ALL, "rus");
	cout << "Mailslot Client" << endl;

	HANDLE hM; // дескриптор почтового ящика 
	DWORD wb; // длина записанного сообщения
	char obuf[50];
	try
	{
		if ((hM = CreateFile(NAME,
			GENERIC_WRITE, // будем писать в ящик
			FILE_SHARE_READ, // разрешаем одновременно читать 
			NULL,
			OPEN_EXISTING, // ящик уже есть
			NULL, NULL)) == INVALID_HANDLE_VALUE)
			throw SetErrorMsgText("CreateFileError", WSAGetLastError());

		for (int i = 0; i < 30; i++)
		{
			string obufstr = "Hello from Client " + to_string(i + 1);
			strcpy_s(obuf, obufstr.c_str());
			if (!WriteFile(hM,
				obuf, // буфер
				sizeof(obuf), // размер буфера
				&wb, // записано
				NULL))
				throw SetErrorMsgText("ReadFileError", WSAGetLastError());
		}

		strcpy_s(obuf,  "STOP");
		if (!WriteFile(hM, obuf, sizeof(obuf), &wb, NULL))
			throw SetErrorMsgText("ReadFileError", WSAGetLastError());
		
	}
	catch(string error)
	{ 
		cout << endl << error << endl;
	}
}
