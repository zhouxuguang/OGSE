#ifndef VBOTEAPOT_H
#define VBOTEAPOT_H

#include "Vector3.h"
#include "Matrix4x4.h"
#include "Matrix3x3.h"

USING_NS_MATH3D


class VBOTeapot
{
private:
    unsigned int vaoHandle;
    unsigned int faces;

    void generatePatches(float * v, float * n, float *tc, unsigned int* el, int grid);
    void buildPatchReflect(int patchNum,
                           float *B, float *dB,
                           float *v, float *n, float *, unsigned int *el,
                           int &index, int &elIndex, int &, int grid,
                           bool reflectX, bool reflectY);
    void buildPatch(Vector3 patch[][4],
                    float *B, float *dB,
                    float *v, float *n,float *, unsigned int *el,
                    int &index, int &elIndex, int &, int grid, Matrix3x3 reflect, bool invertNormal);
    void getPatch( int patchNum, Vector3 patch[][4], bool reverseV );

    void computeBasisFunctions( float * B, float * dB, int grid );
    Vector3 evaluate( int gridU, int gridV, float *B, Vector3 patch[][4] );
    Vector3 evaluateNormal( int gridU, int gridV, float *B, float *dB, Vector3 patch[][4] );
    void moveLid(int,float *,Matrix4x4);

public:
    VBOTeapot(int grid, Matrix4x4 lidTransform);

    void render() const;
};

#endif // VBOTEAPOT_H
