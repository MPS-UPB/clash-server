#pragma once
#include<string>

class Pawn
{
private:
	int color, lum;
	std::string name;
	int x, y;
public:
	Pawn(int x, int y, int color, int lum, std::string name);

	void paint();

	int getX();

	int getY();
};