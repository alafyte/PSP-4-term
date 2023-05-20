#include "stdafx.h"

char* get_command_by_id(int id)
{
	switch (id)
	{
	case 1: return (char*)"start";
	case 2: return (char*)"stop";
	case 3: return (char*)"wait";
	case 4: return (char*)"statistics";
	case 5: return (char*)"exit";
	case 6: return (char*)"shutdown";
	default: return (char*)"error";
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "");
	char rbuf[200];

	DWORD dwRead;
	DWORD dwWrite;
	HANDLE hPipe;
	try
	{
		if ((hPipe = CreateFile(
			L"\\\\.\\pipe\\Tube",
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, NULL,
			NULL)) == INVALID_HANDLE_VALUE)
			throw  SetPipeError("createfile:", GetLastError());
	}
	catch (string ErrorPipeText)
	{
		printf("\n%s\n", ErrorPipeText.c_str());
		return -1;
	}
	char wbuf[40] = "start";
	char ebuf[40] = "error";
	while (true) 
	{
		int serverSendCommand = 0;
		puts("Input some server command\n" \
			"1 - start\n" \
			"2 - stop\n" \
			"3 - wait\n" \
			"4 - statistic\n" \
			"5 - exit\n" \
			"6 - shutdown\n");
		scanf("%d", &serverSendCommand);
		strcpy(wbuf, get_command_by_id(serverSendCommand));
		system("cls");

		WriteFile(hPipe, wbuf, sizeof(wbuf), &dwWrite, NULL);

		printf("send:  %s\n", wbuf);
		if (serverSendCommand == 6)
			break;
		ReadFile(hPipe, rbuf, sizeof(rbuf), &dwRead, NULL);
		if (!strcmp(wbuf, "error")) 
		{
			printf("send:  nocmd");
		}
		else
			printf("get:  %s\n", rbuf);
	}
	CloseHandle(hPipe);
	system("pause");
	return 0;
}