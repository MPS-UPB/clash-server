#include "Game.h"
#include "Pawn.h"

Game *Game::instance = NULL;

Game::Game()
{

}

Game* Game::getInstance()
{
	if (instance == NULL)
	{
		instance = new Game();
	}

	return instance;
}

void Game::addUser(int pid, const char *team, const char *name)
{
	Pawn new_member=Pawn(pid, name, team);
	users.push_back(new_member);
}