#include "stdafx.h"

#define NAME L"\\\\.\\mailslot\\Box"

int main()
{
	setlocale(LC_ALL, "rus");
	cout << "Mailslot Client" << endl;

	HANDLE hM; // дескриптор почтового ящика 
	DWORD wb; // длина записанного сообщения
	char wbuf[] = "Hello from Maislot-client"; // буфер вывода 
	try
	{
		if ((hM = CreateFile(NAME,
			GENERIC_WRITE, // будем писать в ящик
			FILE_SHARE_READ, // разрешаем одновременно читать 
			NULL,
			OPEN_EXISTING, // ящик уже есть
			NULL, NULL)) == INVALID_HANDLE_VALUE)
			throw SetErrorMsgText("CreateFileError", WSAGetLastError());

		for (int i = 0; i < 100; i++)
			if (!WriteFile(hM,
				wbuf, // буфер
				sizeof(wbuf), // размер буфера
				&wb, // записано
				NULL))
				throw SetErrorMsgText("ReadFileError", WSAGetLastError());

		strcpy_s(wbuf,  "STOP");
		if (!WriteFile(hM, wbuf, sizeof(wbuf), &wb, NULL))
			throw SetErrorMsgText("ReadFileError", WSAGetLastError());
		
	}
	catch(string error)
	{ 
		cout << endl << error << endl;
	}
}
