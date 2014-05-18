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
#include <StopWatch.h>
#include <assert.h>
#include <iostream>
using namespace std;

GLMatrixStack modelViewM;
GLMatrixStack projectionM;
GLFrustum frustum;
GLGeometryTransform pipelineTransform;
GLFrame cameraFrame;
GLTriangleBatch torusBatch;

GLuint diffuseLightShader;
GLint locLightPos;
GLint locObjectColor;
GLint locMvpMatrix;
GLint locMvMatrix;
GLint locNormalMatrix;

void changeSize(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    frustum.SetPerspective(35, (float)w / h, 1, 100);
    projectionM.LoadMatrix(frustum.GetProjectionMatrix());
    pipelineTransform.SetMatrixStacks(modelViewM, projectionM);
}

void setupRC()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    cameraFrame.MoveForward(3);

    gltMakeTorus(torusBatch, 0.25f, 0.10f, 30, 30);

    diffuseLightShader = gltLoadShaderPairWithAttributes("diffuseLightShader.vp", "diffuseLightShader.fp",
        2, GLT_ATTRIBUTE_VERTEX, "vertexPos", GLT_ATTRIBUTE_NORMAL, "vertexNormal");

    if (diffuseLightShader == NULL)
        printf("load shader error\n");

    locLightPos = glGetUniformLocation(diffuseLightShader, "lightPos");
    if (locLightPos == -1)
        printf("[%d] error\n", __LINE__);
    locObjectColor = glGetUniformLocation(diffuseLightShader, "objectColor");
    if (locObjectColor == -1)
        printf("[%d] error\n", __LINE__);
    locMvpMatrix = glGetUniformLocation(diffuseLightShader, "mvpMatrix");
    if (locMvpMatrix == -1)
        printf("[%d] error\n", __LINE__);
    locMvMatrix = glGetUniformLocation(diffuseLightShader, "mvMatrix");
    if (locMvMatrix == -1)
        printf("[%d] error\n", __LINE__);
    locNormalMatrix = glGetUniformLocation(diffuseLightShader, "normalMatrix");
    if (locNormalMatrix == -1)
        printf("[%d] error\n", __LINE__);
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static CStopWatch timer;
    float angle = timer.GetElapsedSeconds() * 120;

    modelViewM.PushMatrix(cameraFrame);
        modelViewM.Rotate(angle, 0, 1, 0);
        GLfloat lightPos[] = {-100.0f, 100.0f, 100.0f};
        GLfloat color[] = {1, 0, 0, 1};
 
        glUseProgram(diffuseLightShader);
        glUniform3fv(locLightPos, 1, lightPos);
        glUniform4fv(locObjectColor, 1, color);
        glUniformMatrix4fv(locMvpMatrix, 1, GL_FALSE, pipelineTransform.GetModelViewProjectionMatrix());
        glUniformMatrix4fv(locMvMatrix, 1, GL_FALSE, pipelineTransform.GetModelViewMatrix());
        glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, pipelineTransform.GetNormalMatrix());

        torusBatch.Draw();
    modelViewM.PopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("LearnTexture");
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    setupRC();
    glutMainLoop();

    return 0;
}
