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


void *context;
void *server_socker;

PROCESS_INFORMATION runProcess(TCHAR *AppName, TCHAR *CmdLine)
{
	printf("Running Program: ");
    printf("%s ", AppName);
	if(CmdLine!=NULL)
		printf("%s\n", CmdLine);
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

	processInformation=runProcess(name, params);

	/*WaitForSingleObject( processInformation.hProcess, INFINITE );
    CloseHandle( processInformation.hProcess );
    CloseHandle( processInformation.hThread );*/
}

void startCommunication()
{
	//connect
	printf ("Connecting to MPS Server…\n");
	context = zmq_ctx_new ();
	server_socker = zmq_socket (context, ZMQ_REQ);
	zmq_connect (server_socker, "tcp://localhost:5555");

	//Say hello and wait for answer
	char buffer[20];
	strcpy_s(buffer, 20, "Hello from Loader!");
	zmq_send (server_socker, buffer, strlen(buffer)+1, 0);

	zmq_recv (server_socker, buffer, 10, 0);
	printf ("%s\n", buffer);
}

void connectPawnToServer(char *team, char *name, int pid)
{
	char buffer[1024];
	sprintf_s(buffer, 1024, "connect pawn: %d %s:%s", pid, team, name);

	printf ("LOADER: Connecting %s to the server...\n", buffer);
	zmq_send (server_socker, buffer, strlen(buffer)+1, 0);

	zmq_recv (server_socker, buffer, 10, 0);
	printf ("LOADER: Received: %s\n", buffer);
}

void startRound()
{
	char buffer[1024];
	sprintf_s(buffer, 1024, "start round:");

	printf("LOADER: Starting Round");
	zmq_send(server_socker, buffer, strlen(buffer) + 1, 0);

	zmq_recv(server_socker, buffer, 10, 0);
	printf("LOADER: Received: %s\n", buffer);
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


						PROCESS_INFORMATION pi=runProcess(crt_pawn, cmd_line);
						connectPawnToServer(found_files.cFileName, found_pawns.cFileName, pi.dwProcessId);
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

	startRound();
}

void main()
{
	startServer(TEXT("Server.exe"), NULL);
	startCommunication();
	startClients();
}