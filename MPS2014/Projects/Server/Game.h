#pragma once

#include <vector>
#include "Pawn.h"

class Game
{
private:
	static Game *instance;

	Game();


public:
	std::vector<Pawn> users;

	static Game* getInstance();

	void addUser(int pid, std::string team, std::string name);
};