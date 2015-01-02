#pragma once

#include<iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void mySleep(int sleepMs);

void assert(int err, char* msg = "");

void assert(void* err, char* msg = "");

WCHAR* toWchar(const char *text, WCHAR *buffer, int len_buffer);

char* toChar(WCHAR *text, char *buffer, int len_buffer);

std::string toString(int nr);

std::string getMessageElement(std::string message, int index, bool withDelimiter=true);

PROCESS_INFORMATION runProcess(const char *AppName_c, const char *CmdLine_c);

void killProcess(int pid);