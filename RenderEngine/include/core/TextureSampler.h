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
enum SamplerAddressMode
{
    Clamp,
    Repeat,
    MirroredRepeat
};

class TextureSampler
{
    TextureSampler(TextureFilter minificationFilter,
                   TextureFilter magnificationFilter,
                   SamplerAddressMode wrapS,
                   SamplerAddressMode wrapT,
                   float maximumAnistropy)
    {
        mMinificationFilter = minificationFilter;
        mMagnificationFilter = magnificationFilter;
        mSAddressMode = wrapS;
        mTAddressMode = wrapT;
        mMaximumAnistropy = maximumAnistropy;
    };
    
    ~TextureSampler();
    
private:
    TextureFilter mMinificationFilter;
    TextureFilter mMagnificationFilter;
    SamplerAddressMode mSAddressMode;
    SamplerAddressMode mTAddressMode;
    float mMaximumAnistropy;
};

#endif /* TextureSampler_hpp */
