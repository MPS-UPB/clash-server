#pragma once

#include<iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
/*
#ifdef UTIL_EXPORTS
#define UTIL_API __declspec(dllexport)
#else
#define UTIL_API __declspec(dllimport)
#endif
*/
void mySleep(int sleepMs);

void assert(int err, char* msg = "");

void assert(void* err, char* msg = "");

WCHAR* toWchar(char *text, WCHAR *buffer, int len_buffer);

char* toChar(WCHAR *text, char *buffer, int len_buffer);

