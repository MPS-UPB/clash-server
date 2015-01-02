#include "Pawn.h"
#include "Game.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

Pawn::Pawn(int pid, std::string team, std::string name)
{
	this->pid=pid;
	this->name=name;
	this->team=team;

	id = std::string(team);
	id.append(":");
	id.append(name);
}

Pawn::~Pawn()
{
}

bool Pawn::operator==(const Pawn &other)
{
	return id.compare(other.id)==0;
}

bool Pawn::operator!=(const Pawn &other)
{
	return !(*this==other);
}

std::string Pawn::getId()
{
	return id;
}

std::string Pawn::getTeam()
{
	return team;
}

std::string Pawn::getName()
{
	return name;
}

bool Pawn::fromTeam(std::string team)
{
	return this->team.compare(team)==0;
}