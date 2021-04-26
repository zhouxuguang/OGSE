/*
 * GLConfiguration.cpp
 *
 *  Created on: 2016-11-10
 *      Author: zhouxuguang
 */

#include "GLConfiguration.h"

int IsETCSupported()
{
    int count = 0;
    extern int GLEW_OES_compressed_ETC1_RGB8_texture;
    //GLEW_OES_compressed_ETC1_RGB8_texture = 0;
    
    glGetIntegerv( GL_NUM_COMPRESSED_TEXTURE_FORMATS, &count );
    if( count > 0 )
    {
        GLint* formats = (GLint*)calloc( count, sizeof(GLint) );
        
        glGetIntegerv( GL_COMPRESSED_TEXTURE_FORMATS, formats );
        
        int index;
        for( index = 0; index < count; index++ )
        {
            switch( formats[index] )
            {
//                case GL_ETC1_RGB8_OES:
//                    GLEW_OES_compressed_ETC1_RGB8_texture = 1;
//                    break;
            }
        }
        
        free( formats );
    }
    return 1;
}

GLConfiguration *GLConfiguration::s_sharedConfiguration = NULL;

GLConfiguration::GLConfiguration()
{
	// TODO Auto-generated constructor stub

}

GLConfiguration::~GLConfiguration()
{
	// TODO Auto-generated destructor stub
}

GLConfiguration* GLConfiguration::GetInstance()
{
    if (NULL == s_sharedConfiguration)
    {
        s_sharedConfiguration = new (std::nothrow) GLConfiguration();
        s_sharedConfiguration->GatherGPUInfo();
    }
    
    return s_sharedConfiguration;
}

void GLConfiguration::GatherGPUInfo()
{
    glExtensions = (char *)glGetString(GL_EXTENSIONS);
    
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &maxModelviewStackDepth);
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
    
    supportsETC1 = CheckForGLExtension("GL_OES_compressed_ETC1_RGB8_texture");  //OES_compressed_ETC1_RGB8_texture
    
    supportsS3TC = CheckForGLExtension("GL_EXT_texture_compression_s3tc");
    
    supportsATITC = CheckForGLExtension("GL_AMD_compressed_ATC_texture") || CheckForGLExtension("GL_ATI_texture_compression_atitc");
    
    supportsPVRTC = CheckForGLExtension("GL_IMG_texture_compression_pvrtc");

    supports3DC = CheckForGLExtension("GL_AMD_compressed_3DC_texture");
    
    supportsNPOT = CheckForGLExtension("GL_OES_texture_npot") ||
            CheckForGLExtension("GL_IMG_texture_npot") || CheckForGLExtension("GL_APPLE_texture_2D_limited_npot");
    
    supportsBGRA8888 = CheckForGLExtension("GL_IMG_texture_format_BGRA888");
    
    supportsDiscardFramebuffer = CheckForGLExtension("GL_EXT_discard_framebuffer");
    
    supportsShareableVAO = CheckForGLExtension("vertex_array_object");
    
    supportsOESMapBuffer = CheckForGLExtension("GL_OES_mapbuffer");
    
    supportsOESDepth24 = CheckForGLExtension("GL_OES_depth24");
    
    supportsOESPackedDepthStencil = CheckForGLExtension("GL_OES_packed_depth_stencil");
    
    supportsDerivative = CheckForGLExtension("GL_OES_standard_derivatives");
}

bool GLConfiguration::CheckForGLExtension(const std::string &searchName) const
{
    return  (glExtensions && strstr(glExtensions, searchName.c_str() ) ) ? true : false;
}

bool GLConfiguration::IsSupportsNPOT() const
{
    return supportsNPOT;
}

bool GLConfiguration::IsSupportsPVRTC() const
{
    return supportsPVRTC;
}

bool GLConfiguration::IsSupportsETC() const
{
#ifdef GL_ETC1_RGB8_OES
    return supportsETC1;
#else
    return false;
#endif
}

bool GLConfiguration::IsSupportsS3TC() const
{
#ifdef GL_EXT_texture_compression_s3tc
    return supportsS3TC;
#else
    return false;
#endif
}

bool GLConfiguration::IsSupports3DC() const
{
    return supports3DC;
}

bool GLConfiguration::IsSupportsShareableVAO() const
{
    return supportsShareableVAO;
}

bool GLConfiguration::IsSupportsATITC() const
{
    return supportsATITC;
}

bool GLConfiguration::IsSupportsBGRA8888() const
{
    return supportsBGRA8888;
}

bool GLConfiguration::IsSupportsDiscardFramebuffer() const
{
    return supportsDiscardFramebuffer;
}

bool GLConfiguration::IsSupportsMapBuffer() const
{
    return supportsOESMapBuffer;
}

bool GLConfiguration::IsSupportsOESDepth24() const
{
    return supportsOESDepth24;
}

bool GLConfiguration::IsSupportsOESPackedDepthStencil() const
{
    return supportsOESPackedDepthStencil;
}

bool GLConfiguration::IsSupportDerivative() const
{
    return supportsDerivative;
}
