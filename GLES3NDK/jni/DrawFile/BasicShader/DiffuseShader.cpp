/*
 * DiffuseShader.cpp
 *
 *  Created on: 2016-11-20
 *      Author: zxg
 */

#include "DiffuseShader.h"
#include "../../vbotorus.h"
#include "Vector4.h"

extern std::string strVert;
extern std::string strFrag;


DiffuseShader::DiffuseShader()
{
	// TODO Auto-generated constructor stub
	mProgram.InitWithShader(strVert.c_str(), strFrag.c_str());

	mProgram.LinkProgram();
}

DiffuseShader::~DiffuseShader()
{
	// TODO Auto-generated destructor stub
}

void DiffuseShader::Init()
{
	glClearColor(0.0,0.0,0.0,1.0);
	glEnable(GL_DEPTH_TEST);

	torus = new VBOTorus(0.7f, 0.3f, 30, 30);

	Matrix4x4::CreateRotationX(-35.0f,model);

	Matrix4x4 modelTemp;
	Matrix4x4::CreateRotationY(35.0f,modelTemp);
	model *= modelTemp;

	Matrix4x4::CreateScale(1.0,1.0,1.0,modelTemp);
	model *= modelTemp;

	Matrix4x4::CreateLookAt(Vector3(0.0f,0.0f,2.6f),Vector3(0.0f,0.0f,0.0f),
			Vector3(0.0f,1.0f,0.0f),view);

	projection = Matrix4x4::IDENTITY;

	mProgram.SetUniformVariable("Kd", 0.9f, 0.5f, 0.3f);
	mProgram.SetUniformVariable("Ld", 1.0f, 1.0f, 1.0f);

	//设置灯光位置
	Vector4 vec4(0,0,0,0);
	vec4 = view * Vector4(5.0f,5.0f,2.0f,1.0f);
	mProgram.SetUniformVariable("LightPosition", vec4.x,vec4.y,vec4.z,vec4.w );
}

void DiffuseShader::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setMatrices();
	torus->render();
}

void DiffuseShader::Resize(int width,int height)
{
	glViewport(0,0,width,height);
	Matrix4x4::CreatePerspective(70.0f,(Real)width/height,0.3f,100.f,projection);
}

void DiffuseShader::setMatrices()
{
	Matrix4x4 mv = view * model;
	mProgram.SetUniformMatrix4f("ModelViewMatrix", 1, true, &mv[0][0]);

	Matrix3x3 matNormal = mv.GetMatrix3().Inverse().Transpose();
	mProgram.SetUniformMatrix3f("NormalMatrix",1, true, &matNormal[0][0]);
	mProgram.SetUniformMatrix4f("MVP", 1, true, &(projection * mv)[0][0]);
}

