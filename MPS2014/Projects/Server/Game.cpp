#include "Game.h"
#include "Pawn.h"
#include "util\util.h"

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
		team_colors[team] = team_colors.size();
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

void Game::placePlayingPawns()
{
	//int edges[4];
}

void Game::choosePlayingPawns()
{
	resetPlayingPawns();

	//select playing teams
	int nr_of_total_teams = teams.size();
	nr_of_playing_teams = min(4, nr_of_total_teams);

	int *playing_teams = new int[nr_of_playing_teams];
	for (int i = 0; i < nr_of_playing_teams; i++)
	{
		//choose a random team
		playing_teams[i] = rand()%nr_of_total_teams;

		//if allready chosen, try again
		for (int j = 0; j < i;j++)
			if (playing_teams[i] == playing_teams[j])
			{
				i--;
				break;
			}
	}

	//find the team names
	if (playing_teams_s != NULL)
		delete playing_teams_s;
	playing_teams_s = new std::string[nr_of_playing_teams];
	for (int i = 0; i < nr_of_playing_teams; i++)
	{
		std::map<std::string, int>::iterator it;
		for (it = team_colors.begin(); it != team_colors.end(); it++)
		{
			if (it->second == playing_teams[i])
				playing_teams_s[i] = it->first;
		}
	}

	//number of sure players in a team
	unsigned int half_team = 9999999;
	int nr_of_players = 0;


	//count number of players in each playing team
	int *nr_players_per_team = new int[nr_of_playing_teams];

	for (teams_iter_t it = teams.begin(); it != teams.end(); it++)
		if (half_team>it->second.size())
			half_team = it->second.size();
	half_team /= 2;

	//find number of players currently playing
	for (int i = 0; i < nr_of_playing_teams; i++)
	{
		nr_of_players += teams[playing_teams_s[i]].size();
	}

	//select half_team random pawns from every team to be in the original team
	//for all playing teams
	for (int i = 0; i < nr_of_playing_teams; i++)
	{
		int nr_of_team_players = teams[playing_teams_s[i]].size();
		int probability = nr_of_team_players / half_team;

		pawns_t crt_team = teams[playing_teams_s[i]];
		unsigned int playing = 0;
		//retry as long as not enough pawns choosen
		while (playing < half_team)
		{
			//for all pawns in team
			for (pawns_iter_t itp = crt_team.begin(); itp != crt_team.end(); itp++)
				//if pawn was lucky and wasn't chosen yet
				if (rand() % probability == 0 && itp->second.getPlayingTeam().size() == 0)
				{
					//add it to the playing team
					itp->second.setPlayingTeam(playing_teams_s[i]);

					//if enough pawns, stop
					playing++;
					if (playing == half_team)
						break;
				}
		}
	}

	//choose randomly from the other playing members and assign them to random teams
	int probability = 
		(nr_of_players - (half_team*nr_of_playing_teams)) / 
		(getTeamSize() - half_team*nr_of_playing_teams);
	int *playing_team_size = new int[nr_of_playing_teams];
	for (int i = 0; i < nr_of_playing_teams; i++) playing_team_size[i] = 0;

	int players_on_board = 0;

	//retry as long as not enough pawns choosen
	while (players_on_board < getTeamSize()*nr_of_playing_teams)
	{
		//for all teams
		for (teams_iter_t team = teams.begin(); team != teams.end(); team++)
		{
			//for all pawns
			for (pawns_iter_t pawn = team->second.begin(); pawn != team->second.end(); pawn++)
			{
				//if was lucky and wasn't chosen for a team yet
				if (rand() % probability == 0 && pawn->second.getTeam().size() == 0)
				{
					//randomly choose a team that doesn't have enough members
					int playing_team;
					do
					{
						playing_team = rand() % nr_of_playing_teams;
					} while (playing_team_size[playing_team] >= getTeamSize());
					//and assign the pawn to it
					pawn->second.setPlayingTeam(playing_teams_s[playing_team]);

					//if all teams are full, stop
					players_on_board++;
					if (players_on_board == getTeamSize()*nr_of_playing_teams)
						break;
				}
				if (players_on_board == getTeamSize()*nr_of_playing_teams)
					break;
			}
		}
	}

	delete playing_teams;
	delete nr_players_per_team;
	delete playing_team_size;
}

int Game::getTeamColor(std::string team)
{
	std::map<std::string, int>::iterator it;
	if ((it=team_colors.find(team)) == team_colors.end())
		return -1;
	return it->second;
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
	if (playing_teams_s != NULL)
		delete playing_teams_s;
}