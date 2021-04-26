//
//  Texture.h
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/7/26.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include "GLPrecompile.h"

class Texture
{
public:
    Texture();
    
    ~Texture();
    
    GLuint GenerateTexture2D(GLenum target, GLuint surfaceWidth, GLuint surfaceHeight,
                             GLint  internalFormat,
                             GLenum type ,
                             GLenum format ,
                             void *data = 0,
                             bool buildMipMap = true,
                             GLint mipMp_level = 0,
                             GLint border = 0,
                             GLint Swrap_mode = GL_CLAMP_TO_EDGE,
                             GLint Twrap_mode = GL_CLAMP_TO_EDGE,
                             GLint min_filter = GL_LINEAR,
                             GLint mag_filter = GL_LINEAR_MIPMAP_LINEAR
                             );
    
    GLuint GenerateTexture3D(GLenum target, GLuint surfaceWidth, GLuint surfaceHeight,
                             GLuint surfaceDepth,
                             GLint  internalFormat,
                             GLenum type ,
                             GLenum format ,
                             void *data = 0,
                             bool buildMipMap = true,
                             GLint mipMp_level = 0,
                             GLint border = 0,
                             GLint Rwrap_mode = GL_CLAMP_TO_EDGE,
                             GLint Swrap_mode = GL_CLAMP_TO_EDGE,
                             GLint Twrap_mode = GL_CLAMP_TO_EDGE,
                             GLint min_filter = GL_LINEAR,
                             GLint mag_filter = GL_LINEAR_MIPMAP_LINEAR
                             );
    
    GLuint GetTextureID() { return texID; }
    
    void DeleteTexture();
    
    GLenum GetTargetType() { return target; }
private:
    
    /**
     GL ID
     */
    GLuint texID;
    
    /**
     Depth of the Texture
     */
    GLuint depth;
    
    /**
     Width of the Texture
     */
    GLuint width;
    
    /**
     Height  of the Texture
     */
    GLuint height;
    
    /**
     texture internal format
     */
    GLint internalFormat;
    
    /**
     target type ( texture 2D)
     */
    GLenum target;
    
    /**
     minification texture filter
     */
    GLint miniFilter;
    
    /**
     Magnification texture filter
     */
    
    GLint magniFilter;
    
    /**
     Texture's R wrap mode
     */
    GLint texWrapModeR;
    
    /**
     Texture's S wrap mode
     */
    GLint texWrapModeS;
    
    /**
     Texture's T wrap mode
     */
    GLint texWrapModeT;
};


#endif /* Texture_hpp */
