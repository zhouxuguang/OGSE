#ifndef SCENENORMALMAP_H
#define SCENENORMALMAP_H

#include "vbomesh.h"
#include "vboplane.h"
#include "vbocube.h"
#include "Matrix4x4.h"

#include "GLSLProgram.h"

USING_NS_MATH3D

class SceneNormalMap
{
private:
    GLSLProgram prog;

    int width, height;
    VBOPlane *plane;
    VBOCube *cube;
    VBOMesh *ogre;

    Matrix4x4 model;
    Matrix4x4 view;
    Matrix4x4 projection;
    float angle;

    void setMatrices();

public:
    SceneNormalMap();

    void Init();
    void Draw();
    void Resize(int, int);
};

#endif // SCENENORMALMAP_H
