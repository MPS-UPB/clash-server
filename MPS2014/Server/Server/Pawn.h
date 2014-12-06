#pragma once

class Pawn
{
private:
	//process id
	int pid;
	//user name
	char *name;
	//original team
	char *team;
	//team:name
	char *id;

public:
	Pawn(int pid, const char *team, const char *name);

	~Pawn();

	bool operator==(const Pawn &other);

	bool operator!=(const Pawn &other);

	char* getId();

	char* getTeam();

	char* getName();

	bool fromTeam(char *team);
};