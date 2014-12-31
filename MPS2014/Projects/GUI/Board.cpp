#include "Board.h"
#include "GL\glut.h"

Board::Board(int w, int h)
{
	this->w = w;
	this->h = h;
}

void Board::paint()
{
	//TO DO
}

void Board::addPawn(Pawn p)
{
	Pawns.push_back(p);
}

void Board::removeAllPawns()
{
	Pawns.clear();
}