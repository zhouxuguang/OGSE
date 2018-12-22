#ifndef SCENESHADOW_INCLUDE
#define SCENESHADOW_INCLUDE

#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbotorus.h"
#include "vboteapot.h"

#include "Matrix4x4.h"

USING_NS_MATH3D


class SceneShadowMap 
{
private:
	GLSLProgram prog, solidProg;
	GLuint shadowFBO;

	VBOTeapot *teapot;
	VBOPlane *plane;
	VBOTorus *torus;

	int width, height;
	int shadowMapWidth, shadowMapHeight;

	Matrix4x4 model, view, projection;
	Matrix4x4 lightPV;
	Matrix4x4 shadowBias;
	float angle;

	void setMatrices(GLSLProgram &prog);
	void compileAndLinkShader();
	void setupFBO();
	void drawScene(GLSLProgram &prog);
	void spitOutDepthBuffer();

public:
	SceneShadowMap();

	void Init();
	void update( float t );
	void Draw();
	void Resize(int, int);
};

#endif