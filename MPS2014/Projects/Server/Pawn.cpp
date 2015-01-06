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

	x = y = -1;
}

Pawn::Pawn()
{
	this->pid = -1;
	this->name = std::string();
	this->team = std::string();

	id = std::string();
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

std::string Pawn::getPlayingTeam()
{
	return playing_team;
}

std::string Pawn::getName()
{
	return name;
}

void Pawn::setPid(int pid)
{
	this->pid = pid;
}

void Pawn::setPlayingTeam(std::string playing_team)
{
	this->playing_team = playing_team;
	x = y = -1;
}

void Pawn::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

bool Pawn::fromTeam(std::string team)
{
	return this->team.compare(team)==0;
}

bool Pawn::playingForTeam(std::string team)
{
	return this->playing_team.compare(team) == 0;
}