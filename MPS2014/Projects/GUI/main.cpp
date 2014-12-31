# include <windows.h>
# include <GL/gl.h>
# include "GL/glut.h"

# include <iostream>
using namespace std;

int width=800, height=600;


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

	//TO DO: Set a function that does the painting when glutPostRedisplay is called. Use glutDisplayFunc. In that function you can draw the objects

	//TO DO: set a function to be called when the window is resized. Use glutReshapeFunc. In that function you have to set the projection parameters.

	//TO DO: Set a function to be called when the program is in iddle mode (doesn' have work to do). Use glutIdleFunc. In that function you can call glutPostRedisplay in order to animate objects


	//TO DO: set some common states, like background color (glClearColor); enable/disable lighting and materials; set settings for lights (position, material)


	//only returns from here when the program is closed
	glutMainLoop();

	return 0;
}