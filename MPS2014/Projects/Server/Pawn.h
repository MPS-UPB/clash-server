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

public:
	Pawn(int pid, std::string team, std::string name);

	~Pawn();

	bool operator==(const Pawn &other);

	bool operator!=(const Pawn &other);

	std::string getId();

	std::string getTeam();

	std::string getName();

	bool fromTeam(std::string team);
};