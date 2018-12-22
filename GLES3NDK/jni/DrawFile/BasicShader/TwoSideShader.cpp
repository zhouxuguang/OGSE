//
//  TwoSideShader.cpp
//  GLUtilBox
//
//  Created by zhouxuguang on 16/11/29.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#include "TwoSideShader.h"
#include "vboteapot.h"
#include "Vector4.h"

extern std::string strVert;
extern std::string strFrag;

TwoSideShader::TwoSideShader() : angle(0.0f)
{
    mProgram.InitWithShader(strVert.c_str(), strFrag.c_str());
    mProgram.LinkProgram();
}

TwoSideShader::~TwoSideShader()
{
    delete teapot;
}

void TwoSideShader::Init()
{
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    
    Matrix4x4 transform;
    Matrix4x4::CreateTranslate(Vector3(0.0f,1.5f,0.25f), transform);
    teapot = new VBOTeapot(13, transform);
    
    Matrix4x4::CreateLookAt(Vector3(2.0f,4.0f,3.8f), Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,1.0f,0.0f),view);
    
    mProgram.SetUniformVariable("Material.Kd", 0.9f, 0.5f, 0.3f);
    mProgram.SetUniformVariable("Light.Ld", 1.0f, 1.0f, 1.0f);
    mProgram.SetUniformVariable("Material.Ka", 0.9f, 0.5f, 0.3f);
    mProgram.SetUniformVariable("Light.La", 0.4f, 0.4f, 0.4f);
    mProgram.SetUniformVariable("Material.Ks", 0.8f, 0.8f, 0.8f);
    mProgram.SetUniformVariable("Light.Ls", 1.0f, 1.0f, 1.0f);
    mProgram.SetUniformVariable("Material.Shininess", 100.0f);
    
    //glProvokingVertexOES(GL_FIRST_VERTEX_CONVENTION_OES);
}

void TwoSideShader::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Vector4 worldLight = Vector4(2.0f,4.0f,2.0f,1.0f);
    Matrix4x4::CreateRotation(Vector3(0.0f,1.0f,0.0f), angle, model);
    Vector4 viewLight = view * model * worldLight;
    mProgram.SetUniformVariable("Light.Position", viewLight.x,viewLight.y,viewLight.z,viewLight.w );
    
    model = Matrix4x4::IDENTITY;
    Matrix4x4 matTemp;
    Matrix4x4::CreateTranslate(Vector3(0.0,-1.0,0.0), matTemp);
    Matrix4x4::CreateRotation(Vector3(1.0f,0.0f,0.0f), -90.0f, matTemp);
    
    
    Matrix4x4 mv = view * model;
    mProgram.SetUniformMatrix4f("ModelViewMatrix", 1, true, &mv[0][0]);
    Matrix3x3 matNormal = mv.GetMatrix3().Inverse().Transpose();
    mProgram.SetUniformMatrix3f("NormalMatrix", 1, true, &matNormal[0][0]);
    mProgram.SetUniformMatrix4f("MVP", 1, true, &(projection * mv)[0][0]);
    
    teapot->render();
}

void TwoSideShader::Resize(int width, int height)
{
    glViewport(0,0,width,height);
    Matrix4x4::CreatePerspective(70.0f, (float)width/height, 0.3f, 100.0f, projection);
}
