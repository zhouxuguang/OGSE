//
//  RenderEngineDefine.hpp
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/5.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef RenderEngineDefine_hpp
#define RenderEngineDefine_hpp

#ifdef __ANDROID__
#define RENDERENGINE_ANDROID_PLATFORM

#elif _WIN32
#define RENDERENGINE_WINDOWS_PLATFORM

#elif __APPLE__

#include <TargetConditionals.h>

#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#define RENDERENGINE_IOS_SIMULATOR_PLATFORM

#elif TARGET_OS_IPHONE
// iOS device
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#define RENDERENGINE_IOS_PLATFORM

#elif TARGET_OS_MAC
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>

#define RENDERENGINE_MACOSX_PLATFORM

#else
#   error "Unknown Apple platform"
#endif

#endif

#endif /* RenderEngineDefine_hpp */
