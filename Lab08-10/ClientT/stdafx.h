#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib") 
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <tchar.h>
#include "targetver.h"
using namespace std;
using namespace chrono;

string SetErrorMsgText(string msgText, int code);