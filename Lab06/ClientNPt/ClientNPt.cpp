#include "stdafx.h"

#define PIPE_NAME L"\\\\.\\pipe\\Tube"

int main()
{
    setlocale(LC_ALL, "Rus");
    cout << "ClientNPt" << endl;

    HANDLE hPipe; // дескриптор канала
    DWORD ps;
    DWORD state = PIPE_READMODE_MESSAGE;

    try
    {
        if ((hPipe = CreateFile(
            PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE, //чтение и запись
            FILE_SHARE_READ | FILE_SHARE_WRITE, //совместное чтение и запись
            NULL, //атрибут безопасности
            OPEN_EXISTING, //параметр открытия канала
            NULL, //флаги
            NULL)) == INVALID_HANDLE_VALUE)
            throw SetPipeError("createfile:", GetLastError());

        if (!SetNamedPipeHandleState(hPipe, &state, NULL, NULL)) // функция предназначена для изменения динамических характеристик именованного канала
        {
            throw SetPipeError("SetNamedPipeHandleState: ", GetLastError());
        }

        int message_amount = 100;
        char obuf[50];
        char ibuf[50];

        for (int i = 1; i <= message_amount; i++)
        {
            string obufstr = "Hello from ClientNPt " + to_string(i);
            strcpy_s(obuf, obufstr.c_str());

            if(TransactNamedPipe(hPipe, 
                obuf, //буфер для отправки (записи)
                sizeof(obuf), // размер входного буфера
                ibuf, //буфер для получения (чтения)
                sizeof(ibuf), //размер выходного буфера
                &ps, //количество прочитанных байт
                NULL))  // параметр для асинхронного доступа
                cout << ibuf << endl;
            else
                throw SetPipeError("TransactNamedPipe: ", GetLastError());
        }

        CloseHandle(hPipe);
    }
    catch (string ErrorPipeText)
    {
        cout << endl << ErrorPipeText;
    }

}
