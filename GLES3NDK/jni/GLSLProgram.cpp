/*
 * GLSLProgram.cpp
 *
 *  Created on: 2016-11-8
 *      Author: zhouxuguang
 */

#include "GLSLProgram.h"

GLuint GLSLProgram::LoadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    
    if (0 == shader)
    {
        return 0;
    }
    
    glShaderSource(shader, 1, &pSource, NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen)
        {
            char* buf = (char*) malloc(infoLen);
            if (buf != NULL)
            {
                glGetShaderInfoLog(shader, infoLen, NULL, buf);
                //LOGE("Could not compile shader %d:\n%s\n",shaderType, buf);
                free(buf);
            }
            glDeleteShader(shader);
            shader = 0;
        }
    }
    
    return shader;
}

bool GLSLProgram::LoadShaderFile(const char *pShaderFile,std::string &shaderString)
{
    FILE *fpin = fopen( pShaderFile, "rb" );
    if( fpin == NULL )
    {
        return false;
    }
    fseek( fpin, 0, SEEK_END );
    long length = (long)ftell( fpin );
    char *buffer = new char[ length ];
    rewind( fpin );
    fread( buffer, length, 1, fpin );
    fclose( fpin );
    
    shaderString = buffer;
    delete []buffer;
    return true;
}

GLSLProgram::GLSLProgram():mVertexShader(0),mFragmentShader(0),mProgram(0)
{
}

GLSLProgram::GLSLProgram(const char* pVertexSource, const char* pFragmentSource)
{
    InitWithShader(pVertexSource,pFragmentSource);
}

GLSLProgram::~GLSLProgram()
{
    if (mProgram)
    {
        glDetachShader(mProgram, mVertexShader);
        glDetachShader(mProgram, mFragmentShader);
        glDeleteShader(mVertexShader);
        mVertexShader = 0;
        glDeleteShader(mFragmentShader);
        mFragmentShader = 0;
        
        glDeleteProgram(mProgram);
        mProgram = 0;
    }
}

void GLSLProgram::InitWithShader(const char *pVertexSource, const char *pFragmentSource)
{
    mVertexShader = GLSLProgram::LoadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!mVertexShader) {
        return;
    }
    
    //__android_log_print(ANDROID_LOG_INFO,"GLES2","content =sss");

    mFragmentShader = GLSLProgram::LoadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!mFragmentShader) {
        return;
    }
    
    mProgram = glCreateProgram();
}

void GLSLProgram::InitWithShader(const char *pComputeSource)
{
    mComputeShader = GLSLProgram::LoadShader(GL_VERTEX_SHADER, pComputeSource);
    if (NULL == pComputeSource)
    {
        return;
    }
}

void GLSLProgram::LinkProgram()
{
    if (mProgram) {
        glAttachShader(mProgram, mVertexShader);
        glAttachShader(mProgram, mFragmentShader);
        glLinkProgram(mProgram);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(mProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(mProgram, bufLength, NULL, buf);
                    free(buf);
                }
            }
            glDeleteProgram(mProgram);
            mProgram = 0;
        }
    }
}

void GLSLProgram::LoadProgramBinary(const char * fileName, GLenum format)
{
	FILE *fpin = fopen( fileName, "rb" );
	if( fpin == NULL )
	{
		fprintf( stderr, "Cannot open input GLSL binary file '%s'\n", fileName );
		return;
	}
	fseek( fpin, 0, SEEK_END );
	GLint length = (GLint)ftell( fpin );
	GLubyte *buffer = new GLubyte[ length ];
	rewind( fpin );
	fread( buffer, length, 1, fpin );
	fclose( fpin );

	glProgramBinary( this->mProgram, format, buffer, length );
	delete [] buffer;

	GLint   success;
	glGetProgramiv( this->mProgram, GL_LINK_STATUS, &success );

	if( !success )
	{
		fprintf( stderr, "Did not successfully load the GLSL binary file '%s'\n", fileName );
		return;
	}
}

void GLSLProgram::Use()
{
	glUseProgram(mProgram);
}

void GLSLProgram::UseFixedFunction()
{
	glUseProgram(0);
}

void GLSLProgram::BindAttribLocation (unsigned int index, const char *name)
{
	glBindAttribLocation(mProgram,(GLuint)index,(const GLchar*)name);
}

void GLSLProgram::BindFragDataLocation(unsigned int index, const char *name)
{
    //只有桌面版本支持
}

GLint GLSLProgram::GetAttributeLocation(const char* attrName)
{
	std::map<const char *, int>::iterator iter = mAttributeLocs.find(attrName);
	if(iter == mAttributeLocs.end())
	{
		mAttributeLocs[attrName] = glGetAttribLocation(mProgram, attrName);
	}
	return mAttributeLocs[attrName];
}

GLint GLSLProgram::GetUniformLocation(const char* uniName)
{
	std::map<const char*, int>::iterator iter = mUniformLocs.find(uniName);
	if(iter == mUniformLocs.end())
	{
		mUniformLocs[uniName] = glGetUniformLocation(mProgram, uniName);
	}
	return mUniformLocs[uniName];
}

GLint GLSLProgram::GetUniformBlockIndex(const char* uniName)
{
	return glGetUniformBlockIndex(mProgram, uniName);
}

//void GLSLProgram::SetAttributeVariable( char* name, int val )
//{
//	int loc;
//	if( ( loc = GetAttributeLocation( name ) )  >= 0 )
//	{
//		this->Use();
//		glVertexAttrib1i( loc, val );
//	}
//};

void GLSLProgram::SetAttributeVariable( const char* name, float val )
{
	int loc;
	if( ( loc = GetAttributeLocation( name ) )  >= 0 )
	{
		this->Use();
		glVertexAttrib1f( loc, val );
	}
}

void GLSLProgram::SetAttributeVariable( const char* name, float val0, float val1, float val2 )
{
	int loc;
	if( ( loc = GetAttributeLocation( name ) )  >= 0 )
	{
		this->Use();
		glVertexAttrib3f( loc, val0, val1, val2 );
	}
}

void GLSLProgram::SetAttributeVariable( const char* name, float vals[3] )
{
	int loc;
	if( ( loc = GetAttributeLocation( name ) )  >= 0 )
	{
		this->Use();
		glVertexAttrib3fv( loc, vals );
	}
}

void GLSLProgram::SetUniformVariable( const char* name, int val )
{
	int loc;
	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniform1i( loc, val );
	}
}


void GLSLProgram::SetUniformVariable( const char* name, float val )
{
	int loc;
	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniform1f( loc, val );
	}
}

void GLSLProgram::SetUniformVariable( const char* name, float val0, float val1, float val2 )
{
	int loc;
	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniform3f( loc, val0, val1, val2 );
	}
}

void GLSLProgram::SetUniformVariable( const char* name, float vals[3] )
{
	int loc;

	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniform3fv( loc, 3, vals );
	}
}

void GLSLProgram::SetUniformVariable( const char* name,
							float val0, float val1, float val2, float val3 )
{
	int loc;

	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniform4f( loc, val0,val1, val2, val3 );
	}
}

void GLSLProgram::SetUniformMatrix2f(const char* name ,int count, bool transpose, const float *value)
{
	int loc;

	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniformMatrix2fv( loc, count, transpose, value );
	}
}

void GLSLProgram::SetUniformMatrix3f(const char* name ,int count, bool transpose, const float *value)
{
	int loc;

	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniformMatrix3fv( loc, count, transpose, value );
	}
}

void GLSLProgram::SetUniformMatrix4f(const char* name ,int count, bool transpose, const float *value)
{
	int loc;

	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniformMatrix4fv( loc, count, transpose, value );
	}
}

void GLSLProgram::GetActiveUniformBlockiv(const char* uniName, GLenum pname, GLint *params)
{
	int loc;
	if ( (loc = GetUniformBlockIndex(uniName)) >= 0)
	{
		this->Use();
		glGetActiveUniformBlockiv(mProgram,loc,pname,params);
	}
}

void GLSLProgram::GetUniformIndices(GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices)
{
	glGetUniformIndices(mProgram,uniformCount,uniformNames,uniformIndices);
}

void GLSLProgram::GetActiveUniformsiv(GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params)
{
	glGetActiveUniformsiv(mProgram,uniformCount,uniformIndices,pname,params);
}

