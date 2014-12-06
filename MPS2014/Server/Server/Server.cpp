#include <iostream>
#include "Communication.h"
#include "Game.h"
#include<string>
#include <string.h>

Communication com;

std::string connect_pawn(std::string msg)
{
	int pid;
	std::string name, team;
	
	int start, stop;

	//read command
	start=msg.find(':');
	stop = msg.find(':', start + 1);

	//read pid
	start = stop;
	stop = msg.find(':', start + 1);
	pid = atoi(msg.substr(start + 1, stop - start).c_str());

	//read team
	start = stop;
	stop = msg.find(':', start + 1);
	team=msg.substr(start + 1, stop - start);

	//read name
	start = stop;
	stop = msg.find(':', start + 1);
	name = msg.substr(start + 1, stop - start);

	Game *game = Game::getInstance();
	game->addUser(pid, team.c_str(), name.c_str());

	std::string ret=std::string("added user: ");
	ret.append(team);
	ret.append(":");
	ret.append(name);

	return ret;
}

std::string start_round(std::string msg)
{
	com.removeListener("start round:");
	com.removeListener("connect pawn:");

	return std::string("Round Started!");
}

int main()
{
	std::cout<<"MPS Server Started.\n";

	
	com.addListener("connect pawn:", connect_pawn);
	com.addListener("start round:", start_round);
	com.listeningLoop();

	return 0;				
}