#pragma once

#include "GLSLProgram.h"

class UniformShader
{
public:
	UniformShader(void);
	~UniformShader(void);

	void Init();

	void Resize(int width,int height);

	void Draw();

private:
	GLSLProgram mProgram;

	GLuint vaoHandle;

	void InitUniformBlockBuffer();

};

