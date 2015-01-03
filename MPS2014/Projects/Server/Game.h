#pragma once

#include <map>
#include <vector>
#include "Pawn.h"

class Game
{
private:
	static Game *instance;

	Game();

	class compStr
	{
	public:
		bool operator()(const std::string &x, const std::string &y) const
		{
			return x<y;
		}
	};

	typedef std::map<std::string, Pawn, compStr>		pawns_t;
	typedef std::map< std::string, pawns_t, compStr>	teams_t;
	typedef pawns_t::iterator							pawns_iter_t;
	typedef teams_t::iterator							teams_iter_t;

	//key=team, value=map(key=name, value=Pawn)
	teams_t teams;
	std::map<std::string, int> team_colors;

	int nr_of_playing_teams;
	std::string *playing_teams_s;

public:
	static Game* getInstance();

	//Adds a new pawn if it doesn't exist. If the ID exists, it updates the PID
	void addUser(int pid, std::string team, std::string name);

	//get a user if it exists else return NULL
	Pawn* getUser(std::string id);

	//no pawn is playing for no team
	void resetPlayingPawns();

	//select players to play this round
	void choosePlayingPawns();

	//place the pawns that are currently on the board according to their teams
	void placePlayingPawns();

	//unique from 0 to nr of tems; or -1 if not exists
	int getTeamColor(std::string team);

	int getBoardSize();

	int getTeamSize();

	~Game();
};