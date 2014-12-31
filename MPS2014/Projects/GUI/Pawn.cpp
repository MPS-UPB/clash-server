#include "Pawn.h"
#include "GL\freeglut.h"
#include "Colors.h"


Pawn::Pawn(int x, int y, int color, int lum, std::string name)
{
	this->color = color;
	this->lum = lum;
	this->name = name;
}

void Pawn::paint()
{
	//load crt values
	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR, currentColor);
	glPushMatrix();

	//draw color
	glColor3d(red_color[color], green_color[color], blue_color[color]);
	glutSolidOctahedron();

	//draw lightness
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(0, 0, 0.5);
	glColor3d(lum_color[lum], lum_color[lum], lum_color[lum]);
	glutSolidOctahedron();

	//restore values
	glPopMatrix();
	glColor4f(currentColor[0], currentColor[1], currentColor[2], currentColor[3]);
}

int Pawn::getX()
{
	return x;
}

int Pawn::getY()
{
	return y;
}