//
//  GLPrecompile.h
//  GLUtilBox
//
//  Created by zhouxuguang on 16/11/11.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#ifndef GLPrecompile_hpp
#define GLPrecompile_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
#include <map>

//if __ANDROID_API__ >= 5  OpenGL ES2.0

#ifdef __ANDROID__

#include <GLES/gl.h>
#include <GLES/glext.h>

#if __ANDROID_API__ >= 18
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
#else
	#define GL_GLEXT_PROTOTYPES

	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>

	#define glGenVertexArrays 		glGenVertexArraysOES
	#define glBindVertexArray 		glBindVertexArrayOES
	#define glDeleteVertexArrays		glDeleteVertexArraysOES
	#define glGenerateMipmap 		glGenerateMipmapOES
	#define	glClearDepth				glClearDepthf
	#define glMapBuffer				glMapBufferOES
	#define glUnmapBuffer			glUnmapBufferOES
	#define GL_DEPTH24_STENCIL8			GL_DEPTH24_STENCIL8_OES
	#define GL_WRITE_ONLY				GL_WRITE_ONLY_OES
#endif

#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#elif TARGET_OS_IPHONE
// iOS device
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#   error "Unknown Apple platform"
#endif



#endif

#endif /* GLPrecompile_hpp */
