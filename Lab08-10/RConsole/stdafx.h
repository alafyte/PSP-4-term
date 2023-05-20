#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <tchar.h>
#include "targetver.h"
using namespace std;
#define _itoa_s insted
#pragma warning(disable : 4996)

string SetPipeError(string msgText, int code);
