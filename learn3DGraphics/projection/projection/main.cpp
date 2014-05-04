#include <GLTools.h>
#include <GLShaderManager.h>
#define FREEGLUT_STATIC
#include <GL/glut.h>
#include <windows.h>
#include <StopWatch.h>
#include <GLFrustum.h>

GLShaderManager shaderManager;
GLTriangleBatch torusBatch;
GLFrustum frustum;

void changeSize(int w, int h)
{
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	frustum.SetPerspective(35, (float)w / h, 1, 1000);
}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	M3DMatrix44f translateM, rotateM, modelViewM, modelViewProjectionM;

    static CStopWatch timer;
    float angle = timer.GetElapsedSeconds() * 60;

	m3dTranslationMatrix44(translateM, 0, 0, -1.5);
    m3dRotationMatrix44(rotateM, m3dDegToRad(angle), 0, 1, 0);

    m3dMatrixMultiply44(modelViewM, translateM, rotateM);
	m3dMatrixMultiply44(modelViewProjectionM, frustum.GetProjectionMatrix(), modelViewM);

	GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
	shaderManager.UseStockShader(GLT_SHADER_FLAT, modelViewProjectionM, red);

	torusBatch.Draw();
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void keyFunc(int key, int x, int y)
{
}

void setupRC()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	shaderManager.InitializeStockShaders();
	gltMakeTorus(torusBatch, 0.25f, 0.10f, 30, 30);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH| GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Projection");
	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);
	glutSpecialFunc(keyFunc);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	setupRC();
	glutMainLoop();
	return 0;
}
