#include "util\util.h"
# include <GL/gl.h>
# include "GL/glut.h"
#pragma comment(lib, "freeglut.lib")

#include "Mailman\ComServer.h"
#pragma comment(lib, "mailman.lib")

#include "util\Addresses.h"
#include "util\CommonMessages.h"
#include "util\Definitions.h"

# include <iostream>

#include "Board.h"

int width = 800, height = 600;
ComServer commands(GUI_ADDRESS, 1, TIMEOUT_SERVER_GUI);
bool com_init = false;

Board game_board(1,1);

void paint()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSolidSphere(1, 16, 16);

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	width = w;
	height = h;
}

void init()
{
	glClearColor(0, 0, 0, 1);
	//TO DO: set some common states, like background color (glClearColor); enable/disable lighting and materials; set settings for lights (position, material)
	commands.startListening();
}

//Drop All:
std::string clearScene(std::string, void*)
{
	//TO DO
	return NULL;
}

//Set Pawn:team:name:color:luminosity:x:y
std::string addPawn(std::string, void*)
{
	//TO DO
	return NULL;
}

//Set Board:w:h
std::string setBoard(std::string, void*)
{
	//TO DO: set board size
	return std::string("Done");
}

//Repaint:
std::string repaint(std::string, void*)
{
	glutPostRedisplay();
	return std::string();
}

void idle()
{
	if (!com_init)
	{
		commands.addListener(GUI_CLEAR_SCENE_MSG, clearScene);
		commands.addListener(GUI_ADD_PAWN_MSG, addPawn);
		commands.addListener(GUI_SET_BOARD_MSG, setBoard);
		commands.addListener(GUI_REPAINT_MSG, repaint);
		com_init = true;
	}
}

int main(int argc, char *argv)
{
	//initialize OpenGL
	glutInit(&argc, &argv);

	//We want color images and double buffering
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);// | GLUT_DEPTH);

	//set the window size
	glutInitWindowSize(width, height);
	//place it somewhere on the screen
	glutInitWindowPosition(100, 100);
	//create the window using the previous settings
	glutCreateWindow("GLUT Window");

	//Settings for full screen
	//glutGameModeString("640x480:16@60");
	//glutEnterGameMode();

	//callback functions
	glutDisplayFunc(paint);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	init();

	//only returns from here when the program is closed
	glutMainLoop();

	return 0;
}