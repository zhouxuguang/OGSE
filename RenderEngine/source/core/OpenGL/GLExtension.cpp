//
//  GLExtension.cpp
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/12.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#include "GLExtension.h"

#if defined(WIN32)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif // WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
#include <windows.h>
#elif defined(__APPLE__)
// The NS*Symbol* stuff found in <mach-o/dyld.h> is deprecated.
// Since 10.3 (Panther) OS X has provided the dlopen/dlsym/dlclose
// family of functions under <dlfcn.h>. Since 10.4 (Tiger), Apple claimed
// the dlfcn family was significantly faster than the NS*Symbol* family.
// Since 'deprecated' needs to be taken very seriously with the
// coming of 10.5 (Leopard), it makes sense to use the dlfcn family when possible.
#include <AvailabilityMacros.h>
#if !defined(MAC_OS_X_VERSION_10_3) || (MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_3)
#define USE_APPLE_LEGACY_NSSYMBOL
#include <mach-o/dyld.h>
#else
#include <dlfcn.h>
#endif
#elif defined(__EMSCRIPTEN__)
// Emscripten ships EGL, which we use to get OpenGL function addresses.
#include <EGL/egl.h>
#else
#include <dlfcn.h>
#endif


#ifdef OSG_GL_LIBRARY_STATIC

#include "GLStaticLibrary.h"

void* GetGLExtensionFuncPtr(const char *funcName)
{
    //return GLStaticLibrary::getProcAddress(funcName);
    return NULL;
}

#else

void* GetGLExtensionFuncPtr(const char *funcName)
{
#if defined(__ANDROID__)
#if defined(OSG_GLES1_AVAILABLE)
    static void *handle = dlopen("libGLESv1_CM.so", RTLD_NOW);
#elif defined(OSG_GLES2_AVAILABLE)
    static void *handle = dlopen("libGLESv2.so", RTLD_NOW);
#elif defined(OSG_GLES3_AVAILABLE)
    static void *handle = dlopen("libGLESv3.so", RTLD_NOW);
#elif defined(OSG_GL1_AVAILABLE)
    static void *handle = dlopen("libGL.so", RTLD_NOW);
#endif
    return dlsym(handle, funcName);
    
#elif defined(WIN32)
    
#if defined(OSG_GLES2_AVAILABLE)
    static HMODULE hmodule = GetModuleHandle(TEXT("libGLESv2.dll"));
    return convertPointerType<void*, PROC>(GetProcAddress(hmodule, funcName));
#elif defined(OSG_GLES1_AVAILABLE)
    static HMODULE hmodule = GetModuleHandleA(TEXT("libgles_cm.dll"));
    return convertPointerType<void*, PROC>(GetProcAddress(hmodule, funcName));
#else
    return convertPointerType<void*, PROC>(wglGetProcAddress(funcName));
#endif
    
#elif defined(__APPLE__)
    
#if defined(USE_APPLE_LEGACY_NSSYMBOL)
    std::string temp( "_" );
    temp += funcName;    // Mac OS X prepends an underscore on function names
    if ( NSIsSymbolNameDefined( temp.c_str() ) )
    {
        NSSymbol symbol = NSLookupAndBindSymbol( temp.c_str() );
        return NSAddressOfSymbol( symbol );
    }
    else
        return NULL;
#else
    // I am uncertain of the correct and ideal usage of dlsym here.
    // On the surface, it would seem that the FreeBSD implementation
    // would be the ideal one to copy, but ELF and Mach-o are different
    // and Apple's man page says the following about using RTLD_DEFAULT:
    // "This can be a costly search and should be avoided."
    // The documentation mentions nothing about passing in 0 so I must
    // assume the behavior is undefined.
    // So I could try copying the Sun method which I think all this
    // actually originated from.
    
    // return dlsym( RTLD_DEFAULT, funcName );
    static void *handle = dlopen((const char *)0L, RTLD_LAZY);
    return dlsym(handle, funcName);
#endif
    
#elif defined (__sun)
    
    static void *handle = dlopen((const char *)0L, RTLD_LAZY);
    return dlsym(handle, funcName);
    
#elif defined (__sgi)
    
    static void *handle = dlopen((const char *)0L, RTLD_LAZY);
    return dlsym(handle, funcName);
    
#elif defined (__FreeBSD__)
    
    return dlsym( RTLD_DEFAULT, funcName );
    
#elif defined (__linux__)
    
    return dlsym(0, funcName);
    
#elif defined (__QNX__)
    
    return dlsym(RTLD_DEFAULT, funcName);
    
#elif defined(__EMSCRIPTEN__)
    // Use EGL to get OpenGL function address for Emscripten.
    return convertPointerType<void*, __eglMustCastToProperFunctionPointerType>(eglGetProcAddress(funcName));
    
#else // all other unixes
    
    return dlsym(0, funcName);
    
#endif
}
#endif

///////////////////////////////////////////////////////////////////////////
// Extension function pointers for OpenGL v2.x


//GLExtensions::GLExtensions(unsigned int in_contextID):
//contextID(in_contextID)
//{
//    const char* versionString = (const char*) glGetString( GL_VERSION );
//    bool validContext = versionString!=0;
//    if (!validContext)
//    {
//        OSG_NOTIFY(osg::FATAL)<<"Error: OpenGL version test failed, requires valid graphics context."<<std::endl;
//    }
//    
//    glVersion = validContext ? findAsciiToFloat( versionString ) : 0.0f;
//    glslLanguageVersion = 0.0f;
//    
//    bool shadersBuiltIn = OSG_GLES2_FEATURES || OSG_GLES3_FEATURES || OSG_GL3_FEATURES;
//    
//    isShaderObjectsSupported = validContext && (shadersBuiltIn || osg::isGLExtensionSupported(contextID,"GL_ARB_shader_objects"));
//    isVertexShaderSupported = validContext && (shadersBuiltIn || osg::isGLExtensionSupported(contextID,"GL_ARB_vertex_shader"));
//    isFragmentShaderSupported = validContext && (shadersBuiltIn || osg::isGLExtensionSupported(contextID,"GL_ARB_fragment_shader"));
//    isLanguage100Supported = validContext && (shadersBuiltIn || osg::isGLExtensionSupported(contextID,"GL_ARB_shading_language_100"));
//    isGeometryShader4Supported = validContext && (osg::isGLExtensionSupported(contextID,"GL_EXT_geometry_shader4") || osg::isGLExtensionSupported(contextID,"GL_OES_geometry_shader") || osg::isGLExtensionOrVersionSupported(contextID,"GL_ARB_geometry_shader4", 3.2f));
//    isGpuShader4Supported = validContext && osg::isGLExtensionOrVersionSupported(contextID,"GL_EXT_gpu_shader4", 3.0f);
//    areTessellationShadersSupported = validContext && (osg::isGLExtensionOrVersionSupported(contextID, "GL_ARB_tessellation_shader", 4.0f) || osg::isGLExtensionSupported(contextID,"GL_OES_tessellation_shader"));
//    isUniformBufferObjectSupported = validContext && osg::isGLExtensionOrVersionSupported(contextID,"GL_ARB_uniform_buffer_object", 3.1f);
//    isGetProgramBinarySupported = validContext && osg::isGLExtensionOrVersionSupported(contextID,"GL_ARB_get_program_binary", 4.1f);
//    isGpuShaderFp64Supported = validContext && osg::isGLExtensionOrVersionSupported(contextID,"GL_ARB_gpu_shader_fp64", 4.0f);
//    isShaderAtomicCountersSupported = validContext && osg::isGLExtensionOrVersionSupported(contextID,"GL_ARB_shader_atomic_counters", 4.2f);
//    
//    isRectangleSupported = validContext &&
//    (OSG_GL3_FEATURES ||
//     isGLExtensionSupported(contextID,"GL_ARB_texture_rectangle") ||
//     isGLExtensionSupported(contextID,"GL_EXT_texture_rectangle") ||
//     isGLExtensionSupported(contextID,"GL_NV_texture_rectangle"));
//    
//    isCubeMapSupported = validContext &&
//    (OSG_GLES2_FEATURES || OSG_GLES3_FEATURES || OSG_GL3_FEATURES ||
//     isGLExtensionSupported(contextID,"GL_ARB_texture_cube_map") ||
//     isGLExtensionSupported(contextID,"GL_EXT_texture_cube_map") ||
//     (glVersion >= 1.3f));
//    
//    isClipControlSupported = validContext &&
//    (isGLExtensionSupported(contextID,"GL_ARB_clip_control") ||
//     (glVersion >= 4.5f));
//    
//    
//    isGlslSupported = validContext &&
//    (( glVersion >= 2.0f ) ||
//     (isShaderObjectsSupported &&
//      isVertexShaderSupported &&
//      isFragmentShaderSupported &&
//      isLanguage100Supported ));
//    
//    if( isGlslSupported )
//    {
//        // If glGetString raises an error, assume initial release "1.00"
//        while(glGetError() != GL_NO_ERROR) {}        // reset error flag
//        
//        const char* langVerStr = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
//        if( (glGetError() == GL_NO_ERROR) && langVerStr )
//        {
//            glslLanguageVersion = (findAsciiToFloat(langVerStr));
//        }
//        else
//            glslLanguageVersion = 1.0f;
//    }
//    
//    
//    setGLExtensionFuncPtr(glDrawBuffers, "glDrawBuffers", "glDrawBuffersARB", validContext);
//    setGLExtensionFuncPtr(glAttachShader, "glAttachShader", "glAttachObjectARB", validContext);
//    setGLExtensionFuncPtr(glBindAttribLocation, "glBindAttribLocation", "glBindAttribLocationARB", validContext);
//    setGLExtensionFuncPtr(glCompileShader, "glCompileShader", "glCompileShaderARB", validContext);
//    setGLExtensionFuncPtr(glCreateProgram, "glCreateProgram", "glCreateProgramObjectARB", validContext);
//    setGLExtensionFuncPtr(glCreateShader, "glCreateShader", "glCreateShaderObjectARB", validContext);
//    setGLExtensionFuncPtr(glDeleteProgram, "glDeleteProgram", validContext);
//    setGLExtensionFuncPtr(glDeleteShader, "glDeleteShader", validContext);
//    setGLExtensionFuncPtr(glDetachShader, "glDetachShader", "glDetachObjectARB", validContext);
//    setGLExtensionFuncPtr(glDisableVertexAttribArray, "glDisableVertexAttribArray", validContext);
//    setGLExtensionFuncPtr(glEnableVertexAttribArray, "glEnableVertexAttribArray", validContext);
//    setGLExtensionFuncPtr(glGetActiveAttrib, "glGetActiveAttrib", "glGetActiveAttribARB", validContext);
//    setGLExtensionFuncPtr(glGetActiveUniform, "glGetActiveUniform", "glGetActiveUniformARB", validContext);
//    setGLExtensionFuncPtr(glGetAttachedShaders, "glGetAttachedShaders", "glGetAttachedObjectsARB", validContext);
//    setGLExtensionFuncPtr(glGetAttribLocation, "glGetAttribLocation", "glGetAttribLocationARB", validContext);
//    setGLExtensionFuncPtr(glGetProgramiv, "glGetProgramiv", validContext);
//    setGLExtensionFuncPtr(glGetProgramInfoLog, "glGetProgramInfoLog", validContext);
//    setGLExtensionFuncPtr(glGetShaderiv, "glGetShaderiv", validContext);
//    setGLExtensionFuncPtr(glGetShaderInfoLog, "glGetShaderInfoLog", validContext);
//    setGLExtensionFuncPtr(glGetShaderSource, "glGetShaderSource", "glGetShaderSourceARB", validContext);
//    setGLExtensionFuncPtr(glGetUniformLocation, "glGetUniformLocation", "glGetUniformLocationARB", validContext);
//    setGLExtensionFuncPtr(glGetUniformfv, "glGetUniformfv", "glGetUniformfvARB", validContext);
//    setGLExtensionFuncPtr(glGetUniformiv, "glGetUniformiv", "glGetUniformivARB", validContext);
//    setGLExtensionFuncPtr(glGetVertexAttribdv, "glGetVertexAttribdv", validContext);
//    setGLExtensionFuncPtr(glGetVertexAttribfv, "glGetVertexAttribfv", validContext);
//    setGLExtensionFuncPtr(glGetVertexAttribiv, "glGetVertexAttribiv", validContext);
//    setGLExtensionFuncPtr(glGetVertexAttribPointerv, "glGetVertexAttribPointerv", validContext);
//    setGLExtensionFuncPtr(glIsProgram, "glIsProgram", validContext);
//    setGLExtensionFuncPtr(glIsShader, "glIsShader", validContext);
//    setGLExtensionFuncPtr(glLinkProgram, "glLinkProgram", "glLinkProgramARB", validContext);
//    setGLExtensionFuncPtr(glShaderSource, "glShaderSource", "glShaderSourceARB", validContext);
//    setGLExtensionFuncPtr(glUseProgram, "glUseProgram", "glUseProgramObjectARB", validContext);
//    setGLExtensionFuncPtr(glUniform1f, "glUniform1f", "glUniform1fARB", validContext);
//    setGLExtensionFuncPtr(glUniform2f, "glUniform2f", "glUniform2fARB", validContext);
//    setGLExtensionFuncPtr(glUniform3f, "glUniform3f", "glUniform3fARB", validContext);
//    setGLExtensionFuncPtr(glUniform4f, "glUniform4f", "glUniform4fARB", validContext);
//    setGLExtensionFuncPtr(glUniform1i, "glUniform1i", "glUniform1iARB", validContext);
//    setGLExtensionFuncPtr(glUniform2i, "glUniform2i", "glUniform2iARB", validContext);
//    setGLExtensionFuncPtr(glUniform3i, "glUniform3i", "glUniform3iARB", validContext);
//    setGLExtensionFuncPtr(glUniform4i, "glUniform4i", "glUniform4iARB", validContext);
//    setGLExtensionFuncPtr(glUniform1fv, "glUniform1fv", "glUniform1fvARB", validContext);
//    setGLExtensionFuncPtr(glUniform2fv, "glUniform2fv", "glUniform2fvARB", validContext);
//    setGLExtensionFuncPtr(glUniform3fv, "glUniform3fv", "glUniform3fvARB", validContext);
//    setGLExtensionFuncPtr(glUniform4fv, "glUniform4fv", "glUniform4fvARB", validContext);
//    setGLExtensionFuncPtr(glUniform1iv, "glUniform1iv", "glUniform1ivARB", validContext);
//    setGLExtensionFuncPtr(glUniform2iv, "glUniform2iv", "glUniform2ivARB", validContext);
//    setGLExtensionFuncPtr(glUniform3iv, "glUniform3iv", "glUniform3ivARB", validContext);
//    setGLExtensionFuncPtr(glUniform4iv, "glUniform4iv", "glUniform4ivARB", validContext);
//    setGLExtensionFuncPtr(glUniformMatrix2fv, "glUniformMatrix2fv", "glUniformMatrix2fvARB", validContext);
//    setGLExtensionFuncPtr(glUniformMatrix3fv, "glUniformMatrix3fv", "glUniformMatrix3fvARB", validContext);
//    setGLExtensionFuncPtr(glUniformMatrix4fv, "glUniformMatrix4fv", "glUniformMatrix4fvARB", validContext);
//    setGLExtensionFuncPtr(glValidateProgram, "glValidateProgram", "glValidateProgramARB", validContext);
//    
//    setGLExtensionFuncPtr(glVertexAttrib1d, "glVertexAttrib1d", "glVertexAttrib1dARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib1dv, "glVertexAttrib1dv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib1f, "glVertexAttrib1f", "glVertexAttrib1fARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib1fv, "glVertexAttrib1fv", "glVertexAttrib1fvARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib1s, "glVertexAttrib1s", "glVertexAttrib1sARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib1sv, "glVertexAttrib1sv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib2d, "glVertexAttrib2d", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib2dv, "glVertexAttrib2dv", "glVertexAttrib2dvARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib2f, "glVertexAttrib2f", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib2fv, "glVertexAttrib2fv", "glVertexAttrib2fvARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib2s, "glVertexAttrib2s", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib2sv, "glVertexAttrib2sv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib3d, "glVertexAttrib3d", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib3dv, "glVertexAttrib3dv", "glVertexAttrib3dvARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib3f, "glVertexAttrib3f", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib3fv, "glVertexAttrib3fv", "glVertexAttrib3fvARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib3s, "glVertexAttrib3s", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib3sv, "glVertexAttrib3sv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4Nbv, "glVertexAttrib4Nbv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4Niv, "glVertexAttrib4Niv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4Nsv, "glVertexAttrib4Nsv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4Nub, "glVertexAttrib4Nub", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4Nubv, "glVertexAttrib4Nubv", "glVertexAttrib4NubvARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4Nuiv, "glVertexAttrib4Nuiv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4Nusv, "glVertexAttrib4Nusv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4bv, "glVertexAttrib4bv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4d, "glVertexAttrib4d", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4dv, "glVertexAttrib4dv", "glVertexAttrib4dvARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4f, "glVertexAttrib4f", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4fv, "glVertexAttrib4fv", "glVertexAttrib4fvARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4iv, "glVertexAttrib4iv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4s, "glVertexAttrib4s", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4sv, "glVertexAttrib4sv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4ubv, "glVertexAttrib4ubv", "glVertexAttrib4ubvARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4uiv, "glVertexAttrib4uiv", validContext);
//    setGLExtensionFuncPtr(glVertexAttrib4usv, "glVertexAttrib4usv", validContext);
//    
//    setGLExtensionFuncPtr(glVertexAttribPointer, "glVertexAttribPointer","glVertexAttribPointerARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttribIPointer, "glVertexAttribIPointer","glVertexAttribIPointerARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttribLPointer, "glVertexAttribLPointer","glVertexAttribLPointerARB", validContext);
//    setGLExtensionFuncPtr(glVertexAttribDivisor, "glVertexAttribDivisor", validContext);
//    
//    // v1.5-only ARB entry points, in case they're needed for fallback
//    setGLExtensionFuncPtr(glGetInfoLogARB, "glGetInfoLogARB", validContext);
//    setGLExtensionFuncPtr(glGetObjectParameterivARB, "glGetObjectParameterivARB", validContext);
//    setGLExtensionFuncPtr(glDeleteObjectARB, "glDeleteObjectARB", validContext);
//    setGLExtensionFuncPtr(glGetHandleARB, "glGetHandleARB", validContext);
//    
//    // GL 2.1
//    setGLExtensionFuncPtr(glUniformMatrix2x3fv, "glUniformMatrix2x3fv", validContext);
//    setGLExtensionFuncPtr(glUniformMatrix3x2fv, "glUniformMatrix3x2fv", validContext);
//    setGLExtensionFuncPtr(glUniformMatrix2x4fv, "glUniformMatrix2x4fv", validContext);
//    setGLExtensionFuncPtr(glUniformMatrix4x2fv, "glUniformMatrix4x2fv", validContext);
//    setGLExtensionFuncPtr(glUniformMatrix3x4fv, "glUniformMatrix3x4fv", validContext);
//    setGLExtensionFuncPtr(glUniformMatrix4x3fv, "glUniformMatrix4x3fv", validContext);
//    
//    // ARB_clip_control
//    setGLExtensionFuncPtr(glClipControl, "glClipControl", validContext);
//    
//    // EXT_geometry_shader4
//    setGLExtensionFuncPtr(glProgramParameteri,  "glProgramParameteri", "glProgramParameteriEXT", validContext);
//    
//    // ARB_tesselation_shader
//    setGLExtensionFuncPtr(glPatchParameteri, "glPatchParameteri", validContext);
//    setGLExtensionFuncPtr(glPatchParameterfv, "glPatchParameterfv", validContext);
//    
//    // EXT_gpu_shader4
//    setGLExtensionFuncPtr(glGetUniformuiv,  "glGetUniformuiv", "glGetUniformuivEXT", validContext);
//    setGLExtensionFuncPtr(glBindFragDataLocation,  "glBindFragDataLocation", "glBindFragDataLocationEXT", validContext);
//    setGLExtensionFuncPtr(glBindFragDataLocationIndexed,  "glBindFragDataLocationIndexed", "glBindFragDataLocationIndexedEXT", validContext);
//    setGLExtensionFuncPtr(glGetFragDataIndex,  "glGetFragDataIndex", "glGetFragDataIndexEXT", validContext);
//    setGLExtensionFuncPtr(glGetFragDataLocation,  "glGetFragDataLocation", "glGetFragDataLocationEXT", validContext);
//    setGLExtensionFuncPtr(glUniform1ui,  "glUniform1ui", "glUniform1uiEXT", validContext);
//    setGLExtensionFuncPtr(glUniform2ui,  "glUniform2ui", "glUniform2uiEXT", validContext);
//    setGLExtensionFuncPtr(glUniform3ui,  "glUniform3ui", "glUniform3uiEXT", validContext);
//    setGLExtensionFuncPtr(glUniform4ui,  "glUniform4ui", "glUniform4uiEXT", validContext);
//    setGLExtensionFuncPtr(glUniform1uiv,  "glUniform1uiv", "glUniform1uivEXT", validContext);
//    setGLExtensionFuncPtr(glUniform2uiv,  "glUniform2uiv", "glUniform2uivEXT", validContext);
//    setGLExtensionFuncPtr(glUniform3uiv,  "glUniform3uiv", "glUniform3uivEXT", validContext);
//    setGLExtensionFuncPtr(glUniform4uiv,  "glUniform4uiv", "glUniform4uivEXT", validContext);
//    
//    // ARB_gpu_shader_int64
//    setGLExtensionFuncPtr(glUniform1i64,  "glUniform1i64",  "glUniform1i64ARB",  validContext);
//    setGLExtensionFuncPtr(glUniform1ui64, "glUniform1ui64", "glUniform1ui64ARB", validContext);
//    setGLExtensionFuncPtr(glUniform2i64,  "glUniform2i64",  "glUniform2i64ARB",  validContext);
//    setGLExtensionFuncPtr(glUniform2ui64, "glUniform2ui64", "glUniform2ui64ARB", validContext);
//    setGLExtensionFuncPtr(glUniform3i64,  "glUniform3i64",  "glUniform3i64ARB",  validContext);
//    setGLExtensionFuncPtr(glUniform3ui64, "glUniform3ui64", "glUniform3ui64ARB", validContext);
//    setGLExtensionFuncPtr(glUniform4i64,  "glUniform4i64",  "glUniform4i64ARB",  validContext);
//    setGLExtensionFuncPtr(glUniform4ui64, "glUniform4ui64", "glUniform4ui64ARB", validContext);
//    setGLExtensionFuncPtr(glUniform1i64v, "glUniform1i64v", "glUniform1i64vARB", validContext);
//    setGLExtensionFuncPtr(glUniform1ui64v,"glUniform1ui64v","glUniform1ui64vARB",validContext);
//    setGLExtensionFuncPtr(glUniform2i64v, "glUniform2i64v", "glUniform2i64vARB", validContext);
//    setGLExtensionFuncPtr(glUniform2ui64v,"glUniform2ui64v","glUniform2ui64vARB",validContext);
//    setGLExtensionFuncPtr(glUniform3i64v, "glUniform3i64v", "glUniform3i64vARB", validContext);
//    setGLExtensionFuncPtr(glUniform3ui64v,"glUniform3ui64v","glUniform3ui64vARB",validContext);
//    setGLExtensionFuncPtr(glUniform4i64v, "glUniform4i64v", "glUniform4i64vARB", validContext);
//    setGLExtensionFuncPtr(glUniform4ui64v,"glUniform4ui64v","glUniform4ui64vARB",validContext);
//    
//    // ARB_uniform_buffer_object
//    setGLExtensionFuncPtr(glGetUniformIndices, "glGetUniformIndices", validContext);
//    setGLExtensionFuncPtr(glGetActiveUniformsiv, "glGetActiveUniformsiv", validContext);
//    setGLExtensionFuncPtr(glGetActiveUniformName, "glGetActiveUniformName", validContext);
//    setGLExtensionFuncPtr(glGetUniformBlockIndex, "glGetUniformBlockIndex", validContext);
//    setGLExtensionFuncPtr(glGetActiveUniformBlockiv, "glGetActiveUniformBlockiv", validContext);
//    setGLExtensionFuncPtr(glGetActiveUniformBlockName, "glGetActiveUniformBlockName", validContext);
//    setGLExtensionFuncPtr(glUniformBlockBinding, "glUniformBlockBinding", validContext);
//    
//    // ARB_get_program_binary
//    setGLExtensionFuncPtr(glGetProgramBinary, "glGetProgramBinary", validContext);
//    setGLExtensionFuncPtr(glProgramBinary, "glProgramBinary", validContext);
//    
//    // ARB_gpu_shader_fp64
//    setGLExtensionFuncPtr(glUniform1d, "glUniform1d" , validContext);
//    setGLExtensionFuncPtr(glUniform2d, "glUniform2d" , validContext);
//    setGLExtensionFuncPtr(glUniform3d, "glUniform3d" , validContext);
//    setGLExtensionFuncPtr(glUniform4d, "glUniform4d" , validContext);
//    setGLExtensionFuncPtr(glUniform1dv, "glUniform1dv" , validContext);
//    setGLExtensionFuncPtr(glUniform2dv, "glUniform2dv" , validContext);
//    setGLExtensionFuncPtr(glUniform3dv, "glUniform3dv" , validContext);
//    setGLExtensionFuncPtr(glUniform4dv, "glUniform4dv" , validContext);
//    setGLExtensionFuncPtr(glUniformMatrix2dv, "glUniformMatrix2dv" , validContext);
//    setGLExtensionFuncPtr(glUniformMatrix3dv, "glUniformMatrix3dv" , validContext);
//    setGLExtensionFuncPtr(glUniformMatrix4dv, "glUniformMatrix4dv" , validContext);
//    setGLExtensionFuncPtr(glUniformMatrix2x3dv,  "glUniformMatrix2x3dv" , validContext);
//    setGLExtensionFuncPtr(glUniformMatrix3x2dv,  "glUniformMatrix3x2dv" , validContext);
//    setGLExtensionFuncPtr(glUniformMatrix2x4dv,  "glUniformMatrix2x4dv" , validContext);
//    setGLExtensionFuncPtr(glUniformMatrix4x2dv,  "glUniformMatrix4x2dv" , validContext);
//    setGLExtensionFuncPtr(glUniformMatrix3x4dv,  "glUniformMatrix3x4dv" , validContext);
//    setGLExtensionFuncPtr(glUniformMatrix4x3dv,  "glUniformMatrix4x3dv" , validContext);
//    
//    // ARB_shader_atomic_counters
//    setGLExtensionFuncPtr(glGetActiveAtomicCounterBufferiv,  "glGetActiveAtomicCounterBufferiv" , validContext);
//    
//    // ARB_compute_shader
//    setGLExtensionFuncPtr(glDispatchCompute,  "glDispatchCompute" , validContext);
//    
//    
//    setGLExtensionFuncPtr(glMemoryBarrier,  "glMemoryBarrier", "glMemoryBarrierEXT" , validContext);
//    
//    // BufferObject extensions
//    setGLExtensionFuncPtr(glGenBuffers, "glGenBuffers","glGenBuffersARB", validContext);
//    setGLExtensionFuncPtr(glBindBuffer, "glBindBuffer","glBindBufferARB", validContext);
//    setGLExtensionFuncPtr(glBufferData, "glBufferData","glBufferDataARB", validContext);
//    setGLExtensionFuncPtr(glBufferSubData, "glBufferSubData","glBufferSubDataARB", validContext);
//    setGLExtensionFuncPtr(glDeleteBuffers, "glDeleteBuffers","glDeleteBuffersARB", validContext);
//    setGLExtensionFuncPtr(glIsBuffer, "glIsBuffer","glIsBufferARB", validContext);
//    setGLExtensionFuncPtr(glGetBufferSubData, "glGetBufferSubData","glGetBufferSubDataARB", validContext);
//    setGLExtensionFuncPtr(glBufferStorage, "glBufferStorage","glBufferStorageARB", validContext);
//    setGLExtensionFuncPtr(glNamedBufferStorage, "glNamedBufferStorage","glNamedBufferStorageARB", validContext);
//    setGLExtensionFuncPtr(glMapBuffer, "glMapBuffer","glMapBufferARB", validContext);
//    setGLExtensionFuncPtr(glMapBufferRange,  "glMapBufferRange", "glMapBufferRangeARB" , validContext);
//    setGLExtensionFuncPtr(glUnmapBuffer, "glUnmapBuffer","glUnmapBufferARB", validContext);
//    setGLExtensionFuncPtr(glGetBufferParameteriv, "glGetBufferParameteriv","glGetBufferParameterivARB", validContext);
//    setGLExtensionFuncPtr(glGetBufferPointerv, "glGetBufferPointerv","glGetBufferPointervARB", validContext);
//    setGLExtensionFuncPtr(glBindBufferRange, "glBindBufferRange", validContext);
//    setGLExtensionFuncPtr(glBindBufferBase,  "glBindBufferBase", "glBindBufferBaseEXT", "glBindBufferBaseNV" , validContext);
//    setGLExtensionFuncPtr(glTexBuffer, "glTexBuffer","glTexBufferARB" , validContext);
//    
//    isVBOSupported = validContext && (OSG_GLES2_FEATURES || OSG_GLES3_FEATURES || OSG_GL3_FEATURES || osg::isGLExtensionSupported(contextID,"GL_ARB_vertex_buffer_object"));
//    isPBOSupported = validContext && (OSG_GLES3_FEATURES || OSG_GL3_FEATURES || osg::isGLExtensionSupported(contextID,"GL_ARB_pixel_buffer_object"));
//    isTBOSupported = validContext && osg::isGLExtensionSupported(contextID,"GL_ARB_texture_buffer_object");
//    isVAOSupported = validContext && (OSG_GLES3_FEATURES || OSG_GL3_FEATURES  || osg::isGLExtensionSupported(contextID, "GL_ARB_vertex_array_object", "GL_OES_vertex_array_object"));
//    isTransformFeedbackSupported = validContext && osg::isGLExtensionSupported(contextID, "GL_ARB_transform_feedback2");
//    isBufferObjectSupported = isVBOSupported || isPBOSupported;
//    
//    
//    // BlendFunc extensions
//    isBlendFuncSeparateSupported = validContext &&
//    (OSG_GLES2_FEATURES || OSG_GLES3_FEATURES || OSG_GL3_FEATURES ||
//     osg::isGLExtensionSupported(contextID, "GL_EXT_blend_func_separate") ||
//     (glVersion >= 1.4f));
//    
//    setGLExtensionFuncPtr(glBlendFuncSeparate, "glBlendFuncSeparate", "glBlendFuncSeparateEXT", validContext);
//    
//    setGLExtensionFuncPtr(glBlendFunci, "glBlendFunci", "glBlendFunciARB", validContext);
//    setGLExtensionFuncPtr(glBlendFuncSeparatei, "glBlendFuncSeparatei", "glBlendFuncSeparateiARB", validContext);
//    
//    
//    isSecondaryColorSupported = validContext && isGLExtensionSupported(contextID,"GL_EXT_secondary_color");
//    isFogCoordSupported = validContext && isGLExtensionSupported(contextID,"GL_EXT_fog_coord");
//    isMultiTexSupported = validContext && isGLExtensionSupported(contextID,"GL_ARB_multitexture");
//    isOcclusionQuerySupported = validContext && osg::isGLExtensionSupported(contextID, "GL_NV_occlusion_query");
//    isARBOcclusionQuerySupported = validContext && (OSG_GL3_FEATURES || osg::isGLExtensionSupported(contextID, "GL_ARB_occlusion_query"));
//    
//    isTimerQuerySupported = validContext && osg::isGLExtensionSupported(contextID, "GL_EXT_timer_query");
//    isARBTimerQuerySupported = validContext && osg::isGLExtensionSupported(contextID, "GL_ARB_timer_query");
//    
//    
//    setGLExtensionFuncPtr(glDrawArraysInstanced, "glDrawArraysInstanced","glDrawArraysInstancedARB","glDrawArraysInstancedEXT", validContext);
//    setGLExtensionFuncPtr(glDrawElementsInstanced, "glDrawElementsInstanced","glDrawElementsInstancedARB","glDrawElementsInstancedEXT", validContext);
//    
//    
//    setGLExtensionFuncPtr(glFogCoordfv, "glFogCoordfv","glFogCoordfvEXT", validContext);
//    setGLExtensionFuncPtr(glSecondaryColor3ubv, "glSecondaryColor3ubv","glSecondaryColor3ubvEXT", validContext);
//    setGLExtensionFuncPtr(glSecondaryColor3fv, "glSecondaryColor3fv","glSecondaryColor3fvEXT", validContext);
//    
//    setGLExtensionFuncPtr(glMultiTexCoord1f, "glMultiTexCoord1f","glMultiTexCoord1fARB", validContext);
//    setGLExtensionFuncPtr(glMultiTexCoord4f, "glMultiTexCoord4f","glMultiTexCoord4fARB", validContext);
//    
//    setGLExtensionFuncPtr(glMultiTexCoord1fv, "glMultiTexCoord1fv","glMultiTexCoord1fvARB", validContext);
//    setGLExtensionFuncPtr(glMultiTexCoord2fv, "glMultiTexCoord2fv","glMultiTexCoord2fvARB", validContext);
//    setGLExtensionFuncPtr(glMultiTexCoord3fv, "glMultiTexCoord3fv","glMultiTexCoord3fvARB", validContext);
//    setGLExtensionFuncPtr(glMultiTexCoord4fv, "glMultiTexCoord4fv","glMultiTexCoord4fvARB", validContext);
//    
//    
//    setGLExtensionFuncPtr(glMultiTexCoord1d, "glMultiTexCoord1d","glMultiTexCoord1dARB", validContext);
//    setGLExtensionFuncPtr(glMultiTexCoord1dv, "glMultiTexCoord1dv","glMultiTexCoord1dvARB", validContext);
//    setGLExtensionFuncPtr(glMultiTexCoord2dv, "glMultiTexCoord2dv","glMultiTexCoord2dvARB", validContext);
//    setGLExtensionFuncPtr(glMultiTexCoord3dv, "glMultiTexCoord3dv","glMultiTexCoord3dvARB", validContext);
//    setGLExtensionFuncPtr(glMultiTexCoord4dv, "glMultiTexCoord4dv","glMultiTexCoord4dvARB", validContext);
//    
//    setGLExtensionFuncPtr(glGenOcclusionQueries, "glGenOcclusionQueries","glGenOcclusionQueriesNV", validContext);
//    setGLExtensionFuncPtr(glDeleteOcclusionQueries, "glDeleteOcclusionQueries","glDeleteOcclusionQueriesNV", validContext);
//    setGLExtensionFuncPtr(glIsOcclusionQuery, "glIsOcclusionQuery","glIsOcclusionQueryNV", validContext);
//    setGLExtensionFuncPtr(glBeginOcclusionQuery, "glBeginOcclusionQuery","glBeginOcclusionQueryNV", validContext);
//    setGLExtensionFuncPtr(glEndOcclusionQuery, "glEndOcclusionQuery","glEndOcclusionQueryNV", validContext);
//    setGLExtensionFuncPtr(glGetOcclusionQueryiv, "glGetOcclusionQueryiv","glGetOcclusionQueryivNV", validContext);
//    setGLExtensionFuncPtr(glGetOcclusionQueryuiv, "glGetOcclusionQueryuiv","glGetOcclusionQueryuivNV", validContext);
//    
//    setGLExtensionFuncPtr(glGenQueries, "glGenQueries", "glGenQueriesARB", validContext);
//    setGLExtensionFuncPtr(glDeleteQueries, "glDeleteQueries", "glDeleteQueriesARB", validContext);
//    setGLExtensionFuncPtr(glIsQuery, "glIsQuery", "glIsQueryARB", validContext);
//    setGLExtensionFuncPtr(glBeginQuery, "glBeginQuery", "glBeginQueryARB", validContext);
//    setGLExtensionFuncPtr(glEndQuery, "glEndQuery", "glEndQueryARB", validContext);
//    setGLExtensionFuncPtr(glBeginQueryIndexed, "glBeginQueryIndexed", "glBeginQueryIndexedARB", validContext);
//    setGLExtensionFuncPtr(glEndQueryIndexed, "glEndQueryIndexed", "glEndQueryIndexedARB", validContext);
//    setGLExtensionFuncPtr(glGetQueryiv, "glGetQueryiv", "glGetQueryivARB", validContext);
//    setGLExtensionFuncPtr(glGetQueryObjectiv, "glGetQueryObjectiv","glGetQueryObjectivARB", validContext);
//    setGLExtensionFuncPtr(glGetQueryObjectuiv, "glGetQueryObjectuiv","glGetQueryObjectuivARB", validContext);
//    setGLExtensionFuncPtr(glGetQueryObjectui64v, "glGetQueryObjectui64v","glGetQueryObjectui64vEXT", validContext);
//    setGLExtensionFuncPtr(glQueryCounter, "glQueryCounter", validContext);
//    setGLExtensionFuncPtr(glGetInteger64v, "glGetInteger64v", validContext);
//    
//    
//    // SampleMaski functionality
//    isTextureMultisampleSupported = validContext && isGLExtensionSupported(contextID, "GL_ARB_texture_multisample");
//    isOpenGL32upported = (glVersion >= 3.2f);
//    
//    // function pointers
//    setGLExtensionFuncPtr(glSampleMaski, "glSampleMaski", validContext);
//    isSampleMaskiSupported = validContext && (isOpenGL32upported || isGLExtensionSupported(contextID,"ARB_texture_multisample"));
//    
//    
//    // old styple Vertex/Fragment Programs
//    isVertexProgramSupported = validContext && isGLExtensionSupported(contextID,"GL_ARB_vertex_program");
//    isFragmentProgramSupported = validContext && isGLExtensionSupported(contextID,"GL_ARB_fragment_program");
//    
//    setGLExtensionFuncPtr(glBindProgram,"glBindProgramARB", validContext);
//    setGLExtensionFuncPtr(glGenPrograms, "glGenProgramsARB", validContext);
//    setGLExtensionFuncPtr(glDeletePrograms, "glDeleteProgramsARB", validContext);
//    setGLExtensionFuncPtr(glProgramString, "glProgramStringARB", validContext);
//    setGLExtensionFuncPtr(glProgramLocalParameter4fv, "glProgramLocalParameter4fvARB", validContext);
//    
//    // Sample Extensions (OpenGL>=3.3)
//    setGLExtensionFuncPtr(glSamplerParameteri, "glSamplerParameteri", "glSamplerParameteriARB", validContext);
//    setGLExtensionFuncPtr(glSamplerParameterf, "glSamplerParameterf", "glSamplerParameterfARB", validContext);
//    setGLExtensionFuncPtr(glSamplerParameteriv, "glSamplerParameteriv", "glSamplerParameterivARB", validContext);
//    setGLExtensionFuncPtr(glSamplerParameterfv, "glSamplerParameterfv", "glSamplerParameterfvARB", validContext);
//    setGLExtensionFuncPtr(glSamplerParameterIiv, "glSamplerParameterIiv", "glSamplerParameterIivARB", validContext);
//    setGLExtensionFuncPtr(glSamplerParameterIuiv, "glSamplerParameterIuiv", "glSamplerParameterIuivARB", validContext);
//    
//    setGLExtensionFuncPtr(glGetSamplerParameteriv, "glGetSamplerParameteriv", "glGetSamplerParameterivARB", validContext);
//    setGLExtensionFuncPtr(glGetSamplerParameterfv, "glGetSamplerParameterfv", "glGetSamplerParameterfvARB", validContext);
//    setGLExtensionFuncPtr(glGetSamplerParameterIiv, "glGetSamplerParameterIiv", "glGetSamplerParameterIivARB", validContext);
//    setGLExtensionFuncPtr(glGetSamplerParameterIuiv, "glGetSamplerParameterIuiv", "glGetSamplerParameterIuivARB", validContext);
//    
//    setGLExtensionFuncPtr(glGenSamplers, "glGenSamplers", "glGenSamplersARB", validContext);
//    setGLExtensionFuncPtr(glDeleteSamplers, "glDeleteSamplers", "glDeleteSamplersARB", validContext);
//    setGLExtensionFuncPtr(glBindSampler, "glBindSampler", "glBindSamplerARB", validContext);
//    setGLExtensionFuncPtr(glIsSampler, "glIsSampler", "glIsSamplerARB", validContext);
//    
//    // Texture extensions
//    const char* renderer = validContext ? (const char*) glGetString(GL_RENDERER) : 0;
//    std::string rendererString(renderer ? renderer : "");
//    
//    bool radeonHardwareDetected = (rendererString.find("Radeon")!=std::string::npos || rendererString.find("RADEON")!=std::string::npos);
//    bool fireGLHardwareDetected = (rendererString.find("FireGL")!=std::string::npos || rendererString.find("FIREGL")!=std::string::npos);
//    
//    bool builtInSupport = OSG_GLES2_FEATURES || OSG_GLES3_FEATURES || OSG_GL3_FEATURES;
//    
//    isMultiTexturingSupported = validContext &&
//    (builtInSupport || OSG_GLES1_FEATURES ||
//     isGLExtensionOrVersionSupported( contextID,"GL_ARB_multitexture", 1.3f) ||
//     isGLExtensionOrVersionSupported(contextID,"GL_EXT_multitexture", 1.3f));
//    
//    isTextureFilterAnisotropicSupported = validContext && isGLExtensionSupported(contextID,"GL_EXT_texture_filter_anisotropic");
//    isTextureSwizzleSupported = validContext && isGLExtensionSupported(contextID,"GL_ARB_texture_swizzle");
//    isTextureCompressionARBSupported = validContext && (builtInSupport || isGLExtensionOrVersionSupported(contextID,"GL_ARB_texture_compression", 1.3f));
//    isTextureCompressionS3TCSupported = validContext && (isGLExtensionSupported(contextID,"GL_EXT_texture_compression_s3tc") || isGLExtensionSupported(contextID, "GL_S3_s3tc"));
//    isTextureCompressionPVRTC2BPPSupported = validContext && isGLExtensionSupported(contextID,"GL_IMG_texture_compression_pvrtc");
//    isTextureCompressionPVRTC4BPPSupported = isTextureCompressionPVRTC2BPPSupported;//covered by same extension
//    isTextureCompressionETCSupported = validContext && isGLExtensionSupported(contextID,"GL_OES_compressed_ETC1_RGB8_texture");
//    isTextureCompressionETC2Supported = validContext && isGLExtensionSupported(contextID,"GL_ARB_ES3_compatibility");
//    isTextureCompressionRGTCSupported = validContext && isGLExtensionSupported(contextID,"GL_EXT_texture_compression_rgtc");
//    isTextureCompressionPVRTCSupported = validContext && isGLExtensionSupported(contextID,"GL_IMG_texture_compression_pvrtc");
//    
//    isTextureMirroredRepeatSupported = validContext &&
//    (builtInSupport ||
//     isGLExtensionOrVersionSupported(contextID,"GL_IBM_texture_mirrored_repeat", 1.4f) ||
//     isGLExtensionOrVersionSupported(contextID,"GL_ARB_texture_mirrored_repeat", 1.4f));
//    
//    isTextureEdgeClampSupported = validContext &&
//    (builtInSupport ||
//     isGLExtensionOrVersionSupported(contextID,"GL_EXT_texture_edge_clamp", 1.2f) ||
//     isGLExtensionOrVersionSupported(contextID,"GL_SGIS_texture_edge_clamp", 1.2f));
//    
//    
//    isTextureBorderClampSupported = validContext &&
//    (OSG_GL3_FEATURES ||
//     ((OSG_GL1_FEATURES || OSG_GL2_FEATURES) && isGLExtensionOrVersionSupported(contextID,"GL_ARB_texture_border_clamp", 1.3f)) ||
//     ((OSG_GLES2_FEATURES || OSG_GLES3_FEATURES) && isGLExtensionSupported(contextID,"GL_EXT_texture_border_clamp")));
//    
//    isGenerateMipMapSupported = validContext && (builtInSupport || isGLExtensionOrVersionSupported(contextID,"GL_SGIS_generate_mipmap", 1.4f));
//    preferGenerateMipmapSGISForPowerOfTwo = (radeonHardwareDetected||fireGLHardwareDetected) ? false : true;
//    isTextureMultisampledSupported = validContext && (isGLExtensionSupported(contextID,"GL_ARB_texture_multisample"));
//    isShadowSupported = validContext && (OSG_GL3_FEATURES || isGLExtensionSupported(contextID,"GL_ARB_shadow"));
//    isShadowAmbientSupported = validContext && (isGLExtensionSupported(contextID,"GL_ARB_shadow_ambient"));
//    isClientStorageSupported = validContext && (isGLExtensionSupported(contextID,"GL_APPLE_client_storage"));
//    isNonPowerOfTwoTextureMipMappedSupported = validContext && (builtInSupport || isGLExtensionSupported(contextID, "GL_OES_texture_npot") || isGLExtensionOrVersionSupported(contextID, "GL_ARB_texture_non_power_of_two", 2.0));
//    isNonPowerOfTwoTextureNonMipMappedSupported = validContext && (isNonPowerOfTwoTextureMipMappedSupported || isGLExtensionSupported(contextID, "GL_APPLE_texture_2D_limited_npot"));
//    isTextureIntegerEXTSupported = validContext && (OSG_GL3_FEATURES || isGLExtensionSupported(contextID, "GL_EXT_texture_integer"));
//    
//    if (rendererString.find("GeForce FX")!=std::string::npos)
//    {
//        isNonPowerOfTwoTextureMipMappedSupported = false;
//        OSG_INFO<<"Disabling _isNonPowerOfTwoTextureMipMappedSupported for GeForce FX hardware."<<std::endl;
//    }
//    
//    maxTextureSize=0;
//    if (validContext) glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxTextureSize);
//    
//    GLint osg_max_size = maxTextureSize;
//    if( (getEnvVar("OSG_MAX_TEXTURE_SIZE", osg_max_size)) && osg_max_size<maxTextureSize)
//    {
//        maxTextureSize = osg_max_size;
//    }
//    isTextureMaxLevelSupported = (glVersion >= 1.2f);
//    
//    isTextureStorageEnabled = validContext && ((glVersion >= 4.2f) || isGLExtensionSupported(contextID, "GL_ARB_texture_storage"));
//    
//    if (isTextureStorageEnabled)
//    {
//        std::string value;
//        if (getEnvVar("OSG_GL_TEXTURE_STORAGE", value))
//        {
//            if (value=="OFF" || value=="DISABLE") isTextureStorageEnabled = false;
//            else isTextureStorageEnabled = true;
//        }
//    }
//    
//    setGLExtensionFuncPtr(glTexStorage1D,"glTexStorage1D","glTexStorage1DARB", validContext);
//    setGLExtensionFuncPtr(glTextureStorage1D,"glTextureStorage1D","glTextureStorage1DARB", validContext);
//    setGLExtensionFuncPtr(glTexStorage2D,"glTexStorage2D","glTexStorage2DARB", validContext);
//    setGLExtensionFuncPtr(glTextureStorage2D,"glTextureStorage2D","glTextureStorage2DARB", validContext);
//    setGLExtensionFuncPtr(glTexStorage3D, "glTexStorage3D","glTexStorage3DEXT", validContext);
//    setGLExtensionFuncPtr(glTextureStorage3D, "glTextureStorage3D","glTextureStorage3DEXT", validContext);
//    setGLExtensionFuncPtr(glTexStorage2DMultisample, "glTextureStorage2DMultisample","glTextureStorage2DMultisampleEXT", validContext);
//    setGLExtensionFuncPtr(glTexStorage3DMultisample, "glTextureStorage3DMultisample","glTextureStorage3DMultisampleEXT", validContext);
//    setGLExtensionFuncPtr(glTextureView, "glTextureView","glTextureViewEXT", validContext);
//    
//    setGLExtensionFuncPtr(glCompressedTexImage2D,"glCompressedTexImage2D","glCompressedTexImage2DARB", validContext);
//    setGLExtensionFuncPtr(glCompressedTexSubImage2D,"glCompressedTexSubImage2D","glCompressedTexSubImage2DARB", validContext);
//    setGLExtensionFuncPtr(glGetCompressedTexImage,"glGetCompressedTexImage","glGetCompressedTexImageARB", validContext);;
//    setGLExtensionFuncPtr(glTexImage2DMultisample, "glTexImage2DMultisample", "glTexImage2DMultisampleARB", validContext);
//    
//    setGLExtensionFuncPtr(glTexParameterIiv, "glTexParameterIiv", "glTexParameterIivARB", "glTexParameterIivEXT", validContext);
//    setGLExtensionFuncPtr(glTexParameterIuiv, "glTexParameterIuiv", "glTexParameterIuivARB", "glTexParameterIuivEXT", validContext);
//    
//    setGLExtensionFuncPtr(glBindImageTexture, "glBindImageTexture", "glBindImageTextureARB", validContext);
//    
//    
//    // Texture3D extensions
//    isTexture3DFast = validContext && (OSG_GL3_FEATURES || isGLExtensionSupported(contextID,"GL_EXT_texture3D"));
//    
//    if (isTexture3DFast) isTexture3DSupported = true;
//    else isTexture3DSupported = validContext && (glVersion >= 1.2f);
//    
//    maxTexture3DSize = 0;
//    if (validContext) glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &maxTexture3DSize);
//    
//    setGLExtensionFuncPtr(glTexImage3D, "glTexImage3D","glTexImage3DEXT", validContext);
//    setGLExtensionFuncPtr(glTexSubImage3D, "glTexSubImage3D","glTexSubImage3DEXT", validContext);
//    
//    setGLExtensionFuncPtr(glCompressedTexImage3D, "glCompressedTexImage3D","glCompressedTexImage3DARB", validContext);
//    setGLExtensionFuncPtr(glCompressedTexSubImage3D, "glCompressedTexSubImage3D","glCompressedTexSubImage3DARB", validContext);
//    
//    setGLExtensionFuncPtr(glTexImage3DMultisample, "glTexImage3DMultisample", validContext);
//    setGLExtensionFuncPtr(glGetMultisamplefv, "glGetMultisamplefv", validContext);
//    
//    setGLExtensionFuncPtr(glCopyTexSubImage3D, "glCopyTexSubImage3D","glCopyTexSubImage3DEXT", validContext);
//    setGLExtensionFuncPtr(glBeginConditionalRender, "glBeginConditionalRender", "glBeginConditionalRenderARB");
//    setGLExtensionFuncPtr(glEndConditionalRender, "glEndConditionalRender", "glEndConditionalRenderARB");
//    
//    // Texture2DArray extensions
//    isTexture2DArraySupported = validContext && (OSG_GL3_FEATURES || isGLExtensionSupported(contextID,"GL_EXT_texture_array"));
//    
//    max2DSize = 0;
//    if (validContext) glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max2DSize);
//    maxLayerCount = 0;
//    if (validContext) glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayerCount);
//    
//    // Bindless textures
//    setGLExtensionFuncPtr(glGetTextureHandle,             "glGetTextureHandle", "glGetTextureHandleARB","glGetTextureHandleNV", validContext);
//    setGLExtensionFuncPtr(glMakeTextureHandleResident,    "glMakeTextureHandleResident", "glMakeTextureHandleResidentARB","glMakeTextureHandleResidentNV", validContext);
//    setGLExtensionFuncPtr(glMakeTextureHandleNonResident, "glMakeTextureHandleNonResident", "glMakeTextureHandleNonResidentARB", "glMakeTextureHandleNonResidentNV",validContext);
//    setGLExtensionFuncPtr(glUniformHandleui64,            "glUniformHandleui64", "glUniformHandleui64ARB","glUniformHandleui64NV", validContext);
//    setGLExtensionFuncPtr(glIsTextureHandleResident,      "glIsTextureHandleResident","glIsTextureHandleResidentARB", "glIsTextureHandleResidentNV", validContext);
//    
//    // Blending
//    isBlendColorSupported = validContext &&
//    (OSG_GLES2_FEATURES || OSG_GLES3_FEATURES || OSG_GL3_FEATURES ||
//     isGLExtensionSupported(contextID,"GL_EXT_blend_color") ||
//     (glVersion >= 1.2f));
//    
//    setGLExtensionFuncPtr(glBlendColor, "glBlendColor", "glBlendColorEXT", validContext);
//    
//    bool bultInSupport = OSG_GLES2_FEATURES || OSG_GLES3_FEATURES || OSG_GL3_FEATURES;
//    isBlendEquationSupported = validContext &&
//    (bultInSupport ||
//     isGLExtensionSupported(contextID, "GL_EXT_blend_equation") ||
//     (glVersion >= 1.2f));
//    
//    
//    isBlendEquationSeparateSupported = validContext &&
//    (bultInSupport ||
//     isGLExtensionSupported(contextID, "GL_EXT_blend_equation_separate") ||
//     (glVersion >= 2.0f));
//    
//    
//    isSGIXMinMaxSupported = validContext && isGLExtensionSupported(contextID, "GL_SGIX_blend_alpha_minmax");
//    isLogicOpSupported = validContext && isGLExtensionSupported(contextID, "GL_EXT_blend_logic_op");
//    
//    setGLExtensionFuncPtr(glBlendEquation, "glBlendEquation", "glBlendEquationEXT", validContext);
//    setGLExtensionFuncPtr(glBlendEquationSeparate, "glBlendEquationSeparate", "glBlendEquationSeparateEXT", validContext);
//    
//    setGLExtensionFuncPtr(glBlendEquationi, "glBlendEquationi", "glBlendEquationiARB", validContext);
//    setGLExtensionFuncPtr(glBlendEquationSeparatei, "glBlendEquationSeparatei", "glBlendEquationSeparateiARB", validContext);
//    
//    
//    // glEnablei/glDisabli
//    setGLExtensionFuncPtr(glEnablei, "glEnablei", validContext);
//    setGLExtensionFuncPtr(glDisablei, "glDisablei", validContext);
//    
//    
//    // Stencil`
//    isStencilWrapSupported = validContext && isGLExtensionOrVersionSupported(contextID, "GL_EXT_stencil_wrap", 1.4f);
//    isStencilTwoSidedSupported = validContext && isGLExtensionSupported(contextID, "GL_EXT_stencil_two_side");
//    isOpenGL20Supported = validContext && (glVersion >= 2.0f);
//    isSeparateStencilSupported = validContext && isGLExtensionSupported(contextID, "GL_ATI_separate_stencil");
//    
//    // function pointers
//    setGLExtensionFuncPtr(glActiveStencilFace, "glActiveStencilFaceEXT", validContext);
//    setGLExtensionFuncPtr(glStencilOpSeparate, "glStencilOpSeparate", "glStencilOpSeparateATI", validContext);
//    setGLExtensionFuncPtr(glStencilMaskSeparate, "glStencilMaskSeparate", validContext);
//    setGLExtensionFuncPtr(glStencilFuncSeparate, "glStencilFuncSeparate", "glStencilFuncSeparateATI", validContext);
//    setGLExtensionFuncPtr(glStencilFuncSeparateATI, "glStencilFuncSeparateATI", validContext);
//    
//    
//    // Color Mask
//    setGLExtensionFuncPtr(glColorMaski, "glColorMaski", "glColorMaskiARB", validContext);
//    
//    
//    // ClampColor
//    isClampColorSupported = validContext &&
//    (OSG_GL3_FEATURES ||
//     isGLExtensionSupported(contextID,"GL_ARB_color_buffer_float") ||
//     (glVersion >= 2.0f));
//    
//    setGLExtensionFuncPtr(glClampColor, "glClampColor", "glClampColorARB", validContext);
//    
//    
//    // PrimitiveRestartIndex
//    setGLExtensionFuncPtr(glPrimitiveRestartIndex, "glPrimitiveRestartIndex", "glPrimitiveRestartIndexNV", validContext);
//    
//    
//    // Point
//    isPointParametersSupported = validContext &&
//    (OSG_GL3_FEATURES || (glVersion >= 1.4f)  ||
//     isGLExtensionSupported(contextID,"GL_ARB_point_parameters") ||
//     isGLExtensionSupported(contextID,"GL_EXT_point_parameters") ||
//     isGLExtensionSupported(contextID,"GL_SGIS_point_parameters"));
//    
//    
//    isPointSpriteSupported = validContext && (OSG_GLES2_FEATURES || OSG_GLES3_FEATURES || OSG_GL3_FEATURES || isGLExtensionSupported(contextID, "GL_ARB_point_sprite") || isGLExtensionSupported(contextID, "GL_OES_point_sprite") || isGLExtensionSupported(contextID, "GL_NV_point_sprite"));
//    
//    isPointSpriteModeSupported = isPointSpriteModeSupported && !OSG_GL3_FEATURES;
//    
//    isPointSpriteCoordOriginSupported = validContext && (OSG_GL3_FEATURES || (glVersion >= 2.0f));
//    
//    
//    setGLExtensionFuncPtr(glPointParameteri, "glPointParameteri", "glPointParameteriARB", validContext);
//    if (!glPointParameteri) setGLExtensionFuncPtr(glPointParameteri, "glPointParameteriEXT", "glPointParameteriSGIS", validContext);
//    
//    setGLExtensionFuncPtr(glPointParameterf, "glPointParameterf", "glPointParameterfARB", validContext);
//    if (!glPointParameterf) setGLExtensionFuncPtr(glPointParameterf, "glPointParameterfEXT", "glPointParameterfSGIS", validContext);
//    
//    setGLExtensionFuncPtr(glPointParameterfv, "glPointParameterfv", "glPointParameterfvARB", validContext);
//    if (!glPointParameterfv) setGLExtensionFuncPtr(glPointParameterfv, "glPointParameterfvEXT", "glPointParameterfvSGIS", validContext);
//    
//    
//    // Multisample
//    isMultisampleSupported = validContext && (OSG_GLES2_FEATURES || OSG_GLES3_FEATURES || OSG_GL3_FEATURES || isGLExtensionSupported(contextID,"GL_ARB_multisample"));
//    isMultisampleFilterHintSupported = validContext && isGLExtensionSupported(contextID, "GL_NV_multisample_filter_hint");
//    
//    setGLExtensionFuncPtr(glSampleCoverage, "glSampleCoverage", "glSampleCoverageARB", validContext);
//    
//    
//    // FrameBufferObject
//    setGLExtensionFuncPtr(glBindRenderbuffer, "glBindRenderbuffer", "glBindRenderbufferEXT", "glBindRenderbufferOES", validContext);
//    setGLExtensionFuncPtr(glDeleteRenderbuffers, "glDeleteRenderbuffers", "glDeleteRenderbuffersEXT", "glDeleteRenderbuffersOES", validContext);
//    setGLExtensionFuncPtr(glGenRenderbuffers, "glGenRenderbuffers", "glGenRenderbuffersEXT", "glGenRenderbuffersOES", validContext);
//    setGLExtensionFuncPtr(glRenderbufferStorage, "glRenderbufferStorage", "glRenderbufferStorageEXT", "glRenderbufferStorageOES", validContext);
//    setGLExtensionFuncPtr(glRenderbufferStorageMultisample, "glRenderbufferStorageMultisample", "glRenderbufferStorageMultisampleEXT", "glRenderbufferStorageMultisampleOES", validContext);
//    setGLExtensionFuncPtr(glRenderbufferStorageMultisampleCoverageNV, "glRenderbufferStorageMultisampleCoverageNV", validContext);
//    setGLExtensionFuncPtr(glBindFramebuffer, "glBindFramebuffer", "glBindFramebufferEXT", "glBindFramebufferOES", validContext);
//    setGLExtensionFuncPtr(glDeleteFramebuffers, "glDeleteFramebuffers", "glDeleteFramebuffersEXT", "glDeleteFramebuffersOES", validContext);
//    setGLExtensionFuncPtr(glGenFramebuffers, "glGenFramebuffers", "glGenFramebuffersEXT", "glGenFramebuffersOES", validContext);
//    setGLExtensionFuncPtr(glCheckFramebufferStatus, "glCheckFramebufferStatus", "glCheckFramebufferStatusEXT", "glCheckFramebufferStatusOES", validContext);
//    
//    setGLExtensionFuncPtr(glFramebufferTexture1D, "glFramebufferTexture1D", "glFramebufferTexture1DEXT", "glFramebufferTexture1DOES", validContext);
//    setGLExtensionFuncPtr(glFramebufferTexture2D, "glFramebufferTexture2D", "glFramebufferTexture2DEXT", "glFramebufferTexture2DOES", validContext);
//    setGLExtensionFuncPtr(glFramebufferTexture3D, "glFramebufferTexture3D", "glFramebufferTexture3DEXT", "glFramebufferTexture3DOES", validContext);
//    setGLExtensionFuncPtr(glFramebufferTexture, "glFramebufferTexture", "glFramebufferTextureEXT", "glFramebufferTextureOES", validContext);
//    setGLExtensionFuncPtr(glFramebufferTextureLayer, "glFramebufferTextureLayer", "glFramebufferTextureLayerEXT", "glFramebufferTextureLayerOES", validContext);
//    setGLExtensionFuncPtr(glFramebufferTextureFace,  "glFramebufferTextureFace", "glFramebufferTextureFaceEXT", "glFramebufferTextureFaceOES" , validContext);
//    setGLExtensionFuncPtr(glFramebufferRenderbuffer, "glFramebufferRenderbuffer", "glFramebufferRenderbufferEXT", "glFramebufferRenderbufferOES", validContext);
//    //ARB_framebuffer_no_attachments
//    //OpenGL 4.3
//    setGLExtensionFuncPtr(glFramebufferParameteri, "glFramebufferParameteri", "glFramebufferParameteriARB", "glFramebufferParameteriOES", validContext);
//    setGLExtensionFuncPtr(glGetFramebufferParameteriv, "glGetFramebufferParameteriv", "glGetFramebufferParameterivARB", "glGetFramebufferParameterivOES", validContext);
//    //OpenGL 4.5 (EXT_direct_state_access required)
//    setGLExtensionFuncPtr(glNamedFramebufferParameteri, "glNamedFramebufferParameteri", "glNamedFramebufferParameteriEXT", "glNamedFramebufferParameteriOES", validContext);
//    setGLExtensionFuncPtr(glGetNamedFramebufferParameteriv, "glGetNamedFramebufferParameteriv", "glGetNamedFramebufferParameterivEXT", "glGetNamedFramebufferParameterivOES", validContext);
//    
//    setGLExtensionFuncPtr(glGenerateMipmap, "glGenerateMipmap", "glGenerateMipmapEXT", "glGenerateMipmapOES", validContext);
//    setGLExtensionFuncPtr(glBlitFramebuffer, "glBlitFramebuffer", "glBlitFramebufferEXT", "glBlitFramebufferOES", validContext);
//    setGLExtensionFuncPtr(glGetRenderbufferParameteriv, "glGetRenderbufferParameteriv", "glGetRenderbufferParameterivEXT", "glGetRenderbufferParameterivOES", validContext);
//    
//    
//    isFrameBufferObjectSupported =
//    glBindRenderbuffer != 0 &&
//    glDeleteRenderbuffers != 0 &&
//    glGenRenderbuffers != 0 &&
//    glRenderbufferStorage != 0 &&
//    glBindFramebuffer != 0 &&
//    glDeleteFramebuffers != 0 &&
//    glGenFramebuffers != 0 &&
//    glCheckFramebufferStatus != 0 &&
//    glFramebufferTexture2D != 0 &&
//    glFramebufferRenderbuffer != 0 &&
//    glGenerateMipmap != 0 &&
//    glGetRenderbufferParameteriv != 0 &&
//    ( OSG_GLES2_FEATURES || OSG_GLES1_FEATURES || isGLExtensionOrVersionSupported(contextID, "GL_EXT_framebuffer_object",3.0f) );
//    
//    
//    isPackedDepthStencilSupported = validContext &&
//    (OSG_GL3_FEATURES ||
//     (isGLExtensionSupported(contextID, "GL_EXT_packed_depth_stencil")) ||
//     (isGLExtensionSupported(contextID, "GL_OES_packed_depth_stencil")));
//    
//    //subroutine
//    osg::setGLExtensionFuncPtr(glGetSubroutineUniformLocation, "glGetSubroutineUniformLocation", validContext);
//    osg::setGLExtensionFuncPtr(glGetActiveSubroutineUniformName, "glGetActiveSubroutineUniformName", validContext);
//    osg::setGLExtensionFuncPtr(glGetActiveSubroutineUniformiv, "glGetActiveSubroutineUniformiv", validContext);
//    osg::setGLExtensionFuncPtr(glGetSubroutineIndex, "glGetSubroutineIndex", validContext);
//    osg::setGLExtensionFuncPtr(glGetActiveSubroutineName, "glGetActiveSubroutineName", validContext);
//    osg::setGLExtensionFuncPtr(glGetProgramStageiv, "glGetProgramStageiv", validContext);
//    osg::setGLExtensionFuncPtr(glUniformSubroutinesuiv, "glUniformSubroutinesuiv", validContext);
//    osg::setGLExtensionFuncPtr(glGetUniformSubroutineuiv, "glGetUniformSubroutineuiv", validContext);
//    
//    
//    // Sync
//    osg::setGLExtensionFuncPtr(glFenceSync, "glFenceSync", validContext);
//    osg::setGLExtensionFuncPtr(glIsSync, "glIsSync", validContext);
//    osg::setGLExtensionFuncPtr(glDeleteSync, "glDeleteSync", validContext);
//    osg::setGLExtensionFuncPtr(glClientWaitSync, "glClientWaitSync", validContext);
//    osg::setGLExtensionFuncPtr(glWaitSync, "glWaitSync", validContext);
//    osg::setGLExtensionFuncPtr(glGetSynciv, "glGetSynciv", validContext);
//    
//    // Indirect Rendering
//    osg::setGLExtensionFuncPtr(glDrawArraysIndirect, "glDrawArraysIndirect", "glDrawArraysIndirectEXT", validContext);
//    osg::setGLExtensionFuncPtr(glMultiDrawArraysIndirect, "glMultiDrawArraysIndirect", "glMultiDrawArraysIndirectEXT", validContext);
//    osg::setGLExtensionFuncPtr(glDrawElementsIndirect, "glDrawElementsIndirect", "glDrawElementsIndirectEXT", validContext);
//    osg::setGLExtensionFuncPtr(glMultiDrawElementsIndirect, "glMultiDrawElementsIndirect", "glMultiDrawElementsIndirectEXT", validContext);
//    
//    // ARB_sparse_texture
//    osg::setGLExtensionFuncPtr(glTexPageCommitment, "glTexPageCommitment","glTexPageCommitmentARB", "glTexPageCommitmentEXT", validContext);
//    
//    // Transform Feeedback
//    osg::setGLExtensionFuncPtr(glBeginTransformFeedback, "glBeginTransformFeedback", "glBeginTransformFeedbackEXT", validContext);
//    osg::setGLExtensionFuncPtr(glEndTransformFeedback, "glEndTransformFeedback", "glEndTransformFeedbackEXT", validContext);
//    osg::setGLExtensionFuncPtr(glTransformFeedbackVaryings, "glTransformFeedbackVaryings", "glTransformFeedbackVaryingsEXT", validContext);
//    osg::setGLExtensionFuncPtr(glGetTransformFeedbackVarying, "glGetTransformFeedbackVarying", "glGetTransformFeedbackVaryingEXT", validContext);
//    osg::setGLExtensionFuncPtr(glBindTransformFeedback, "glBindTransformFeedback", validContext);
//    osg::setGLExtensionFuncPtr(glDeleteTransformFeedbacks, "glDeleteTransformFeedbacks", validContext);
//    osg::setGLExtensionFuncPtr(glGenTransformFeedbacks, "glGenTransformFeedbacks", validContext);
//    osg::setGLExtensionFuncPtr(glIsTransformFeedback, "glIsTransformFeedback", validContext);
//    osg::setGLExtensionFuncPtr(glPauseTransformFeedback, "glPauseTransformFeedback", validContext);
//    osg::setGLExtensionFuncPtr(glResumeTransformFeedback, "glResumeTransformFeedback", validContext);
//    osg::setGLExtensionFuncPtr(glDrawTransformFeedback, "glDrawTransformFeedback", validContext);
//    osg::setGLExtensionFuncPtr(glDrawTransformFeedbackStream, "glDrawTransformFeedbackStream", validContext);
//    osg::setGLExtensionFuncPtr(glDrawTransformFeedbackInstanced, "glDrawTransformFeedbackInstanced", validContext);
//    osg::setGLExtensionFuncPtr(glDrawTransformFeedbackStreamInstanced, "glDrawTransformFeedbackStreamInstanced", validContext);
//    osg::setGLExtensionFuncPtr(glCreateTransformFeedbacks, "glCreateTransformFeedbacks", validContext);
//    osg::setGLExtensionFuncPtr(glTransformFeedbackBufferBase, "glTransformFeedbackBufferBase", validContext);
//    osg::setGLExtensionFuncPtr(glTransformFeedbackBufferRange, "glTransformFeedbackBufferRange", validContext);
//    osg::setGLExtensionFuncPtr(glGetTransformFeedbackiv, "glGetTransformFeedbackiv", validContext);
//    osg::setGLExtensionFuncPtr(glGetTransformFeedbacki_v, "glGetTransformFeedbacki_v", validContext);
//    osg::setGLExtensionFuncPtr(glGetTransformFeedbacki64_v, "glGetTransformFeedbacki64_v", validContext);
//    
//    //Vertex Array Object
//    osg::setGLExtensionFuncPtr(glGenVertexArrays, "glGenVertexArrays", "glGenVertexArraysOES", validContext);
//    osg::setGLExtensionFuncPtr(glBindVertexArray, "glBindVertexArray", "glBindVertexArrayOES", validContext);
//    osg::setGLExtensionFuncPtr(glDeleteVertexArrays, "glDeleteVertexArrays", "glDeleteVertexArraysOES", validContext);
//    osg::setGLExtensionFuncPtr(glIsVertexArray, "glIsVertexArray", "glIsVertexArrayOES", validContext);
//    
//    // OpenGL 4.3 / ARB_vertex_attrib_binding
//    isVertexAttribBindingSupported = validContext && (isGLExtensionOrVersionSupported(contextID, "GL_ARB_vertex_attrib_binding", 4.3f));
//    
//    osg::setGLExtensionFuncPtr(glBindVertexBuffer, "glBindVertexBuffer", "glBindVertexBufferOES", validContext);
//    osg::setGLExtensionFuncPtr(glVertexArrayVertexBuffer, "glVertexArrayVertexBuffer", "glVertexArrayVertexBufferOES", validContext);
//    osg::setGLExtensionFuncPtr(glVertexAttribBinding, "glVertexAttribBinding", "glVertexAttribBindingOES", validContext);
//    osg::setGLExtensionFuncPtr(glVertexArrayAttribBinding, "glVertexArrayAttribBinding", "glVertexArrayAttribBindingOES", validContext);
//    
//    osg::setGLExtensionFuncPtr(glVertexAttribFormat, "glVertexAttribBinding", "glVertexAttribBindingOES", validContext);
//    osg::setGLExtensionFuncPtr(glVertexAttribIFormat, "glVertexAttribBinding", "glVertexAttribBindingOES", validContext);
//    osg::setGLExtensionFuncPtr(glVertexAttribLFormat, "glVertexAttribLFormat", "glVertexAttribLFormatOES", validContext);
//    osg::setGLExtensionFuncPtr(glVertexArrayAttribFormat, "glVertexArrayAttribFormat", "glVertexArrayAttribFormatOES", validContext);
//    osg::setGLExtensionFuncPtr(glVertexArrayAttribIFormat, "glVertexArrayAttribIFormat", "glVertexArrayAttribIFormatOES", validContext);
//    osg::setGLExtensionFuncPtr(glVertexArrayAttribLFormat, "glVertexArrayAttribLFormat", "glVertexArrayAttribLFormatOES", validContext);
//    
//    // MultiDrawArrays
//    setGLExtensionFuncPtr(glMultiDrawArrays, "glMultiDrawArrays", "glMultiDrawArraysEXT", validContext);
//    setGLExtensionFuncPtr(glMultiDrawElements, "glMultiDrawElements", "glMultiDrawElementsEXT");
//    setGLExtensionFuncPtr(glDrawArraysInstancedBaseInstance, "glDrawArraysInstancedBaseInstance", "glDrawArraysInstancedBaseInstanceEXT");
//    setGLExtensionFuncPtr(glDrawElementsInstancedBaseInstance, "glDrawElementsInstancedBaseInstance", "glDrawElementsInstancedBaseInstanceEXT");
//    setGLExtensionFuncPtr(glDrawElementsInstancedBaseVertexBaseInstance, "glDrawElementsInstancedBaseVertexBaseInstance", "glDrawElementsInstancedBaseVertexBaseInstanceEXT");
//    
//    setGLExtensionFuncPtr(glDrawRangeElements, "glDrawRangeElements");
//    setGLExtensionFuncPtr(glDrawElementsBaseVertex, "glDrawElementsBaseVertex", "glDrawElementsBaseVertexEXT");
//    setGLExtensionFuncPtr(glDrawRangeElementsBaseVertex, "glDrawRangeElementsBaseVertex", "glDrawRangeElementsBaseVertexEXT");
//    setGLExtensionFuncPtr(glDrawElementsInstancedBaseVertex, "glDrawElementsInstancedBaseVertex", "glDrawElementsInstancedBaseVertexEXT");
//    setGLExtensionFuncPtr(glMultiDrawElementsBaseVertex, "glMultiDrawElementsBaseVertex", "glMultiDrawElementsBaseVertexEXT");
//    setGLExtensionFuncPtr(glProvokingVertex, "glProvokingVertex", "glProvokingVertexEXT");
//    
//    setGLExtensionFuncPtr(glBeginConditionalRender, "glBeginConditionalRender", "glBeginConditionalRenderEXT");
//    setGLExtensionFuncPtr(glEndConditionalRender, "glEndConditionalRender", "glEndConditionalRenderEXT");
//    
//    // ViewportArray
//    isViewportArraySupported = validContext && (isGLExtensionOrVersionSupported(contextID, "GL_ARB_viewport_array", 4.1f));
//    
//    osg::setGLExtensionFuncPtr(glViewportArrayv, "glViewportArrayv", validContext);
//    osg::setGLExtensionFuncPtr(glViewportIndexedf, "glViewportIndexedf", validContext);
//    osg::setGLExtensionFuncPtr(glViewportIndexedfv, "glViewportIndexedfv", validContext);
//    osg::setGLExtensionFuncPtr(glScissorArrayv, "glScissorArrayv", validContext);
//    osg::setGLExtensionFuncPtr(glScissorIndexed, "glScissorIndexed", validContext);
//    osg::setGLExtensionFuncPtr(glScissorIndexedv, "glScissorIndexedv", validContext);
//    osg::setGLExtensionFuncPtr(glDepthRangeArrayv, "glDepthRangeArrayv", validContext);
//    osg::setGLExtensionFuncPtr(glDepthRangeIndexed, "glDepthRangeIndexed", validContext);
//    osg::setGLExtensionFuncPtr(glDepthRangeIndexedf, "glDepthRangeIndexedfOES", "glDepthRangeIndexedfNV", validContext);
//    osg::setGLExtensionFuncPtr(glGetFloati_v, "glGetFloati_v", validContext);
//    osg::setGLExtensionFuncPtr(glGetDoublei_v, "glGetDoublei_v", validContext);
//    osg::setGLExtensionFuncPtr(glGetIntegerIndexedvEXT, "glGetIntegerIndexedvEXT", validContext);
//    osg::setGLExtensionFuncPtr(glEnableIndexedEXT, "glEnableIndexedEXT", validContext);
//    osg::setGLExtensionFuncPtr(glDisableIndexedEXT, "glDisableIndexedEXT", validContext);
//    osg::setGLExtensionFuncPtr(glIsEnabledIndexedEXT, "glIsEnabledIndexedEXT", validContext);
//    
//    setGLExtensionFuncPtr(glClientActiveTexture,"glClientActiveTexture","glClientActiveTextureARB", validContext);
//    setGLExtensionFuncPtr(glActiveTexture, "glActiveTexture","glActiveTextureARB", validContext);
//    setGLExtensionFuncPtr(glFogCoordPointer, "glFogCoordPointer","glFogCoordPointerEXT", validContext);
//    setGLExtensionFuncPtr(glSecondaryColorPointer, "glSecondaryColorPointer","glSecondaryColorPointerEXT", validContext);
//    
//    if (validContext)
//    {
//        if (osg::getGLVersionNumber() >= 2.0 || osg::isGLExtensionSupported(contextID, "GL_ARB_vertex_shader") || OSG_GLES2_FEATURES || OSG_GLES3_FEATURES || OSG_GL3_FEATURES)
//        {
//            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&glMaxTextureUnits);
//#ifdef OSG_GL_FIXED_FUNCTION_AVAILABLE
//            glGetIntegerv(GL_MAX_TEXTURE_COORDS, &glMaxTextureCoords);
//#else
//            glMaxTextureCoords = glMaxTextureUnits;
//#endif
//        }
//#ifdef GL_MAX_TEXTURE_UNITS
//        else if ( osg::getGLVersionNumber() >= 1.3 ||
//                 osg::isGLExtensionSupported(contextID,"GL_ARB_multitexture") ||
//                 osg::isGLExtensionSupported(contextID,"GL_EXT_multitexture") ||
//                 OSG_GLES1_FEATURES)
//        {
//            GLint maxTextureUnits = 0;
//            glGetIntegerv(GL_MAX_TEXTURE_UNITS,&maxTextureUnits);
//            glMaxTextureUnits = maxTextureUnits;
//            glMaxTextureCoords = maxTextureUnits;
//        }
//#endif
//        else
//        {
//            glMaxTextureUnits = 1;
//            glMaxTextureCoords = 1;
//        }
//    }
//    else
//    {
//        glMaxTextureUnits = 0;
//        glMaxTextureCoords = 0;
//    }
//    
//    osg::setGLExtensionFuncPtr(glObjectLabel, "glObjectLabel", validContext);
//    
//}
