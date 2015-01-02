#include "util.h"
#include <tchar.h>

void mySleep(int sleepMs)
{
	Sleep(sleepMs);
}

void assert(int err, char* msg)
{
	if(err==0)
		std::cout<<"Eroare!"<<msg<<errno;
}

void assert(void* err, char* msg)
{
	if(err==NULL)
		std::cout<<"Eroare!"<<msg;
}

WCHAR* toWchar(const char *text, WCHAR *buffer, int len_buffer)
{
	MultiByteToWideChar(0, 0, text, -1, buffer, len_buffer);

	return buffer;
}

char* toChar(WCHAR *text, char *buffer, int len_buffer)
{
	size_t ret;
	wcstombs_s(&ret, buffer, len_buffer, text, len_buffer);
	if (ret == len_buffer) buffer[len_buffer - 1] = '\0';

	return buffer;
}

std::string getMessageElement(std::string message, int index, bool withDelimiter)
{
	size_t iterator = 0;
	int elements = 0;

	do
	{
		iterator = message.find(':', iterator);
		if (iterator == std::string::npos)
		{
			if (withDelimiter)
				return std::string(":");
			else return std::string("");
		}
		elements++;
	} while (elements <= index);

	if (withDelimiter)
		if (iterator<message.length())
			iterator++;
	
	return message.substr(0, iterator);

}

PROCESS_INFORMATION runProcess(const char *AppName_c, const char *CmdLine_c)
{

	WCHAR AppName[1024], CmdLine[1024];

	toWchar(AppName_c, AppName, 1024);
	toWchar(CmdLine_c, CmdLine, 1024);

	char buffer[1024];


	printf("Running Program: ");
	printf("%s ", toChar(AppName, buffer, 1024));
	if (CmdLine != NULL)
		printf("%s\n", toChar(CmdLine, buffer, 1024));
	else
		printf("\n");

	PROCESS_INFORMATION processInformation;
	STARTUPINFO startupInfo;
	memset(&processInformation, 0, sizeof(processInformation));
	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	BOOL result;
	TCHAR tempCmdLine[MAX_PATH * 2];  //Needed since CreateProcessW may change the contents of CmdLine
	if (CmdLine != NULL)
	{
		_tcscpy_s(tempCmdLine, MAX_PATH * 2, CmdLine);
		result = ::CreateProcess(AppName, tempCmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation);
	}
	else
	{
		result = ::CreateProcess(AppName, CmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation);
	}

	if (result == 0)
	{
		printf("ERROR: CreateProcess failed!\n");
	}

	return processInformation;
}