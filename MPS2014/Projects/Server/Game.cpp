#include "Game.h"
#include "Pawn.h"
#include "util\util.h"
#include "util\Definitions.h"
#include <vector>
#include <algorithm>
#include <random>

Game *Game::instance = NULL;

Game::Game()
{
	nr_of_playing_teams=0;
	playing_teams_s=NULL;
}

Game* Game::getInstance()
{
	if (instance == NULL)
	{
		instance = new Game();
	}

	return instance;
}

void Game::addUser(int pid, std::string team, std::string name)
{
	//generate id
	std::string id(team);
	id.append(":");
	id.append(name);

	//if id exists, update pid
	Pawn *exists = getUser(id);
	if (exists)
	{
		exists->setPid(pid);
		return;
	}
	//else add pawn
	Pawn new_pawn = Pawn(pid, name, team);

	//if team does not exist, add a new team to the list
	if (teams.find(team) == teams.end())
	{
		pawns_t new_team;
		teams[team] = new_team;
		team_properties[team].color = team_properties.size();
	}

	//add the pawn to the team
	(teams.find(team)->second)[name] = new_pawn;
}


Pawn* Game::getUser(std::string id)
{
	std::string name, team;
	team = getMessageElement(id, 0, false);
	name = getMessageElement(id, 1, false);

	/*
	for (teams_iter_t team = teams.begin(); team != teams.end(); team++)
	{
		for (pawns_iter_t pawn = team->second.begin(); pawn != team->second.end(); pawn++)
			std::cout << i->first << " == " << i->second << std::endl;
	}
	*/

	teams_iter_t team_it = teams.find(team);
	if (team_it != teams.end())
	{
		pawns_iter_t pawn_it = team_it->second.find(name);
		if (pawn_it != team_it->second.end())
		{
			return &(pawn_it->second);
		}
	}

	return NULL;
}

void Game::resetPlayingPawns()
{
	for (teams_iter_t team = teams.begin(); team != teams.end(); team++)
	{
		for (pawns_iter_t pawn = team->second.begin(); pawn != team->second.end(); pawn++)
			pawn->second.setPlayingTeam(std::string());
	}
}

//move x and y in the next position from the line considering th edge
void Game::getDefaultPosition(int edge, int index, int &x, int &y)
{
	//if not on board
	if (x < 0 || y<0)
	{
		x = y = -1;
		return;
	}

	//else get position
	switch (edge)
	{
	case 0 :
		x = index;
		y = 0;
		break;
	case 1:
		x = getBoardSize() - 1;
		y = index;
		break;
	case 2:
		x = getBoardSize() - 1-index;
		y = getBoardSize() - 1;
		break;
	case 3:
		x = 0;
		y = getBoardSize() - 1 - index;
	}
}

void Game::placePlayingPawns()
{
	//Select playing edges
	/*

	Edges:
	|-0-|
	3   1
	|-2-|

	Coordinate System:

	0-- > X
	|
    V
	Y

	*/

	int nr_of_playing_teams = playing_teams.size();

	int e0, e1;
	int edges[4] = { 0, 0, 0, 0 };
	if (nr_of_playing_teams <4)
	{
		e0 = rand() % 2;
		e1 = e0 + 2;
		edges[e0] = 1;
		edges[e1] = 1;
	}
	if (nr_of_playing_teams == 3)
	{
		edges[e0 + 1] = 1;
	}
	if (nr_of_playing_teams == 4)
	{
		edges[0] = edges[1] = edges[2] = edges[3] = 1;
	}

	int crt_edge = 0;
	int x, y;
	//go through all the playing teams
	for (int i = 0; i < nr_of_playing_teams; i++)
	{
		//choose an edge for it
		for (crt_edge; crt_edge < 4; crt_edge++)
			if (edges[crt_edge])
				break;

		x = y = -1;

		std::vector < int >pawn_positions;
		for (int j = 0; j < getTeamSize(); j++)
		{
			pawn_positions.push_back(j);
		}
		std::shuffle(pawn_positions.begin(), pawn_positions.end(), std::default_random_engine(RANDOM_SEED));

		//go through all the pawns and place on board considering where it plays
		for (teams_iter_t team = teams.begin(); team != teams.end(); team++)
		{
			int index = 0;
			for (pawns_iter_t pawn = team->second.begin(); pawn != team->second.end(); pawn++)
				if (pawn->second.playingForTeam(playing_teams[i]))
				{
					getDefaultPosition(crt_edge, pawn_positions[index], x, y);
					pawn->second.setPosition(x, y);
					index++;
				}
		}

	}
}

void Game::choosePlayingPawns()
{
	resetPlayingPawns();

	//-------------------SELECT TEAMS TO PLAY-------------------
	int nr_of_playing_teams = min(4, teams.size());

	playing_teams.clear();
	teams_iter_t ti = teams.begin();

	for (ti = teams.begin(); ti != teams.end(); ti++)
		playing_teams.push_back(ti->first);

	std::shuffle(playing_teams.begin(), playing_teams.end(), std::default_random_engine(RANDOM_SEED));
	playing_teams.resize(nr_of_playing_teams);


	
	//-------------------PUT HALF OF THE ORIGINAL MEMBERS IN THE TEAMS-------------------
	unsigned int half_team = 9999999;
	for (teams_iter_t it = teams.begin(); it != teams.end(); it++)
		if (half_team>it->second.size())
			half_team = it->second.size();
	//half of the smallest team must surely be in the original teams
	half_team /= 2;
	//select half_team random pawns from every team to be in the original team
	//for all playing teams
	for (int i = 0; i < nr_of_playing_teams; i++)
	{
		pawns_t &crt_team = teams[playing_teams[i]];
		unsigned int playing = 0;

		std::vector <Pawn*> random_pawns;
		
		//for all pawns in team
		for (pawns_iter_t itp = crt_team.begin(); itp != crt_team.end(); itp++)
			random_pawns.push_back(&(itp->second));
		//randomly choose half_team membes
		std::shuffle(random_pawns.begin(), random_pawns.end(), std::default_random_engine(RANDOM_SEED));
		random_pawns.resize(half_team);

		for (int j = 0; j < random_pawns.size(); j++)
		{
			random_pawns[j]->setPlayingTeam(playing_teams[i]);
		}
	}

	//-----------------------PUT THE REST OF THE PAWNS IN RANDOM TEAMS------------------

	//add all plausible pawns to a random vector
	std::vector <Pawn*>remaining_pawns;
	for (int i = 0; i < playing_teams.size(); i++)
	{
		pawns_t &team = teams[playing_teams[i]];

		for (pawns_iter_t pawn = team.begin(); pawn != team.end(); pawn++)
			if (pawn->second.getPlayingTeam().length()==0)
				remaining_pawns.push_back(&(pawn->second));
	}

	std::shuffle(remaining_pawns.begin(), remaining_pawns.end(), std::default_random_engine(RANDOM_SEED));

	//for each team
	for (int i = 0; i < playing_teams.size(); i++)
	{
		//give the same number of members
		int nr_remaining_pawns_per_team = getTeamSize() - half_team;
		for (int j = 0; j < nr_remaining_pawns_per_team; j++)
		{
			remaining_pawns[i*playing_teams.size() + j]->setPlayingTeam(playing_teams[i]);
		}
	}

}

int Game::getTeamColor(std::string team)
{
	std::map<std::string, teamPropesties, compStr>::iterator it;
	if ((it = team_properties.find(team)) == team_properties.end())
		return -1;
	return it->second.color;
}

int Game::getBoardSize()
{
	//TO DO
	return 13;
}

int Game::getTeamSize()
{
	//TO DO
	return 12;
}

Game::~Game()
{
}