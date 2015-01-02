#include "zeromq/include/zmq.h"
#include <string>
#include <iostream>

#pragma comment(lib, "util.lib")
#include "util/util.h"
#include "util\Addresses.h"
#include "util\CommonMessages.h"

#pragma comment(lib, "mailman.lib")
#include "Mailman\ComClient.h"

ComClient *mailman_server;
ComClient *mailman_gui;

bool server_started = false;
bool gui_started = false;
bool pawn_connected = false;
bool round_started = false;

std::string waitForPawn(std::string msg, void *opt)
{
	pawn_connected = true;
	return std::string();
}

std::string waitForServer(std::string msg, void *opt)
{
	server_started = true;
	return std::string();
}

std::string waitForGUI(std::string msg, void *opt)
{
	gui_started = true;
	return std::string();
}

std::string waitForRoundStart(std::string msg, void *opt)
{
	round_started = true;
	return std::string();
}

void initListeners()
{
	mailman_server->addListener(HELLO_BACK_MSG, waitForServer);
	mailman_server->addListener(CONNECT_PAWN_MSG, waitForPawn);
	mailman_server->addListener(START_ROUND_MSG, waitForRoundStart);

	mailman_gui->addListener(HELLO_BACK_MSG, waitForGUI);
}

void startServer(char *name, char *params)
{
	//start exe
	runProcess(name, params);

	//start communication channel to server
	mailman_server = new ComClient(SERVER_ADDRESS_TO, -1);
	initListeners();

	//connect
	printf("LOADER: Connecting to MPS Server...\n");

	//send hello
	mailman_server->sendMessage(HELLO_MSG);

	//and wait for reply
	while (!server_started)
	{
		mySleep(100);
	}

	printf("LOADER: Connected to MPS Server!\n");
}

void startGui(char *name, char *params)
{
	//start exe
	runProcess(name, params);

	mailman_gui = new ComClient(GUI_ADDRESS_TO, -1);

	//connect
	printf("LOADER: Starting GUI...\n");

	//send hello and wait for reply
	
	mailman_gui->sendMessage(HELLO_MSG);

	while (!gui_started)
	{
		mySleep(100);
	}

	printf("LOADER: GUI Started!\n");
}

void connectPawnToServer(std::string team, std::string name, int pid)
{
	printf("LOADER: Connecting %s:%s...\n", team, name);

	//construct message for server
	std::string message(CONNECT_PAWN_MSG);
	char buffer[1024];
	sprintf_s(buffer, 1024, "%d", pid);
	message.append(buffer);
	message.append(":");
	message.append(team);
	message.append(":");
	message.append(name);

	//send message
	pawn_connected = false;
	mailman_server->sendMessage(message);

	//wait for confirmation
	while (!pawn_connected)
	{
		mySleep(100);
	}

	printf("LOADER: Connected!\n");
}

void startRound()
{
	printf("LOADER: Starting Round!\n");
	round_started = false;
	mailman_server->sendMessage(START_ROUND_MSG);

	while (!round_started)
	{
		mySleep(100);
	}

	printf("LOADER: Round Started!\n");
}

void startClients()
{
	char buffer[MAX_PATH];
	WCHAR wbuffer[MAX_PATH];

	WIN32_FIND_DATA found_files;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	WIN32_FIND_DATA found_pawns;
	HANDLE hPawn = INVALID_HANDLE_VALUE;

	DWORD dwError=0;

	//Find all teams
	std::string crt_directory(TEAMS_PATH);
	crt_directory.append("\\*");
	
	// Find the first file in the directory.
	hFind = FindFirstFile(toWchar(crt_directory.c_str(), wbuffer, MAX_PATH), &found_files);
	assert(INVALID_HANDLE_VALUE != hFind, "LOADER: Cannot open pawns directory!\n");

	//List all teams
	do
	{
		if (found_files.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && found_files.cFileName[0]!='.')
		{
			std::string crt_team;

			//Find all pawns in team
			crt_team.append(TEAMS_PATH);
			crt_team.append("\\");
			crt_team.append(toChar(found_files.cFileName, buffer, MAX_PATH));
			crt_team.append("\\*");

			hPawn = FindFirstFile(toWchar(crt_team.c_str(), wbuffer, MAX_PATH), &found_pawns);
			assert(INVALID_HANDLE_VALUE != hPawn, "LOADER: Cannot open team directory!\n");

			//List all members in the teams
			do
			{
				//If it is a file
				if ((!(found_pawns.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)))
				{
					std::string crt_pawn(toChar(found_pawns.cFileName, buffer, MAX_PATH));
					//And executable
					if (crt_pawn.compare(crt_pawn.length()-4, 4, ".exe") == 0)
					{
						//Construct running path
						std::string process(TEAMS_PATH);
						process.append("\\");
						process.append(crt_team);
						process.append("\\");
						process.append(crt_pawn);

						//and run process
						PROCESS_INFORMATION pi=runProcess(process.c_str(), NULL);

						//connecting pawn to server
						connectPawnToServer(crt_team, crt_pawn.substr(crt_pawn.length()-4), pi.dwProcessId);
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
}

void main()
{
	startServer("Server.exe", NULL);
	startGui("GUI.exe", NULL);
	startClients();
	startRound();

	//TO DO: Listening Loop for Loader
	printf("LOADER: Loader Exiting.\n");
}