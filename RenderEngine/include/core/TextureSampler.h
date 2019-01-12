//
//  TextureSampler.h
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/12.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef TextureSampler_hpp
#define TextureSampler_hpp

#include <stdio.h>

//纹理过滤的枚举
enum TextureFilter
{
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear,
};

//纹理包装枚举
enum TextureWrap
{
    Clamp,
    Repeat,
    MirroredRepeat
};

class TextureSampler
{
    TextureSampler(TextureFilter minificationFilter,
                   TextureFilter magnificationFilter,
                   TextureWrap wrapS,
                   TextureWrap wrapT,
                   float maximumAnistropy)
    {
        mMinificationFilter = minificationFilter;
        mMagnificationFilter = magnificationFilter;
        mWrapS = wrapS;
        mWrapT = wrapT;
        mMaximumAnistropy = maximumAnistropy;
    };
    
    ~TextureSampler();
    
private:
    TextureFilter mMinificationFilter;
    TextureFilter mMagnificationFilter;
    TextureWrap mWrapS;
    TextureWrap mWrapT;
    float mMaximumAnistropy;
};

#endif /* TextureSampler_hpp */
