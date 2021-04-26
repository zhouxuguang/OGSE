#include "NDKGL2.h"
#include "GLSLProgram.h"

#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "Vector3.h"

USING_NS_MATH3D

#ifdef __ANDROID__

#include <android/log.h>

#define  LOG_TAG    "GLES2"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    //LOGI("GL %s = %s\n", name, v);
    GLchar  a = 0;
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

GLSLProgram *gProgram = NULL;

float _rotation = 0;
GLuint _vertexArray;
GLuint _vertexBuffer;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Uniform index.
enum
{
    UNIFORM_MODELVIEWPROJECTION_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// Attribute index.
enum
{
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    NUM_ATTRIBUTES
};

GLfloat gCubeVertexData[216] =
{
    // Data layout for each line below is:
    // positionX, positionY, positionZ,     normalX, normalY, normalZ,
    0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,          1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,         1.0f, 0.0f, 0.0f,

    0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 1.0f, 0.0f,

    -0.5f, 0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, -1.0f, 0.0f,

    0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f,

    0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 0.0f, -1.0f
};

Matrix4x4 modelViewProjectionMatrix;
Matrix3x3 normalMatrix;

std::string strVert;
std::string strFrag;

void OnDrawGlFrame()
{
	LOGI("OnDrawGlFrame begin");
	glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
	LOGI("OnDrawGlFrame glClearColor");
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	LOGI("OnDrawGlFrame glClear");

	glBindVertexArray(_vertexArray);

	LOGI("Use();  modelViewProjectionMatrix");

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Render the object again with ES2
	gProgram->Use();

	LOGI("Use();  modelViewProjectionMatrix");

	gProgram->SetUniformMatrix4f("modelViewProjectionMatrix",1,1,(const float*)&modelViewProjectionMatrix[0][0]);
	LOGI("SetUniformMatrix4f  modelViewProjectionMatrix");
	gProgram->SetUniformMatrix3f("normalMatrix",1,1,(const float*)&normalMatrix[0][0]);
	LOGI("SetUniformMatrix3f  normalMatrix");

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void OnResize(int width, int height)
{
	LOGI("OnResize(%d, %d)", width, height);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float aspect = fabs(width / (float)height);
    Matrix4x4 projectionMatrix;
    Matrix4x4::CreatePerspective(65.0f,aspect,0.1f,100.0f,projectionMatrix);

    LOGI("1-----Matrix4x4::CreatePerspective(65.0f,aspect,0.1f,100.0f,projectionMatrix);");

    Matrix4x4 baseModelViewMatrix;
    Matrix4x4::CreateTranslate(0.0f, 0.0f, -4.0f,baseModelViewMatrix);

    LOGI("2-----Matrix4x4::CreateTranslate(0.0f, 0.0f, -4.0f,baseModelViewMatrix);");

    Matrix4x4 rotateMatrix;
    Matrix4x4::CreateRotation(0.0f, 1.0f, 0.0f,_rotation,rotateMatrix);
    baseModelViewMatrix = baseModelViewMatrix * rotateMatrix;

    LOGI("3-----Matrix4x4::CreateRotation(0.0f, 1.0f, 0.0f,_rotation,rotateMatrix);");

     //Compute the model view matrix for the object rendered with GLKit
    Matrix4x4 tranMatrix;
    Matrix4x4::CreateTranslate(0.0f, 0.0f, -1.5f,tranMatrix);
    LOGI("4-----Matrix4x4::CreateTranslate(0.0f, 0.0f, -1.5f,tranMatrix);");
    Matrix4x4::CreateRotation(1.0f, 1.0f, 1.0f,_rotation,rotateMatrix);
    LOGI("5-----Matrix4x4::CreateRotation(1.0f, 1.0f, 1.0f,_rotation,rotateMatrix);");
    Matrix4x4 modelViewMatrix = rotateMatrix * tranMatrix;

    LOGI("6-----Matrix4x4 modelViewMatrix = rotateMatrix * tranMatrix;");
    modelViewMatrix = baseModelViewMatrix * modelViewMatrix;

    // Compute the model view matrix for the object rendered with ES2
    Matrix4x4::CreateTranslate(0.0f, 0.0f, 1.5f,tranMatrix);
    Matrix4x4::CreateRotation(1.0f, 1.0f, 1.0f,_rotation,rotateMatrix);

    LOGI("7-----Matrix4x4::CreateRotation(1.0f, 1.0f, 1.0f,_rotation,rotateMatrix);");
    modelViewMatrix = rotateMatrix * tranMatrix;
    modelViewMatrix = baseModelViewMatrix * modelViewMatrix;

    normalMatrix = modelViewMatrix.GetMatrix3().Inverse().Transpose();

    LOGI("8-----normalMatrix = modelViewMatrix.GetMatrix3().Inverse().Transpose();");

    modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;

    _rotation += 10.f;

    LOGI("9----- _rotation += 10.f;");

	glViewport(0, 0, width, height);

	LOGI("10-----glViewport(0, 0, width, height);");
	checkGlError("glViewport");
}

void OnInited()
{
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

	gProgram = new GLSLProgram(strVert.c_str(), strFrag.c_str());

	gProgram->BindAttribLocation(0, "position");
	gProgram->BindAttribLocation(1, "normal");

	gProgram->LinkProgram();

	LOGI("OnInited()");

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &_vertexArray);
    glBindVertexArray(_vertexArray);

    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gCubeVertexData), gCubeVertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(12));

    glBindVertexArray(0);
}
