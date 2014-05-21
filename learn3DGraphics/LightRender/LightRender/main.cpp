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
GLTriangleBatch sphereBatch;

enum ShaderType {
    DIFFUSE_SHADER = 0,
    ADSGUARAUD_SHADER,
    SHADER_NUM
};
GLuint shader[SHADER_NUM];
ShaderType shaderType;

GLint locAmbientColor;
GLint locDiffuseColor;
GLint locSpecularColor;
GLint locLightPos;
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

void selectShader(ShaderType type)
{
    shaderType = type;
    locLightPos = glGetUniformLocation(shader[shaderType], "lightPos");
    if (locLightPos == -1)
        printf("[%d] error\n", __LINE__);
    locDiffuseColor = glGetUniformLocation(shader[shaderType], "diffuseColor");
    if (locDiffuseColor == -1)
        printf("[%d] error\n", __LINE__);
    locMvpMatrix = glGetUniformLocation(shader[shaderType], "mvpMatrix");
    if (locMvpMatrix == -1)
        printf("[%d] error\n", __LINE__);
    locMvMatrix = glGetUniformLocation(shader[shaderType], "mvMatrix");
    if (locMvMatrix == -1)
        printf("[%d] error\n", __LINE__);
    locNormalMatrix = glGetUniformLocation(shader[shaderType], "normalMatrix");
    if (locNormalMatrix == -1)
        printf("[%d] error\n", __LINE__);

    if (shaderType == ADSGUARAUD_SHADER) {
        locAmbientColor = glGetUniformLocation(shader[shaderType], "ambientColor");
        if (locAmbientColor == -1)
            printf("[%d] error\n", __LINE__);

        locSpecularColor = glGetUniformLocation(shader[shaderType], "specularColor");
        if (locSpecularColor == -1)
            printf("[%d] error\n", __LINE__);
    }
}

void setupRC()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    cameraFrame.MoveForward(3);

    gltMakeTorus(torusBatch, 0.25f, 0.10f, 30, 30);
    gltMakeSphere(sphereBatch, 0.15f, 30, 15);

    shader[DIFFUSE_SHADER] = gltLoadShaderPairWithAttributes("diffuseLightShader.vp", "diffuseLightShader.fp",
        2, GLT_ATTRIBUTE_VERTEX, "vertexPos", GLT_ATTRIBUTE_NORMAL, "vertexNormal");

    if (shader[DIFFUSE_SHADER] == NULL)
        printf("load diffuseLightShader error\n");

    shader[ADSGUARAUD_SHADER] = gltLoadShaderPairWithAttributes("ADSGuaraud.vp", "diffuseLightShader.fp",
        2, GLT_ATTRIBUTE_VERTEX, "vertexPos", GLT_ATTRIBUTE_NORMAL, "vertexNormal");

    if (shader[ADSGUARAUD_SHADER] == NULL)
        printf("load ADSGuaraudShader error\n");

    selectShader(ADSGUARAUD_SHADER);
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static CStopWatch timer;
    float elapsedTime = timer.GetElapsedSeconds();

    modelViewM.PushMatrix(cameraFrame);
        float angle = elapsedTime * 120;
        modelViewM.Rotate(angle, 0, 1, 0);
        GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f };
        GLfloat ambientColor[] = {0, 0.0f, 0, 1};
        GLfloat diffuseColor[] = { 1, 0, 0, 1 };
        GLfloat specularColor[] = { 1, 1, 1, 1 };
 
        glUseProgram(shader[shaderType]);
        glUniform3fv(locLightPos, 1, lightPos);
        glUniform4fv(locDiffuseColor, 1, diffuseColor);
        glUniformMatrix4fv(locMvpMatrix, 1, GL_FALSE, pipelineTransform.GetModelViewProjectionMatrix());
        glUniformMatrix4fv(locMvMatrix, 1, GL_FALSE, pipelineTransform.GetModelViewMatrix());
        glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, pipelineTransform.GetNormalMatrix());
        if (shaderType == ADSGUARAUD_SHADER) {
            glUniform4fv(locAmbientColor, 1, ambientColor);
            glUniform4fv(locSpecularColor, 1, specularColor);

        }

        torusBatch.Draw();

    modelViewM.PopMatrix();
    modelViewM.PushMatrix(cameraFrame);
        float elapsedTime4Sphere = elapsedTime;
        static const float PERIOD = 5; // second
        static const float MAX_DISTANCE = 2;
        while (elapsedTime4Sphere > PERIOD)
            elapsedTime4Sphere -= PERIOD;
        float distance = 0;

        if (elapsedTime4Sphere < PERIOD/2)
            distance = 2 * MAX_DISTANCE/ PERIOD * elapsedTime4Sphere - MAX_DISTANCE / 2;
        else
            distance = -2 * MAX_DISTANCE / PERIOD * elapsedTime4Sphere + 3 * MAX_DISTANCE / 2;

        modelViewM.Translate(0.8, distance, 0);

        glUniformMatrix4fv(locMvpMatrix, 1, GL_FALSE, pipelineTransform.GetModelViewProjectionMatrix());
        glUniformMatrix4fv(locMvMatrix, 1, GL_FALSE, pipelineTransform.GetModelViewMatrix());
        glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, pipelineTransform.GetNormalMatrix());

        sphereBatch.Draw();

    modelViewM.PopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 32:
            selectShader((ShaderType)((shaderType + 1) % SHADER_NUM));
            glutPostRedisplay();
            break;

        case 'q':
            break;
    }
}

int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Light Shader");
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyFunc);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    setupRC();
    glutMainLoop();

    return 0;
}
