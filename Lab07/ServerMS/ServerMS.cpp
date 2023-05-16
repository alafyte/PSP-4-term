#include "stdafx.h"

#define NAME L"\\\\.\\mailslot\\Box"

int main()
{
    setlocale(LC_ALL, "rus");
    cout << "Mailslot Server" << endl;

    HANDLE hM; // дескриптор почтового ящика 
    DWORD rb; // длина почитанного сообщения
    char rbuf[300]; // буфер ввода
    DWORD time_waiting = 180000; // 3 минуты
    bool flag = true;
    clock_t start, end;

    try
    {
        if ((hM = CreateMailslot(NAME,
            300,
            MAILSLOT_WAIT_FOREVER, // ждать вечно
            NULL)) == INVALID_HANDLE_VALUE)
            throw SetErrorMsgText("CreateMailslotError", WSAGetLastError());

        while (true)
        {

            if (ReadFile(hM,
                rbuf, // буфер
                sizeof(rbuf), // размер буфера
                &rb, // прочитано
                NULL))
            {
                cout << rbuf << endl;
                if (flag)
                {
                    start = clock();
                    flag = false;
                }
            }
            else
                throw SetErrorMsgText("ReadFileError", WSAGetLastError());

            if (strcmp(rbuf, "STOP") == 0)
            {
                end = clock();
                cout << "\nВремя обмена: " << ((double)(end - start) / CLK_TCK) << " секунд" << endl;
                flag = true;
            }
        }

        CloseHandle(hM);
    }
    catch (string error)
    {
        cout << endl << error << endl;
    }
}

