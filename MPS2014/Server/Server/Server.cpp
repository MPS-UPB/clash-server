#include <iostream>
#include "Communication.h"
#include "Game.h"
#include<string>
#include <string.h>

Communication com;

//TO DO: use something else than strtok
std::string connect_pawn(std::string msgs)
{
	int pid;
	std::string name, team;

	char *msg=_strdup(msgs.c_str());
	int len=strlen(msg);
	char *context=NULL;

	char *split=strtok_s(msg, ":", &context);

	pid=atoi(strtok_s(NULL, ":", &context));
	team=std::string(strtok_s(NULL, ":", &context));
	name=std::string(strtok_s(NULL, ":", &context));

	Game *game = Game::getInstance();
	game->addUser(pid, team.c_str(), name.c_str());

	std::string ret=std::string("added user: ");
	ret.append(team);
	ret.append(":");
	ret.append(name);

	free(msg);

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

	Game *game = Game::getInstance();
	game->addUser(544, "retfsdvds", "asfsdvdfgdf");

	
	com.addListener("connect pawn:", connect_pawn);
	com.addListener("start round:", start_round);
	com.startListening();
	com.listeningLoop();

	return 0;				
}