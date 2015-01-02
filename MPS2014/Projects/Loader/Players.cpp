#include "Players.h"

Players::Players()
{
	pawns = new std::map<std::string, Pawn, compId>();
}

void Players::addPawn(Pawn p)
{
	(*pawns)[p.getId()] = p;
}

Pawn* Players::getPawn(std::string id)
{
	return &((*pawns)[id]);
}

Players::~Players()
{
	delete pawns;
}