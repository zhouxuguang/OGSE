//
//  GLES3Untily.hpp
//  RenderEngine
//
//  Created by zhouxuguang on 2021/4/28.
//  Copyright © 2021 Zhou,Xuguang. All rights reserved.
//

#ifndef GLES3Untily_hpp
#define GLES3Untily_hpp

#include <stdio.h>
#include "GLConfiguration.h"

//纹理格式枚举
typedef uint32_t TextureFormat;
enum
{
    kTexFormatAlpha8 = 1,
    kTexFormatARGB4444 = 2,
    kTexFormatRGB24 = 3,
    kTexFormatRGBA32 = 4,
    kTexFormatARGB32 = 5,
    kTexFormatARGBFloat = 6, // only for internal use at runtime
    kTexFormatRGB565 = 7,
    kTexFormatBGR24 = 8,
    // This one is for internal use; storage is 16 bits/pixel; samples
    // as Alpha (OpenGL) or RGB (D3D9). Can be reduced to 8 bit alpha/luminance on lower hardware.
    // Why it's not Luminance on GL: for some reason alpha seems to be faster.
    kTexFormatAlphaLum16 = 9,
    kTexFormatDXT1 = 10,
    kTexFormatDXT3 = 11,
    kTexFormatDXT5 = 12,
    kTexFormatRGBA4444 = 13,

    kTexFormatPCCount = 14,

    kTexReserved1 = 14, // Use reservedX when adding a new 'PC' texture format
    kTexReserved2 = 15,
    kTexReserved3 = 16,
    kTexReserved4 = 17,
    kTexReserved5 = 18,
    kTexReserved6 = 19,
    // [20..27] used to be Wii-specific formats before Unity 4.0
    kTexReserved11 = 28,
    kTexReserved12 = 29,

    // iPhone
    kTexFormatPVRTC_RGB2 = 30,
    kTexFormatPVRTC_RGBA2 = 31,

    kTexFormatPVRTC_RGB4 = 32,
    kTexFormatPVRTC_RGBA4 = 33,

    kTexFormatETC_RGB4 = 34,

    kTexFormatATC_RGB4 = 35,
    kTexFormatATC_RGBA8 = 36,

    // Pixels returned by iPhone camera
    kTexFormatBGRA32 = 37,

    kTexFormatFlashATF_RGB_DXT1 = 38,
    kTexFormatFlashATF_RGBA_JPG = 39,
    kTexFormatFlashATF_RGB_JPG = 40,

    // EAC and ETC2 compressed formats, mandated by OpenGL ES 3.0
    kTexFormatEAC_R = 41,
    kTexFormatEAC_R_SIGNED = 42,
    kTexFormatEAC_RG = 43,
    kTexFormatEAC_RG_SIGNED = 44,
    kTexFormatETC2_RGB = 45,
    kTexFormatETC2_RGBA1 = 46,
    kTexFormatETC2_RGBA8 = 47,

    // ASTC. The RGB and RGBA formats are internally identical, we just need to carry the has-alpha information somehow
    kTexFormatASTC_RGB_4x4 = 48,
    kTexFormatASTC_RGB_5x5 = 49,
    kTexFormatASTC_RGB_6x6 = 50,
    kTexFormatASTC_RGB_8x8 = 51,
    kTexFormatASTC_RGB_10x10 = 52,
    kTexFormatASTC_RGB_12x12 = 53,

    kTexFormatASTC_RGBA_4x4 = 54,
    kTexFormatASTC_RGBA_5x5 = 55,
    kTexFormatASTC_RGBA_6x6 = 56,
    kTexFormatASTC_RGBA_8x8 = 57,
    kTexFormatASTC_RGBA_10x10 = 58,
    kTexFormatASTC_RGBA_12x12 = 59,

    kTexFormatTotalCount    = 60 // keep this last!
};

class TransferFormatGLES30
{
public:
    uint32_t    format;
    uint32_t    dataType;

    TransferFormatGLES30 (uint32_t format, uint32_t dataType)
        : format    (format)
        , dataType    (dataType)
    {
    }
};

//// Map RenderTextureFormat to closest GL sized internal format.
//uint32_t GetColorFormatGLES30 (RenderTextureFormat format);
//
//// Get closest depth internal format.
//uint32_t GetDepthOnlyFormatGLES30 (DepthBufferFormat format);
//
//// Get closest depth&stencil internal format.
//uint32_t GetDepthStencilFormatGLES30 (DepthBufferFormat format);

// Get transfer (upload) format, dataType pair for internal format.
TransferFormatGLES30 GetTransferFormatGLES30 (uint32_t internalFormat);

// Get default framebuffer (0) internal format (guess based on bits)
uint32_t GetDefaultFramebufferColorFormatGLES30 (void);

// Get default framebuffer (0) depth format
uint32_t GetDefaultFramebufferDepthFormatGLES30 (void);

#endif /* GLES3Untily_hpp */
