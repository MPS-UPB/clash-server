#include "Pawn.h"
#include "Game.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

Pawn::Pawn(int pid, const char *team, const char *name)
{
	this->pid=pid;
	this->name=_strdup(name);
	this->team=_strdup(team);

	int dim=strlen(name)+strlen(team)+2;

	id=(char*)malloc(sizeof(char)*(dim));
	strcpy_s(id, dim, team);
	strcat_s(id, dim, ":");
	strcat_s(id, dim, team);
}

Pawn::~Pawn()
{
	free(name);
	free(team);
	free(id);
}

bool Pawn::operator==(const Pawn &other)
{
	return (strcmp(id, other.id)==0);
}

bool Pawn::operator!=(const Pawn &other)
{
	return !(*this==other);
}

char* Pawn::getId()
{
	return id;
}

char* Pawn::getTeam()
{
	return team;
}

char* Pawn::getName()
{
	return name;
}

bool Pawn::fromTeam(char *team)
{
	return (strcmp(this->team, team)==0);
}