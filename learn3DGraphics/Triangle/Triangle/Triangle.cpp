#include <GLTools.h>
#include <GLShaderManager.h>
#define FREEGLUT_STATIC
#include <GL/glut.h>
#include <windows.h>
#include <StopWatch.h>

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
	// set viewport square and in the window's center
	int x = 0;
    int y = 0;
    if (w < h) {
        y = (h - w) / 2;
        h = w;
    } else {
        x = (w - h) / 2;
        w = h;
    }

	glViewport(x, y, w, h);
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

    static CStopWatch timer;
    static float lastTime = 0;
    float elapsedSec = timer.GetElapsedSeconds();

	// generate random color
	static GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
    if (elapsedSec - lastTime > 0.1) {
        lastTime = elapsedSec;
        color[0] = (color[0] + 0.01f > 1.0f) ? 0.0f : (color[0] + 0.01f);
        color[1] = (color[1] + 0.02f > 1.0f) ? 0.0f : (color[1] + 0.02f);
        color[2] = (color[2] + 0.03f > 1.0f) ? 0.0f : (color[2] + 0.03f);
    }

	// generate new angle
	float angle = elapsedSec * 60;

	M3DMatrix44f translationMatrix, rotationMatrix, finalMatrix;
	m3dTranslationMatrix44(translationMatrix, xPos, yPos, 0);
	m3dRotationMatrix44(rotationMatrix, m3dDegToRad(angle), 0, 0, 1);
	m3dMatrixMultiply44(finalMatrix, translationMatrix, rotationMatrix);

	shaderManager.UseStockShader(GLT_SHADER_FLAT, finalMatrix, color);
	triangleBatch.Draw();

	glutSwapBuffers();
	started = true;
    glutPostRedisplay();
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
