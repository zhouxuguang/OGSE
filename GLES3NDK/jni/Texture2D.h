//
//  Texture2D.hpp
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/7/23.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#ifndef Texture2D_hpp
#define Texture2D_hpp

#include <stdio.h>

enum PixelFormat
{
    AUTO,
    
    BGRA8888,

    RGBA8888,

    RGB888,

    RGB565,

    A8,

    I8,

    AI88,

    RGBA4444,

    RGB5A1,

    PVRTC4,

    PVRTC4A,

    PVRTC2,

    PVRTC2A,

    ETC,

    S3TC_DXT1,

    S3TC_DXT3,

    S3TC_DXT5,

    ATC_RGB,

    ATC_EXPLICIT_ALPHA,

    ATC_INTERPOLATED_ALPHA,

    DEFAULT = AUTO,
    
    NONE = -1
};

class Texture2D
{
    //
    
public:
    //
};

#endif /* Texture2D_hpp */
