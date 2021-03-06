#pragma once
#include <string>

class Pawn
{
private:
	//process id
	int pid;
	//user name
	std::string name;
	//original team
	std::string team;
	//team:name
	std::string id;

	std::string playing_team;
	//position on board
	int x, y;

public:
	Pawn(int pid, std::string team, std::string name);

	Pawn();

	~Pawn();

	bool operator==(const Pawn &other);

	bool operator!=(const Pawn &other);

	std::string getId();

	std::string getTeam();

	std::string getPlayingTeam();

	std::string getName();

	void setPid(int pid);

	void setPlayingTeam(std::string playing_team);

	void setPosition(int x, int y);

	bool fromTeam(std::string team);

	bool playingForTeam(std::string team);
};