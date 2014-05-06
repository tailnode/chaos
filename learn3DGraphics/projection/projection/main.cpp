#include <GLTools.h>
#include <GLShaderManager.h>
#define FREEGLUT_STATIC
#include <GL/glut.h>
#include <windows.h>
#include <StopWatch.h>
#include <GLFrustum.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <GLFrame.h>
#include <time.h>
#include <stdlib.h>

GLShaderManager shaderManager;
GLTriangleBatch torusBatch;
GLTriangleBatch sphereBatch;
GLBatch floorBatch;
GLFrustum frustum;

GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipeline;
GLFrame cameraFrame;

static const int SPHERE_NUMS = 50;
GLFrame spheres[SPHERE_NUMS];

void initSpheres()
{
    for (int i = 0; i < SPHERE_NUMS; i++) {
		float x = (rand() % 400 - 200) * 0.1f;
		float y = (rand() % 100 - 50) * 0.1f;
		float z = (rand() % 400 - 200) * 0.1f;
		spheres[i].SetOrigin(x, y, z);
	}
}

void changeSize(int w, int h)
{
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	frustum.SetPerspective(35, (float)w / h, 1, 100);
    projectionMatrix.LoadMatrix(frustum.GetProjectionMatrix());

    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static CStopWatch timer;
    float torusAngle = timer.GetElapsedSeconds() * 120;
    float sphereAngle = timer.GetElapsedSeconds() * 120;

	GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat green[] = {0.0f, 1.0f, 0.0f, 1.0f};

    modelViewMatrix.PushMatrix();

    M3DMatrix44f cameraMatrix;
    cameraFrame.GetCameraMatrix(cameraMatrix);
    modelViewMatrix.PushMatrix(cameraMatrix);

    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), black);
    floorBatch.Draw();

    modelViewMatrix.Translate(0, 0, -2.5f);
    modelViewMatrix.PushMatrix();
    modelViewMatrix.Rotate(torusAngle, 0, 1, 0);

    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), red);
	torusBatch.Draw();
	
    modelViewMatrix.PopMatrix();
    modelViewMatrix.PushMatrix();

    modelViewMatrix.Translate(0.5f, 0.0f, 0.0f);
    modelViewMatrix.Rotate(sphereAngle, 0, -1, 0);
    modelViewMatrix.Translate(0.5f, 0.0f, 0.0f);
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), green);
    sphereBatch.Draw();

    modelViewMatrix.PopMatrix();

    modelViewMatrix.PushMatrix();
    modelViewMatrix.Translate(-0.5f, 0.0f, 0.0f);
    modelViewMatrix.Rotate(sphereAngle, 0, 1, 0);
    modelViewMatrix.Translate(0.5f, 0.0f, 0.0f);
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), green);
    sphereBatch.Draw();
    modelViewMatrix.PopMatrix();

    for (int i = 0; i < SPHERE_NUMS; i++) {
        modelViewMatrix.PushMatrix();
        modelViewMatrix.MultMatrix(spheres[i]);
        shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), green);
        sphereBatch.Draw();
        modelViewMatrix.PopMatrix();
    }
    modelViewMatrix.PopMatrix();
    modelViewMatrix.PopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();
}

void keyFunc(int key, int x, int y)
{
    static const float step = 0.1f;
    static const float angle = m3dDegToRad(1);
    switch (key)
    {
    case GLUT_KEY_UP:
        cameraFrame.MoveForward(step);
        break;

    case GLUT_KEY_DOWN:
        cameraFrame.MoveForward(-step);
        break;

    case GLUT_KEY_LEFT:
        cameraFrame.RotateWorld(angle, 0, 1, 0);
        break;

    case GLUT_KEY_RIGHT:
        cameraFrame.RotateWorld(-angle, 0, 1, 0);
        break;

    case GLUT_KEY_HOME:
        cameraFrame.RotateWorld(angle, 1, 0, 0);
        break;

    case GLUT_KEY_END:
        cameraFrame.RotateWorld(-angle, 1, 0, 0);
        break;

    case GLUT_KEY_PAGE_DOWN:
        cameraFrame.MoveUp(-step);
        break;

    case GLUT_KEY_PAGE_UP:
        cameraFrame.MoveUp(step);
        break;

	default:
		break;
    }
}

void setupRC()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	shaderManager.InitializeStockShaders();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    
	gltMakeTorus(torusBatch, 0.25f, 0.10f, 30, 30);
    gltMakeSphere(sphereBatch, 0.10f, 30, 15);

    floorBatch.Begin(GL_LINES, 324);
    for (float i = -20; i <= 20; i += 0.5)
    {
        floorBatch.Vertex3f(i, -0.5, 20);
        floorBatch.Vertex3f(i, -0.5, -20);
        
        floorBatch.Vertex3f(20, -0.5, i);
        floorBatch.Vertex3f(-20, -0.5, i);
    }
    floorBatch.End();

    initSpheres();
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

	srand(time(NULL));
	setupRC();
	glutMainLoop();
	return 0;
}
