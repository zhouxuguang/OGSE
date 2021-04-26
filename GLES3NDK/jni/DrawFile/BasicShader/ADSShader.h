//
//  ADSShader.hpp
//  GLUtilBox
//
//  Created by zhouxuguang on 16/11/21.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#ifndef ADSShader_hpp
#define ADSShader_hpp

#include "GLSLProgram.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Vector3.h"

USING_NS_MATH3D

class VBOTorus;

class ADSShader
{
public:
    ADSShader();
    virtual ~ADSShader();
    
    void Init();
    
    void Resize(int width,int height);
    
    void Draw();
    
private:
    VBOTorus *torus;
    float angle;
    
    Matrix4x4 model;
    Matrix4x4 view;
    Matrix4x4 projection;
    
    void setMatrices();
    
    GLSLProgram mProgram;
    
};

#endif /* ADSShader_hpp */
