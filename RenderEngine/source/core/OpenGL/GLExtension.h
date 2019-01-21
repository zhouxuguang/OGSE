//
//  GLExtension.h
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/12.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef GLExtension_h
#define GLExtension_h

#include <stdio.h>

/** Return the address of the specified OpenGL/GLES function.
 * Return NULL if function not supported by OpenGL library.
 * Note, glGLExtensionFuncPtr is declared inline so that the code
 * is compiled locally to the calling code.  This should get by Windows'
 * dumb implementation of having different GL function ptr's for each
 * library when linked to it.
 */
void* getGLExtensionFuncPtr(const char *funcName);

/** Return the address of the specified OpenGL function. If not found then
 * check a second function name, if this fails then return NULL as function is
 * not supported by OpenGL library. This is used for checking something
 * like glActiveTexture (which is in OGL1.3) or glActiveTextureARB.
 */
inline void* getGLExtensionFuncPtr(const char *funcName, const char *fallbackFuncName)
{
    void* ptr = getGLExtensionFuncPtr(funcName);
    return (ptr!=0) ? ptr : getGLExtensionFuncPtr(fallbackFuncName);
}

/** Return the address of the specified OpenGL function. If not found then
 * check a second function name, if this fails then return NULL as function is
 * not supported by OpenGL library. This is used for checking something
 * like glActiveTexture (which is in OGL1.3) or glActiveTextureARB.
 */
inline void* getGLExtensionFuncPtr(const char *funcName1, const char *funcName2, const char *funcName3)
{
    void* ptr = getGLExtensionFuncPtr(funcName1);
    return (ptr!=0) ? ptr : getGLExtensionFuncPtr(funcName2, funcName3);
}

template<typename T, typename R>
bool convertPointer(T& dest, R src)
{
    memcpy(&dest, &src, sizeof(src));
    return src!=0;
}

template<typename T, typename R>
T convertPointerType(R src)
{
    T dest;
    memcpy(&dest, &src, sizeof(src));
    return dest;
}

template<typename T>
bool setGLExtensionFuncPtr(T& t, const char* str1, bool validContext=true)
{
    return convertPointer(t, validContext ? getGLExtensionFuncPtr(str1) : 0);
}

template<typename T>
bool setGLExtensionFuncPtr(T& t, const char* str1, const char* str2, bool validContext=true)
{
    return convertPointer(t, validContext ? getGLExtensionFuncPtr(str1, str2) : 0);
}

template<typename T>
bool setGLExtensionFuncPtr(T& t, const char* str1, const char* str2, const char* str3, bool validContext=true)
{
    return convertPointer(t, validContext ? getGLExtensionFuncPtr(str1, str2, str3) : 0);
}

#endif /* GLExtension_h */
