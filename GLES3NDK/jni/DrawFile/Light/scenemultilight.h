#ifndef SCENEMULTILIGHT_H
#define SCENEMULTILIGHT_H

#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbomesh.h"

#include "Matrix4x4.h"

USING_NS_MATH3D


class SceneMultiLight
{
private:
    GLSLProgram prog;
    VBOPlane *plane;
    VBOMesh * mesh;

    Matrix4x4 model;
    Matrix4x4 view;
    Matrix4x4 projection;
    
    void setMatrices();

public:
    SceneMultiLight();

    void Init();
    void Draw();
    void Resize(int, int);
};

#endif // SCENEMULTILIGHT_H
