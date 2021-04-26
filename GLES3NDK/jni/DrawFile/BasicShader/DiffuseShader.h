/*
 * DiffuseShader.h
 *
 *  Created on: 2016-11-20
 *      Author: zxg
 */

#ifndef DIFFUSESHADER_H_
#define DIFFUSESHADER_H_

#include "GLSLProgram.h"

#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Vector3.h"

USING_NS_MATH3D

class VBOTorus;
class DiffuseShader
{
public:
	DiffuseShader();
	virtual ~DiffuseShader();

    void Init();

    void Resize(int width,int height);

    void Draw();

private:
    GLuint vaoHandle;

    GLSLProgram mProgram;

    Matrix4x4 model;
    Matrix4x4 view;
    Matrix4x4 projection;

	VBOTorus *torus;

	void setMatrices();
};

#endif /* DIFFUSESHADER_H_ */
