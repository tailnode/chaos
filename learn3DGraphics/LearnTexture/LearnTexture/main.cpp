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
GLFrame objectFrame;
float cuboidLength = 0.1f;
float cuboidWidth = 1.0f;
float cuboidHeigth = 0.2f;
bool isAnisotropy = false;
bool isAnisotropySupported = false;
GLfloat largest = 1.0f;
GLuint textureID[2];

bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	
	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if(pBits == NULL) 
		return false;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
				 eFormat, GL_UNSIGNED_BYTE, pBits);
	
    free(pBits);
    
    if(minFilter == GL_LINEAR_MIPMAP_LINEAR || 
       minFilter == GL_LINEAR_MIPMAP_NEAREST ||
       minFilter == GL_NEAREST_MIPMAP_LINEAR ||
       minFilter == GL_NEAREST_MIPMAP_NEAREST)
        glGenerateMipmap(GL_TEXTURE_2D);
    
	return true;
}

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

    M3DVector4f lightPos = {0.0f, 10.0f, 5.0f, 1.0f};
    M3DVector4f lightEyePos;
    m3dTransformVector4(lightEyePos, lightPos, cameraM);
    GLfloat whiteLight[] = {1.0f, 1.0f, 1.0f, 1.0f};

    modelViewM.PushMatrix();

    modelViewM.Translate(0, 0, -3);

    M3DMatrix44f objectM;
    objectFrame.GetMatrix(objectM);
    modelViewM.MultMatrix(objectM);

    glBindTexture(GL_TEXTURE_2D, textureID[0]);
#if 0
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
                                 pipelineTransform.GetModelViewMatrix(),
                                 pipelineTransform.GetProjectionMatrix(), 
                                 lightEyePos, whiteLight, 0);
#else
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, 
                                 pipelineTransform.GetModelViewProjectionMatrix(),
                                 0);
#endif
    cuboidBatch.Draw();

    modelViewM.PopMatrix();

    static CStopWatch timer;
    float angle = timer.GetElapsedSeconds() * 120;
    modelViewM.Translate(0.5f, 0.6f, -5);
    modelViewM.Rotate(angle, 1, 1, 1);
    modelViewM.Scale(0.5/cuboidLength, 0.3/cuboidHeigth, 0.4/cuboidWidth);
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
#if 0
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, 
                                 pipelineTransform.GetModelViewMatrix(),
                                 pipelineTransform.GetProjectionMatrix(), 
                                 lightEyePos, whiteLight, 0);
#else
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, 
                                 pipelineTransform.GetModelViewProjectionMatrix(),
                                 0);
#endif

    cuboidBatch.Draw();

    modelViewM.PopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyFunc(int key, int x, int y)
{
    static const float step = 0.1f;
    static const float angle = m3dDegToRad(4);
    switch (key)
    {
    case GLUT_KEY_UP:
        objectFrame.RotateWorld(-angle, 1, 0, 0);
        break;

    case GLUT_KEY_DOWN:
        objectFrame.RotateWorld(angle, 1, 0, 0);
        break;

    case GLUT_KEY_LEFT:
        objectFrame.RotateWorld(-angle, 0, 1, 0);
        break;

    case GLUT_KEY_RIGHT:
        objectFrame.RotateWorld(angle, 0, 1, 0);
        break;

    case GLUT_KEY_HOME:
        cameraFrame.MoveForward(step);
        break;

    case GLUT_KEY_END:
        cameraFrame.MoveForward(-step);
        break;

    // enable/disable anisotropy
    case GLUT_KEY_F1:
        if (isAnisotropySupported) {
            isAnisotropy = !isAnisotropy;
            if (isAnisotropy)
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
            else
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
        }
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void generateCuboidBatch(GLBatch& batch, float length, float width, float height)
{
    float halfLength = length / 2;
    float halfWidth = width / 2;
    float halfHeight = height / 2;

    M3DVector3f frontLeftBottom  = {-halfLength, -halfHeight,  halfWidth};
    M3DVector3f frontLeftTop     = {-halfLength,  halfHeight,  halfWidth};
    M3DVector3f frontRightBottom = { halfLength, -halfHeight,  halfWidth};
    M3DVector3f frontRightTop    = { halfLength,  halfHeight,  halfWidth};
    M3DVector3f backLeftBottom   = {-halfLength, -halfHeight, -halfWidth};
    M3DVector3f backLeftTop      = {-halfLength,  halfHeight, -halfWidth};
    M3DVector3f backRightBottom  = { halfLength, -halfHeight, -halfWidth};
    M3DVector3f backRightTop     = { halfLength,  halfHeight, -halfWidth};

    batch.Begin(GL_QUADS, 24, 1);
    
    // bottom face
    batch.Normal3f(0, -1, 0);
    batch.MultiTexCoord2f(0, 0, 1);
    batch.Vertex3fv(frontLeftBottom);
    batch.Normal3f(0, -1, 0);
    batch.MultiTexCoord2f(0, 0, 0);
    batch.Vertex3fv(backLeftBottom);
    batch.Normal3f(0, -1, 0);
    batch.MultiTexCoord2f(0, 1, 0);
    batch.Vertex3fv(backRightBottom);
    batch.Normal3f(0, -1, 0);
    batch.MultiTexCoord2f(0, 1, 1);
    batch.Vertex3fv(frontRightBottom);

    // up face
    batch.Normal3f(0, 1, 0);
    batch.MultiTexCoord2f(0, 0, 1);
    batch.Vertex3fv(frontLeftTop);
    batch.Normal3f(0, 1, 0);
    batch.MultiTexCoord2f(0, 1, 1);
    batch.Vertex3fv(frontRightTop);
    batch.Normal3f(0, 1, 0);
    batch.MultiTexCoord2f(0, 1, 0);
    batch.Vertex3fv(backRightTop);
    batch.Normal3f(0, 1, 0);
    batch.MultiTexCoord2f(0, 0, 0);
    batch.Vertex3fv(backLeftTop);

    // front face
    batch.Normal3f(0, 0, 1);
    batch.MultiTexCoord2f(0, 0, 3);
    batch.Vertex3fv(frontLeftTop);
    batch.Normal3f(0, 0, 1);
    batch.MultiTexCoord2f(0, 0, 0);
    batch.Vertex3fv(frontLeftBottom);
    batch.Normal3f(0, 0, 1);
    batch.MultiTexCoord2f(0, 3, 0);
    batch.Vertex3fv(frontRightBottom);
    batch.Normal3f(0, 0, 1);
    batch.MultiTexCoord2f(0, 3, 3);
    batch.Vertex3fv(frontRightTop);

    // back face
    batch.Normal3f(0, 0, -1);
    batch.MultiTexCoord2f(0, 0, 1);
    batch.Vertex3fv(backLeftTop);
    batch.Normal3f(0, 0, -1);
    batch.MultiTexCoord2f(0, 1, 1);
    batch.Vertex3fv(backRightTop);
    batch.Normal3f(0, 0, -1);
    batch.MultiTexCoord2f(0, 1, 0);
    batch.Vertex3fv(backRightBottom);
    batch.Normal3f(0, 0, -1);
    batch.MultiTexCoord2f(0, 0, 0);
    batch.Vertex3fv(backLeftBottom);

    // left face
    batch.Normal3f(-1, 0, 0);
    batch.MultiTexCoord2f(0, 0, 3);
    batch.Vertex3fv(backLeftTop);
    batch.Normal3f(-1, 0, 0);
    batch.MultiTexCoord2f(0, 0, 0);
    batch.Vertex3fv(backLeftBottom);
    batch.Normal3f(-1, 0, 0);
    batch.MultiTexCoord2f(0, 3, 0);
    batch.Vertex3fv(frontLeftBottom);
    batch.Normal3f(-1, 0, 0);
    batch.MultiTexCoord2f(0, 3, 3);
    batch.Vertex3fv(frontLeftTop);

    // right face
    batch.Normal3f(1, 0, 0);
    batch.MultiTexCoord2f(0, 0, 1);
    batch.Vertex3fv(backRightTop);
    batch.Normal3f(1, 0, 0);
    batch.MultiTexCoord2f(0, 1, 1);
    batch.Vertex3fv(frontRightTop);
    batch.Normal3f(1, 0, 0);
    batch.MultiTexCoord2f(0, 1, 0);
    batch.Vertex3fv(frontRightBottom);
    batch.Normal3f(1, 0, 0);
    batch.MultiTexCoord2f(0, 0, 0);
    batch.Vertex3fv(backRightBottom);

    batch.End();
}

void setupRC()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    shaderManager.InitializeStockShaders();
    pipelineTransform.SetMatrixStacks(modelViewM, projectionM);

    generateCuboidBatch(cuboidBatch, cuboidLength, cuboidWidth, cuboidHeigth);

    glGenTextures(2, textureID);
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    LoadTGATexture("stone.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
    LoadTGATexture("black_white.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT);

    if(gltIsExtSupported("GL_EXT_texture_filter_anisotropic")) {
        isAnisotropySupported = true;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
        printf("anisotropy is supported, largest %f\n", largest);
    }
}

void cleanRC()
{
    glDeleteTextures(2, textureID);
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
