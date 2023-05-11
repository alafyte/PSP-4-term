#include "stdafx.h"

#define PIPE_NAME L"\\\\.\\pipe\\Tube"

int main()
{
    setlocale(LC_ALL, "Rus");
    cout << "ClientNPct" << endl;

    DWORD ps;

    try
    {
        char obuf[50] = "Hello from ClientNPct!";
        char ibuf[50];


        if (CallNamedPipe(PIPE_NAME,
            obuf, //буфер для отправки (записи)
            sizeof(obuf), // размер входного буфера
            ibuf, //буфер для получения (чтения)
            sizeof(ibuf), //размер выходного буфера
            &ps, //количество прочитанных байт
            NULL))  // параметр для асинхронного доступа
            cout << ibuf << endl;
        else
            throw SetPipeError("CallNamedPipe: ", GetLastError());

    }
    catch (string ErrorPipeText)
    {
        cout << endl << ErrorPipeText;
    }

}
