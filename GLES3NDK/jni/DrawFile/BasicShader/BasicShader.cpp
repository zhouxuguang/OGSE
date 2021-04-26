/*
 * BasicShader.cpp
 *
 *  Created on: 2016-11-19
 *      Author: zhouxuguang
 */

#include "BasicShader.h"
#include "Matrix4x4.h"
#include <stdio.h>

USING_NS_MATH3D

float positionData[] = {
    -0.8f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
    0.0f, 0.8f, 0.0f };

float colorData[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f };

//shader鏂囦欢鐨勫唴瀹�
extern std::string strVert;
extern std::string strFrag;

BasicShader::BasicShader()
{
    mProgram.InitWithShader(strVert.c_str(), strFrag.c_str());
    
    mProgram.BindAttribLocation(0, "VertexPosition");
    mProgram.BindAttribLocation(1, "VertexColor");
    mProgram.LinkProgram();

}

BasicShader::~BasicShader()
{
	// TODO Auto-generated destructor stub
}

void BasicShader::Init()
{
    // Create the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];
    // Populate the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData,GL_STATIC_DRAW);
    // Populate the color buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData,GL_STATIC_DRAW);
    
    // Create and set-up the vertex array object
    glGenVertexArrays( 1, (GLuint*)&vaoHandle );
    glBindVertexArray((GLuint)vaoHandle);
    // Enable the vertex attribute arrays
    glEnableVertexAttribArray(0); // Vertex position
    glEnableVertexAttribArray(1); // Vertex color
    // Map index 0 to the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0,(GLubyte *)NULL );
    // Map index 1 to the color buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0,(GLubyte *)NULL );
}

void BasicShader::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mProgram.Use();
    
    glBindVertexArray((GLuint)vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3 );
}

void BasicShader::Resize(int width, int height)
{
	Matrix4x4 rotateMatrix;
	//Matrix4x4::CreateRotation(0.0f, 0.0f, 1.0f,15.0,rotateMatrix);
	Matrix4x4::CreateTranslate(0,0.2,0,rotateMatrix);
    mProgram.SetUniformMatrix4f("RotationMatrix", 1, true, &rotateMatrix[0][0]);
    
    glViewport(0, 0, width, height);
}
