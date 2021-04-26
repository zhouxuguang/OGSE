/*
 * BasicShader.h
 *
 *  Created on: 2016-11-19
 *      Author: zhouxuguang
 */

#ifndef BASICSHADER_H_
#define BASICSHADER_H_

#include "../../GLSLProgram.h"

class BasicShader
{
public:
	BasicShader();
	virtual ~BasicShader();
    
    void Init();
    
    void Resize(int width,int height);
    
    void Draw();
    
private:
    GLuint vaoHandle;
    
    GLSLProgram mProgram;
    
};

#endif /* BASICSHADER_H_ */
