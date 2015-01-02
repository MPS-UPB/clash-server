#pragma once
#include <string>
#include "Mailman\ComClient.h"

class Pawn
{
private:
	std::string executable;
	std::string id;
	std::string team;
	std::string name;
	int pid;

	bool pawn_connected = false;

public:
	Pawn(std::string exe, std::string team, std::string name);

	Pawn();

	void start();

	void terminate();

	void restart();

	int getPID();

	std::string getId();

	void Pawn::connectToServer(ComClient *mailman_server);
};