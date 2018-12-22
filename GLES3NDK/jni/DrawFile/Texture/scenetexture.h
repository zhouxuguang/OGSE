#ifndef SCENETEXTURE_H
#define SCENETEXTURE_H

#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "Matrix4x4.h"

USING_NS_MATH3D

class SceneTexture
{
private:
    GLSLProgram prog;

    VBOPlane *plane;
    VBOCube *cube;

    Matrix4x4 model;
    Matrix4x4 view;
    Matrix4x4 projection;
    float angle;

    void setMatrices();

public:
    SceneTexture();

    void Init();
    void update( float t );
    void Draw();
    void Resize(int, int);
};

#endif // SCENEPERFRAGMENT_H
