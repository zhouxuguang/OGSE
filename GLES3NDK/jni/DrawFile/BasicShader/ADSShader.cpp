//
//  ADSShader.cpp
//  GLUtilBox
//
//  Created by zhouxuguang on 16/11/21.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#include "ADSShader.h"
#include "vbotorus.h"
#include "Vector4.h"

extern std::string strVert;
extern std::string strFrag;

ADSShader::ADSShader()
{
    mProgram.InitWithShader(strVert.c_str(), strFrag.c_str());
    
    mProgram.LinkProgram();
}

ADSShader::~ADSShader()
{
    if(torus != NULL)
    {
    		delete torus;
    		torus = NULL;
    }
}

void ADSShader::Init()
{
    glClearColor(0.0,0.0,0.0,1.0);
    glEnable(GL_DEPTH_TEST);
    //glProvokingVertex(0);
    
    torus = new VBOTorus(0.7f, 0.3f, 50, 50);
    
    Matrix4x4::CreateRotation(1.0, 0, 0, -35.0, model);
    Matrix4x4 modelTmp;
    Matrix4x4::CreateRotation(0.0, 1.0, 0, 35.0, modelTmp);
    model *= modelTmp;
    
    Matrix4x4::CreateLookAt(Vector3(0.0f,0.0f,2.8f), Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,1.0f,0.0f),view);
    
    Vector4 worldLight = Vector4(5.0f,5.0f,2.0f,1.0f);
    
    mProgram.SetUniformVariable("Material.Kd", 0.9f, 0.5f, 0.3f);
    mProgram.SetUniformVariable("Light.Ld", 1.0f, 1.0f, 1.0f);
    Vector4 lightView = view * worldLight;
    mProgram.SetUniformVariable("Light.Position", lightView.x,lightView.y,lightView.z,lightView.w );
    mProgram.SetUniformVariable("Material.Ka", 0.9f, 0.5f, 0.3f);
    mProgram.SetUniformVariable("Light.La", 0.4f, 0.4f, 0.4f);
    mProgram.SetUniformVariable("Material.Ks", 0.8f, 0.8f, 0.8f);
    mProgram.SetUniformVariable("Light.Ls", 1.0f, 1.0f, 1.0f);
    mProgram.SetUniformVariable("Material.Shininess", 100.0f);
}

void ADSShader::Resize(int width, int height)
{
    glViewport(0,0,width,height);
    Matrix4x4::CreatePerspective(70.0, (Real)width/height, 0.3f, 100.0f, projection);

    Matrix4x4 mv = view * model;
    mProgram.SetUniformMatrix4f("ModelViewMatrix", 1, true, (const float*)&mv);
    Matrix3x3 normalMat = mv.GetMatrix3().Inverse().Transpose();
    mProgram.SetUniformMatrix3f("NormalMatrix", 1, true, (const float*)&normalMat);
    mProgram.SetUniformMatrix4f("MVP", 1, true, &(projection * mv)[0][0]);
}

void ADSShader::Draw()
{
    glClear((GLuint)(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    torus->render();
}
