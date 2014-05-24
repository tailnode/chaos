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
const unsigned int REDRAW_INTERVAL = 10;

enum ShaderType {
    DIFFUSE_SHADER = 0,
    ADSGOURAUD_SHADER,
    ADSPHONG_SHADER,
    DISSOLVE_SHADER,
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
GLint locTexture;
GLint locDissolveThreshold;

GLuint cloudTexture;

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

    if (shaderType == ADSGOURAUD_SHADER || shaderType == ADSPHONG_SHADER || shaderType == DISSOLVE_SHADER) {
        locAmbientColor = glGetUniformLocation(shader[shaderType], "ambientColor");
        if (locAmbientColor == -1)
            printf("[%d] error\n", __LINE__);

        locSpecularColor = glGetUniformLocation(shader[shaderType], "specularColor");
        if (locSpecularColor == -1)
            printf("[%d] error\n", __LINE__);

        if (shaderType == DISSOLVE_SHADER) {
            locTexture = glGetUniformLocation(shader[shaderType], "cloudTexture");
            locDissolveThreshold = glGetUniformLocation(shader[shaderType], "dissolveThreshold");
        }
    }

    if (shaderType == DISSOLVE_SHADER)
        glDisable(GL_CULL_FACE);
    else
        glEnable(GL_CULL_FACE);
}

bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
    GLbyte *pBits;
    int nWidth, nHeight, nComponents;
    GLenum eFormat;

    // Read the texture bits
    pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
    if (pBits == NULL)
        return false;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
        eFormat, GL_UNSIGNED_BYTE, pBits);

    free(pBits);

    if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
        minFilter == GL_NEAREST_MIPMAP_NEAREST)
        glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

void setupRC()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    cameraFrame.MoveForward(3);

    gltMakeTorus(torusBatch, 0.25f, 0.10f, 30, 30);
    gltMakeSphere(sphereBatch, 0.15f, 30, 15);

    shader[DIFFUSE_SHADER] = gltLoadShaderPairWithAttributes("diffuseLightShader.vp", "diffuseLightShader.fp",
        2, GLT_ATTRIBUTE_VERTEX, "vertexPos", GLT_ATTRIBUTE_NORMAL, "vertexNormal");

    if (shader[DIFFUSE_SHADER] == NULL)
        printf("load diffuseLightShader error\n");

    shader[ADSGOURAUD_SHADER] = gltLoadShaderPairWithAttributes("ADSGouraud.vp", "diffuseLightShader.fp",
        2, GLT_ATTRIBUTE_VERTEX, "vertexPos", GLT_ATTRIBUTE_NORMAL, "vertexNormal");

    if (shader[ADSGOURAUD_SHADER] == NULL)
        printf("load ADSGouraudShader error\n");

    shader[ADSPHONG_SHADER] = gltLoadShaderPairWithAttributes("ADSPhong.vp", "ADSPhong.fp",
        2, GLT_ATTRIBUTE_VERTEX, "vertexPos", GLT_ATTRIBUTE_NORMAL, "vertexNormal");

    if (shader[ADSPHONG_SHADER] == NULL)
        printf("load ADSPhongShader error\n");

    shader[DISSOLVE_SHADER] = gltLoadShaderPairWithAttributes("dissolve.vp", "dissolve.fp",
        3, GLT_ATTRIBUTE_VERTEX, "vertexPos", GLT_ATTRIBUTE_NORMAL, "vertexNormal", GLT_ATTRIBUTE_TEXTURE0, "texCoords");

    if (shader[DISSOLVE_SHADER] == NULL)
        printf("load dissolveShader error\n");
    
    selectShader(DIFFUSE_SHADER);

    glGenTextures(1, &cloudTexture);
    glBindTexture(GL_TEXTURE_2D, cloudTexture);
    LoadTGATexture("Clouds.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

void timerCB(int millisec)
{
    glutPostRedisplay();
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static CStopWatch timer;
    float elapsedTime = timer.GetElapsedSeconds();

    modelViewM.PushMatrix(cameraFrame);
        float angle = elapsedTime * 120;
        modelViewM.Rotate(angle, 0, 1, 0);
        GLfloat lightPos[] = { -100.0f, 100.0f, 100.0f };
        GLfloat ambientColor[] = {0, 0.0f, 0, 1};
        GLfloat diffuseColor[] = { 1, 0, 0, 1 };
        GLfloat specularColor[] = { 1, 1, 1, 1 };
 
        glUseProgram(shader[shaderType]);
        glUniform3fv(locLightPos, 1, lightPos);
        glUniform4fv(locDiffuseColor, 1, diffuseColor);
        glUniformMatrix4fv(locMvpMatrix, 1, GL_FALSE, pipelineTransform.GetModelViewProjectionMatrix());
        glUniformMatrix4fv(locMvMatrix, 1, GL_FALSE, pipelineTransform.GetModelViewMatrix());
        glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, pipelineTransform.GetNormalMatrix());
        if (shaderType == ADSGOURAUD_SHADER || shaderType == ADSPHONG_SHADER || shaderType == DISSOLVE_SHADER) {
            glUniform4fv(locAmbientColor, 1, ambientColor);
            glUniform4fv(locSpecularColor, 1, specularColor);

            if (shaderType == DISSOLVE_SHADER) {
                glUniform1i(locTexture, 0);
                static const float DISSOLVE_TIME = 7.8f;
                static const float MAX_DISSOLVE_VALUE = 0.7f;
                float time = fmod(elapsedTime, DISSOLVE_TIME * 2);
                float dissolveThreshold = 1.0f;
                if (time < DISSOLVE_TIME)
                    dissolveThreshold = MAX_DISSOLVE_VALUE / DISSOLVE_TIME * time;
                else
                    dissolveThreshold = -MAX_DISSOLVE_VALUE / DISSOLVE_TIME * time + 2 * MAX_DISSOLVE_VALUE;
                glUniform1f(locDissolveThreshold, dissolveThreshold);
            }
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
    glutTimerFunc(REDRAW_INTERVAL, timerCB, 0);
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
