#include <GLTools.h>
#include <GLShaderManager.h>
#define FREEGLUT_STATIC
#include <GL/glut.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

GLfloat squareW = 0;
GLfloat squareX = 0;
GLfloat squareY = 0;

void changeSize(int w, int h)
{
	glViewport(0, 0, w, h);
}

void renderScene()
{
	printf("renderScene\n");
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	GLfloat red = GLfloat(rand() % 0x100) / 0x100;
	GLfloat green = GLfloat(rand() % 0x100) / 0x100;
	GLfloat blue = GLfloat(rand() % 0x100) / 0x100;
	glClearColor(red, green, blue, 1);

	GLint x = rand() % 800;
	GLint y = rand() % 600;
	GLsizei w = 50 + rand() % 50;
	GLsizei h = 50 + rand() % 50;
	glScissor(x, y, w, h);
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	glutSwapBuffers();
}

void keyFunc(int k, int x, int y)
{
}

void setupRC()
{
	srand(time(NULL));
	glClearColor(0, 0, 0, 1);
}

DWORD WINAPI threadWork(LPVOID param)
{
	while (true)
	{
		Sleep(500);
		glutPostRedisplay();
	}
	return 0;
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("TestScissor");
	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);
	glutSpecialFunc(keyFunc);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	setupRC();
	DWORD targetThreadID;
	CreateThread(NULL, 0, threadWork, NULL, 0, &targetThreadID);
	glutMainLoop();
	return 0;
}
