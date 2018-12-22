#ifndef SCENEREPROJTEX_H
#define SCENEREPROJTEX_H

#include "scene.h"
#include "glslprogram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbomesh.h"
#include "vboteapot.h"
#include "vbotorus.h"
#include "skybox.h"

#include "Matrix4x4.h"

USING_NS_MATH3D

//投影纹理映射

class SceneProjTex //: public Scene
{
private:
    GLSLProgram prog;

    int width, height;
    VBOTeapot *teapot;
    VBOTorus *torus;
    VBOPlane *plane;
    SkyBox *sky;

    Matrix4x4 model;
    Matrix4x4 view;
    Matrix4x4 projection;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneProjTex();

    void Init();
    void update( float t );
    void Draw();
    void Resize(int, int);
};

#endif // SCENEREFRACTCUBE_H
