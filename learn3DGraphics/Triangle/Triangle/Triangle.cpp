#include <GLTools.h>
#include <GLShaderManager.h>
#define FREEGLUT_STATIC
#include <GL/glut.h>
#include <windows.h>

GLBatch triangleBatch;
GLShaderManager shaderManager;
GLfloat vVerts[] = {-0.5f, 0.0f, 0.0f,
					0.5f, 0.0f, 0.0f,
					0.0f, 0.5f, 0.0f};

GLfloat xPos = 0;
GLfloat yPos = 0;

bool started = false;

void changeSize(int w, int h)
{
	glViewport(0, 0, w/2, h/2);
}

void setupRC()
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	shaderManager.InitializeStockShaders();

	triangleBatch.Begin(GL_TRIANGLES, 3);
	triangleBatch.CopyVertexData3f(vVerts);
	triangleBatch.End();
}

void renderScene()
{
	printf("%s\n", __FUNCTION__);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// generate random color
	static GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	color[0] += 0.01f;
	color[1] += 0.02f;
	color[2] += 0.03f;
	if (color[0] > 1.0f) color[0] = 0.0f;
	if (color[1] > 1.0f) color[1] = 0.0f;
	if (color[2] > 1.0f) color[2] = 0.0f;

	// generate new angle
	static GLfloat angle = -5;
	angle = (angle + 5 > 360) ? (angle + 5 - 360) : (angle + 5);

	M3DMatrix44f translationMatrix, rotationMatrix, finalMatrix;
	m3dTranslationMatrix44(translationMatrix, xPos, yPos, 0);
	m3dRotationMatrix44(rotationMatrix, m3dDegToRad(angle), 0, 0, 1);
	m3dMatrixMultiply44(finalMatrix, translationMatrix, rotationMatrix);

	shaderManager.UseStockShader(GLT_SHADER_FLAT, finalMatrix, color);
	triangleBatch.Draw();

	glutSwapBuffers();
	started = true;
}

void keyFunc(int key, int x, int y)
{
	printf("key = %d\n", key);
	static const GLfloat step = 0.025f;
	
	switch (key)
	{
	case GLUT_KEY_UP:
		yPos = (yPos + step > 1) ? 1 : (yPos + step);
		break;
		
	case GLUT_KEY_DOWN:
		yPos = (yPos - step < -1) ? -1 : (yPos - step);
		break;
		
	case GLUT_KEY_LEFT:
		xPos = (xPos - step < -1) ? -1 : (xPos - step);
		break;
		
	case GLUT_KEY_RIGHT:
		xPos = (xPos + step > 1) ? 1 : (xPos + step);
		break;

	default:
		break;
	}
	
	glutPostRedisplay();
}

DWORD WINAPI threadWork(LPVOID threadNo)
{
	while (true) {
		Sleep(200);
		printf("xx\n");
		if (started)
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
	glutCreateWindow("Triangle");
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
