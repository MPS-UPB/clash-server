#pragma once

#include "Pawn.h"
#include <vector>

class Board
{
private:
	std::vector<Pawn> Pawns;
	int w, h;
public:
	Board(int w, int h);

	void paint();

	void addPawn(Pawn p);

	void removeAllPawns();
};