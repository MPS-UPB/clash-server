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

PROCESS_INFORMATION runProcess(char *AppName_c, char *CmdLine_c)
{

	WCHAR AppName[1024], CmdLine[1024];

	//MultiByteToWideChar( 0,0, AppName_c, -1, AppName, 1024);
	//MultiByteToWideChar( 0,0, CmdLine_c, -1, CmdLine, 1024);

	toWchar(AppName_c, AppName, 1024);
	toWchar(CmdLine_c, CmdLine, 1024);

	char buffer[1024];


	printf("LOADER: Running Program: ");
    printf("%s ", toChar(AppName, buffer, 1024));
	if(CmdLine!=NULL)
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
        _tcscpy_s(tempCmdLine, MAX_PATH *2, CmdLine);
        result = ::CreateProcess(AppName, tempCmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation);
    }
    else
    {
        result = ::CreateProcess(AppName, CmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation);
    }

    if (result == 0)
    {
		printf("LOADER: ERROR: CreateProcess failed!\n");
    }

	return processInformation;
}



void startServer(char *name, char *params)
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
	printf ("LOADER: Connecting to MPS Server...\n");
	context = zmq_ctx_new ();
	server_socker = zmq_socket (context, ZMQ_REQ);
	zmq_connect (server_socker, "tcp://localhost:5555");

	//Say hello and wait for answer
	char buffer[1024];
	strcpy_s(buffer, 1024, "Hello from Loader!");
	printf ("LOADER: Sending: %s\n", buffer);
	zmq_send (server_socker, buffer, strlen(buffer)+1, 0);

	zmq_recv (server_socker, buffer, 1024, 0);
	printf ("LOADER: Received: %s\n", buffer);
}

void connectPawnToServer(char *team, char *name, int pid)
{
	char buffer[1024];
	sprintf_s(buffer, 1024, "connect pawn: %d:%s:%s", pid, team, name);

	printf ("LOADER: Sending: %s\n", buffer);
	zmq_send (server_socker, buffer, strlen(buffer)+1, 0);

	zmq_recv (server_socker, buffer, 1024, 0);
	printf ("LOADER: Received: %s\n", buffer);
}

void startRound()
{
	char buffer[1024];
	sprintf_s(buffer, 1024, "start round:");

	printf("LOADER: Sending: %s\n", buffer);
	zmq_send(server_socker, buffer, strlen(buffer) + 1, 0);

	zmq_recv(server_socker, buffer, 1024, 0);
	printf("LOADER: Received: %s\n", buffer);
}

void startClients()
{
	char buffer[MAX_PATH];
	WIN32_FIND_DATA found_files;
	char crt_directory[MAX_PATH];
	WCHAR crt_directory_w[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	WIN32_FIND_DATA found_pawns;
	char crt_team[MAX_PATH];
	WCHAR crt_team_w[MAX_PATH];
	HANDLE hPawn = INVALID_HANDLE_VALUE;

	char crt_pawn[MAX_PATH];
	char cmd_line[MAX_PATH];

	DWORD dwError=0;

	//Find all teams
	strcpy_s(crt_directory, MAX_PATH, ".\\Teams\\*");

	// Find the first file in the directory.
	hFind = FindFirstFile(toWchar(crt_directory, crt_directory_w, 1024), &found_files);
	assert(INVALID_HANDLE_VALUE != hFind, "LOADER: Cannot open pawns directory!\n");

	//List all teams
	do
	{
		if (found_files.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && found_files.cFileName[0]!='.')
		{
			//printf("Team: %s\n", found_files.cFileName);

			//Find all pawns in team
			strcpy_s(crt_team, MAX_PATH, ".\\Teams\\");
			strcat_s(crt_team, MAX_PATH, toChar(found_files.cFileName, buffer, MAX_PATH));
			strcat_s(crt_team, MAX_PATH, "\\*");

			hPawn = FindFirstFile(toWchar(crt_team, crt_team_w, MAX_PATH), &found_pawns);
			assert(INVALID_HANDLE_VALUE != hPawn, "LOADER: Cannot open team directory!\n");

			//List all members in the teams
			do
			{
				if ((!(found_pawns.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)))
				{
					char *cFileName=toChar(found_pawns.cFileName, buffer, MAX_PATH);
					if(strncmp(&cFileName[strlen(cFileName)-4], ".exe", 4)==0)
					{
						//printf("\t%s\n", found_pawns.cFileName);

						//starting pawn
						strcpy_s(crt_pawn, MAX_PATH, ".\\Teams\\");
						strcat_s(crt_pawn, MAX_PATH, toChar(found_files.cFileName, buffer, MAX_PATH));
						strcat_s(crt_pawn, MAX_PATH, "\\");
						strcat_s(crt_pawn, MAX_PATH, toChar(found_pawns.cFileName, buffer, MAX_PATH));

						strcpy_s(cmd_line, MAX_PATH, "\"");
						strcat_s(cmd_line, MAX_PATH, toChar(found_files.cFileName, buffer, MAX_PATH));
						strcat_s(cmd_line, MAX_PATH, "\" \"");
						strcat_s(cmd_line, MAX_PATH, toChar(found_pawns.cFileName, buffer, MAX_PATH));
						strcat_s(cmd_line, MAX_PATH, "\"");


						PROCESS_INFORMATION pi=runProcess(crt_pawn, cmd_line);

						//connecting pawn to server
						char name[1024], team[1024];
						toChar(found_files.cFileName, team, 1024);
						toChar(found_pawns.cFileName, name, 1024);

						//printf("Connecting pawn: %s %s\n", team, name);
						connectPawnToServer(team, name, pi.dwProcessId);
					}
				}
			}
			while (FindNextFile(hPawn, &found_pawns) != 0);
			assert(dwError != ERROR_NO_MORE_FILES, "LOADER: Cannot read team directory!\n");
			FindClose(hPawn);
		}
	}
	while (FindNextFile(hFind, &found_files) != 0);

	assert(dwError != ERROR_NO_MORE_FILES, "LOADER: Cannot read pawns directory!\n");

	FindClose(hFind);

	startRound();
}

void main()
{
	startServer("Server.exe", NULL);
	startCommunication();
	startClients();

	printf("LOADER: Loader Exiting.\n");
}