/*
 * GLSLProgram.h
 *
 *  Created on: 2016-11-8
 *      Author: zhouxuguang
 */

#ifndef GLSLPROGRAM_H_
#define GLSLPROGRAM_H_

#include "GLPrecompile.h"

class GLSLProgram
{
public:
    GLSLProgram();
    
	GLSLProgram(const char* pVertexSource, const char* pFragmentSource);
    
	~GLSLProgram();
    
    void InitWithShader(const char* pVertexSource, const char* pFragmentSource);
    
    void InitWithShader(const char* pComputeSource);
    
    //编译和连接程序
    void LinkProgram();

	//加载二进制shader程序
	void LoadProgramBinary( const char * fileName, GLenum format );

	//使用程序对象
	void Use();

	//使用固定管线
	void UseFixedFunction();

	//绑定属性变量
	void BindAttribLocation (unsigned int index, const char *name);
    
    void BindFragDataLocation(unsigned int index , const char *name);

	//获得属性变量
	GLint GetAttributeLocation(const char* attrName);

	//获得指定Uniform变量的位置
	GLint GetUniformLocation( const char* uniName);

	GLint GetUniformBlockIndex( const char* uniName);

	//设置属性变量的值
	//void	 SetAttributeVariable( char *, int );
	void SetAttributeVariable( const char *, float );
	void SetAttributeVariable( const char *, float, float, float );
	void SetAttributeVariable( const char *, float[3] );

	//设置uniform变量的值
	void SetUniformVariable( const char *, int );
	void SetUniformVariable( const char *, float );
	void SetUniformVariable( const char *, float, float, float );
	void SetUniformVariable( const char *, float[3] );
	void SetUniformVariable( const char *, float, float, float, float );

	void SetUniformMatrix2f(const char* name,int count, bool transpose, const float *value);
	void SetUniformMatrix3f(const char* name,int count, bool transpose, const float *value);
	void SetUniformMatrix4f(const char* name,int count, bool transpose, const float *value);

	void GetActiveUniformBlockiv(const char* uniName, GLenum pname, GLint *params);

	void GetUniformIndices ( GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);

	void GetActiveUniformsiv ( GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
    
    //加载shader
    static GLuint LoadShader(GLenum shaderType, const char* pSource);
    
    static bool LoadShaderFile(const char* pShaderFile,std::string &shaderString);

private:
	GLuint mProgram;		//程序对象
    GLuint mVertexShader;      //顶点shader
    GLuint mFragmentShader;     //片段shader
    GLuint mComputeShader;      //计算shader

	std::map<const char *, int>	mAttributeLocs;	//attribute属性变量的map
	std::map<const char *, int> mUniformLocs;	//Uniform变量的map
};

#endif /* GLSLPROGRAM_H_ */
