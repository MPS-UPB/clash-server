#include "zeromq/include/zmq.h"
#include <string>
#include <iostream>
#include "Players.h"

#pragma comment(lib, "util.lib")
#include "util/util.h"
#include "util\Addresses.h"
#include "util\CommonMessages.h"
#include "util\Definitions.h"

#pragma comment(lib, "mailman.lib")
#include "Mailman\ComClient.h"
#include "Mailman\ComServer.h"

ComClient *mailman_server;
ComClient *mailman_gui;
ComServer *mailman_from_server;

Players players;

bool game_ended = false;

void startServer(char *name, char *params)
{
	//start exe
	runProcess(name, params);

	//start communication channel to server
	mailman_server = new ComClient(SERVER_ADDRESS_TO, TIMEOUT_LOADER_SERVER);

	//connect
	printf("LOADER: Connecting to MPS Server...\n");

	//send hello
	std::string response = mailman_server->sendMessage(HELLO_MSG);

	if (getMessageElement(response, 0).compare(HELLO_BACK_MSG)==0)
		printf("LOADER: Connected to MPS Server!\n");
	else
		printf("LOADER: Error connecting to server:%s\n", response);
}

void startGui(char *name, char *params)
{
	//start exe
	runProcess(name, params);

	mailman_gui = new ComClient(GUI_ADDRESS_TO, TIMEOUT_LOADER_GUI);

	//connect
	printf("LOADER: Starting GUI...\n");

	//send hello and wait for reply
	std::string response = mailman_gui->sendMessage(HELLO_MSG);

	if (getMessageElement(response, 0).compare(HELLO_BACK_MSG) == 0)
		printf("LOADER: GUI Started!\n");
	else
		printf("LOADER: Error starting gui:%s\n", response);
}

void startRound()
{
	printf("LOADER: Starting Round!\n");

	std::string response = mailman_server->sendMessage(START_ROUND_MSG);

	if (getMessageElement(response, 0).compare(START_ROUND_BACK_MSG) == 0)
		printf("LOADER: Round Started!\n");
	else
		printf("LOADER: Error starting round:%s\n", response);
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

						Pawn p(process, crt_team, crt_pawn.substr(crt_pawn.length() - 4));
						players.addPawn(p);
						players.getPawn(p.getId())->connectToServer(mailman_server);
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

std::string restartPawn(std::string msg, void *pthis)
{
	//generate the id
	std::string name;
	std::string team;
	team = getMessageElement(msg, 1);
	name = getMessageElement(msg, 2);
	std::string id(team);
	id.append(name);

	//restart the pawn with the received id
	players.getPawn(id)->restart();

	//send back confirmation and the new PID
	std::string reply(RESTART_PAWN_BACK_MSG);
	reply.append(toString(players.getPawn(id)->getPID()));

	return reply;
}

std::string endGame(std::string msg, void *pthis)
{
	game_ended = true;
	return std::string(STOP_GAME_BACK_MSG);
}

void startListening()
{
	mailman_from_server = new ComServer(LOADER_ADDRESS, 1, TIMEOUT_SERVER_LOADER);
	mailman_from_server->addListener(RESTART_PAWN_MSG, restartPawn);
	mailman_from_server->addListener(STOP_GAME_MSG, endGame);
}

void main()
{
	startServer("Server.exe", NULL);
	startGui("GUI.exe", NULL);
	startClients();
	startRound();

	startListening();

	while (!game_ended)
	{
		mySleep(1000);
	}

	//TO DO: Listening Loop for Loader
	printf("LOADER: Loader Exiting.\n");
}