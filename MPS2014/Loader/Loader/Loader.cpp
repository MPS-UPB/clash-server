#define ZMQ_STATIC
#include "zeromq/include/zmq.h"
#include <string>
#include <iostream>
#include "util.h"

//#include "stdafx.h"
#include <windows.h>
#include <direct.h>
#include <string.h>
#include <tchar.h>


PROCESS_INFORMATION RunTest(TCHAR *AppName, TCHAR *CmdLine)
{
	printf("Running Program: ");
    printf("%s ", AppName);
	if(CmdLine!=NULL)
		printf("%s\n", CmdLine);
	else
		printf("\n", CmdLine);

    PROCESS_INFORMATION processInformation;
    STARTUPINFO startupInfo;
    memset(&processInformation, 0, sizeof(processInformation));
    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

    BOOL result;
    TCHAR tempCmdLine[MAX_PATH * 2];  //Needed since CreateProcessW may change the contents of CmdLine
    if (CmdLine != NULL)
    {
        _tcscpy_s(tempCmdLine, MAX_PATH *2, CmdLine);
        result = ::CreateProcess(AppName, tempCmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation);
    }
    else
    {
        result = ::CreateProcess(AppName, CmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation);
    }

    if (result == 0)
    {
        wprintf(L"ERROR: CreateProcess failed!");
    }

	return processInformation;
}



void startServer(TCHAR *name, TCHAR *params)
{
	PROCESS_INFORMATION processInformation;

	processInformation=RunTest(name, params);

	/*WaitForSingleObject( processInformation.hProcess, INFINITE );
    CloseHandle( processInformation.hProcess );
    CloseHandle( processInformation.hThread );*/
}

void startCommunication(void **context_voidpp, void **responder_voidpp)
{
	void *context=*context_voidpp;
	void *requester=*responder_voidpp;

	printf ("Connecting to hello world server…\n");
	context = zmq_ctx_new ();
	requester = zmq_socket (context, ZMQ_REQ);
	zmq_connect (requester, "tcp://localhost:5555");

	int request_nbr;
	for (request_nbr = 0; request_nbr != 1; request_nbr++) 
	{
		char buffer [10];

		printf ("1 Sending %s\n", "Hello1");
		zmq_send (requester, "Hello1", 7, 0);

		zmq_recv (requester, buffer, 10, 0);
		printf ("1 Received:%s\n", buffer);

		mySleep(1000);
	}
}

void startClients()
{
	WIN32_FIND_DATA found_files;
	TCHAR crt_directory[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	WIN32_FIND_DATA found_pawns;
	TCHAR crt_team[MAX_PATH];
	HANDLE hPawn = INVALID_HANDLE_VALUE;

	TCHAR crt_pawn[MAX_PATH];
	TCHAR cmd_line[MAX_PATH];

	DWORD dwError=0;

	//Find all teams
	strcpy_s(crt_directory, MAX_PATH, ".\\Teams\\*");

	// Find the first file in the directory.
	hFind = FindFirstFile(crt_directory, &found_files);
	assert(INVALID_HANDLE_VALUE != hFind, "Cannot open pawns directory!");

	//List all teams
	do
	{
		if (found_files.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && found_files.cFileName[0]!='.')
		{
			printf("Team: %s\n", found_files.cFileName);

			//Find all pawns in team
			strcpy_s(crt_team, MAX_PATH, ".\\Teams\\");
			strcat_s(crt_team, MAX_PATH, found_files.cFileName);
			strcat_s(crt_team, MAX_PATH, "\\*");

			hPawn = FindFirstFile(crt_team, &found_pawns);
			assert(INVALID_HANDLE_VALUE != hPawn, "Cannot open team directory!");

			//List all members in the teams
			do
			{
				if ((!(found_pawns.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)))
				{
					if(strncmp(&found_pawns.cFileName[strlen(found_pawns.cFileName)-4], ".exe", 4)==0)
					{
						printf("\t%s\n", found_pawns.cFileName);

						//starting pawn
						strcpy_s(crt_pawn, MAX_PATH, ".\\Teams\\");
						strcat_s(crt_pawn, MAX_PATH, found_files.cFileName);
						strcat_s(crt_pawn, MAX_PATH, "\\");
						strcat_s(crt_pawn, MAX_PATH, found_pawns.cFileName);

						strcpy_s(cmd_line, MAX_PATH, "\"");
						strcat_s(cmd_line, MAX_PATH, found_files.cFileName);
						strcat_s(cmd_line, MAX_PATH, "\" \"");
						strcat_s(cmd_line, MAX_PATH, found_pawns.cFileName);
						strcat_s(cmd_line, MAX_PATH, "\"");


						RunTest(crt_pawn, cmd_line);
					}
				}
			}
			while (FindNextFile(hPawn, &found_pawns) != 0);
			assert(dwError != ERROR_NO_MORE_FILES, "Cannot read team directory!");
			FindClose(hPawn);
		}
	}
	while (FindNextFile(hFind, &found_files) != 0);

	assert(dwError != ERROR_NO_MORE_FILES, "Cannot read pawns directory!");

	FindClose(hFind);
}

void main()
{
	void *context;
	void *responder;

	startServer(TEXT("Server.exe"), NULL);
	startClients();
	//startCommunication(&context, &responder);

	
}