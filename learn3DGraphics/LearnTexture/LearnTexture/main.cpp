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

GLShaderManager shaderManager;
GLMatrixStack modelViewM;
GLMatrixStack projectionM;
GLFrustum frustum;
GLGeometryTransform pipelineTransform;
GLBatch cuboidBatch;
GLFrame cameraFrame;

void changeSize(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    frustum.SetPerspective(35, (float)w/h, 1, 100);
    projectionM.LoadMatrix(frustum.GetProjectionMatrix());
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    modelViewM.PushMatrix();

    M3DMatrix44f cameraM;
    cameraFrame.GetCameraMatrix(cameraM);
    modelViewM.MultMatrix(cameraM);

    modelViewM.Translate(0, 0, -3);

//    glBindTexture(GL_TEXTURE_2D, textureID);

    GLfloat lightPos[] = {1, 1, 1};
    GLfloat lightColor[] = {1, 1, 1, 1};
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
                                 pipelineTransform.GetModelViewMatrix(),
                                 pipelineTransform.GetProjectionMatrix(), 
                                 lightPos, lightColor, 0);
    cuboidBatch.Draw();

    glutPostRedisplay();
}

void keyFunc(int key, int x, int y)
{
}

void generateCuboidBatch(GLBatch& batch, float length, float width, float height)
{
    M3DVector3f frontLeftBottom = {0, 0, 0};
    M3DVector3f frontLeftUp = {0, 0, 0};
    M3DVector3f frontRightBottom = {0, 0, 0};
    M3DVector3f frontRightUp = {0, 0, 0};
    M3DVector3f backLeftBottom = {0, 0, 0};
    M3DVector3f backLeftUp = {0, 0, 0};
    M3DVector3f backRightBottom = {0, 0, 0};
    M3DVector3f backRightUp = {0, 0, 0};

    frontRightBottom[0] = length;
    frontRightUp[0] = length;
    backRightBottom[0] = length;
    backRightUp[0] = length;

    frontLeftUp[1] = height;
    frontRightUp[1] = height;
    backLeftUp[1] = height;
    backRightUp[1] = height;

    frontLeftBottom[2] = width;
    frontLeftUp[2] = width;
    frontRightBottom[2] = width;
    frontRightUp[2] = width;

    batch.Begin(GL_QUADS, 24, 1);
    
    // bottom face
    batch.Normal3f(0, -1, 0);
    batch.MultiTexCoord2f(0, 0, 1);
    batch.Vertex3fv(frontLeftBottom);
    batch.Normal3f(0, -1, 0);
    batch.MultiTexCoord2f(0, 1, 1);
    batch.Vertex3fv(frontRightBottom);
    batch.Normal3f(0, -1, 0);
    batch.MultiTexCoord2f(0, 1, 0);
    batch.Vertex3fv(backRightBottom);
    batch.Normal3f(0, -1, 0);
    batch.MultiTexCoord2f(0, 0, 0);
    batch.Vertex3fv(backLeftBottom);

    // up face
    batch.Normal3f(0, 1, 0);
    batch.MultiTexCoord2f(0, 0, 1);
    batch.Vertex3fv(frontLeftUp);
    batch.Normal3f(0, 1, 0);
    batch.MultiTexCoord2f(0, 1, 1);
    batch.Vertex3fv(frontRightUp);
    batch.Normal3f(0, 1, 0);
    batch.MultiTexCoord2f(0, 1, 0);
    batch.Vertex3fv(backRightUp);
    batch.Normal3f(0, 1, 0);
    batch.MultiTexCoord2f(0, 0, 0);
    batch.Vertex3fv(backLeftUp);
}

void setupRC()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    shaderManager.InitializeStockShaders();
    pipelineTransform.SetMatrixStacks(modelViewM, projectionM);

    generateCuboidBatch(cuboidBatch, 0.1f, 1.0f, 0.2f);
}

void cleanRC()
{
}

int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH| GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("LearnTexture");
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
    cleanRC();
    return 0;
}
