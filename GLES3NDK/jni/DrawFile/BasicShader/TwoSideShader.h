//
//  TwoSideShader.h
//  GLUtilBox
//
//  Created by zhouxuguang on 16/11/29.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#ifndef TwoSideShader_hpp
#define TwoSideShader_hpp

#include "GLSLProgram.h"
#include "Matrix4x4.h"

using namespace math3d;

class VBOTeapot;

class TwoSideShader
{
public:
    TwoSideShader();
    virtual ~TwoSideShader();
    
    void Init();
    
    void Resize(int width,int height);
    
    void Draw();
    
private:
    GLuint vaoHandle;
    
    GLSLProgram mProgram;
    
    VBOTeapot *teapot;
    float angle;
    
    Matrix4x4 model;
    Matrix4x4 view;
    Matrix4x4 projection;
    
};

#endif /* TwoSideShader_hpp */
