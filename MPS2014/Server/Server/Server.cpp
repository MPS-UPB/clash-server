#include <iostream>
#include "Mailman\ComServer.h"
#include "Game.h"
#include<string>
#include <string.h>

ComServer comLoader("tcp://*:5555", 1, 0);
ComServer comPawns("tcp://*:5556", 100, 0);

#include "FunctionCalls.h"



int main()
{
	std::cout<<"MPS Server Started.\n";

	Game *game = Game::getInstance();
	
	comLoader.addListener("connect pawn:", connect_pawn);
	comLoader.addListener("start round:", start_round);
	comLoader.addListener("hello:", replyToHello);
	

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