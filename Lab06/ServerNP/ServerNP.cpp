#include "stdafx.h"

#define PIPE_NAME L"\\\\.\\pipe\\Tube"

int main()
{
	setlocale(LC_ALL, "Rus");
	cout << "ServerNP" << endl;

	HANDLE hPipe; // дескриптор канала
	DWORD ps;

	try
	{
		if ((hPipe = CreateNamedPipe(PIPE_NAME,
			PIPE_ACCESS_DUPLEX, //дуплексный канал
			PIPE_TYPE_MESSAGE | PIPE_WAIT, // сообщения|синхронный
			1, NULL, NULL, // максимум 1 экземпляр
			INFINITE, NULL)) == INVALID_HANDLE_VALUE)
			throw SetPipeError("create:", GetLastError());

		if (!ConnectNamedPipe(hPipe, NULL)) // ожидать клиента 
			throw SetPipeError("connect:", GetLastError());
		char buf[50];
		while (ReadFile(hPipe, buf, sizeof(buf), &ps, NULL))
		{
			cout << buf << endl;

			if (!WriteFile(hPipe, buf, sizeof(buf), &ps, NULL))
				throw SetPipeError("WriteFile: ", GetLastError());
		}



		if (!DisconnectNamedPipe(hPipe))
			throw SetPipeError("Disconnect: ", GetLastError());
		if (!CloseHandle(hPipe))
			throw SetPipeError("Close: ", GetLastError());

	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}

}

