#include <iostream>
#include "Mailman\ComServer.h"
#include "Game.h"
#include<string>
#include <string.h>
#include "util\Addresses.h"
#include "util\Definitions.h"
#include "util\CommonMessages.h"

#pragma comment (lib, "Mailman.lib")

ComServer comLoader(LOADER_ADDRESS, 1, TIMEOUT_SERVER_LOADER);
ComServer comPawns(SERVER_ADDRESS, WORKER_THREADS_NR_DEF, TIMEOUT_SERVER_PAWN);

#include "FunctionCalls.h"

int main()
{
	std::cout<<"MPS Server Started.\n";

	Game *game = Game::getInstance();
	
	comLoader.addListener(HELLO_MSG, replyToHello);
	comLoader.addListener(CONNECT_PAWN_MSG, connect_pawn);
	comLoader.addListener(START_ROUND_MSG, start_round);
	
	comPawns.addListener("send path:", sendPath);
	comPawns.addListener("send message:", sendMessage);
	comPawns.addListener("do continue:", doContinue);
	comPawns.addListener("get cell state:", getCellState);
	comPawns.addListener("get path:", getPath);
	comPawns.addListener("get position:", getPosition);
	comPawns.addListener("get grade:", getGrade);
	comPawns.addListener("get color:", getColor);
	
	comLoader.startListening();

	return 0;				
}