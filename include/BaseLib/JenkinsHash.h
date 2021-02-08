//
//  JenkinsHash.h
//  baselib
//
//  Created by zhouxuguang on 16/7/20.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#ifndef OGSE_BASELIB_JENKINSHASH_INCLUDE_H
#define OGSE_BASELIB_JENKINSHASH_INCLUDE_H

#include "PreCompile.h"

namespace baselib 
{
    
    typedef eUInt32 uint32_t;
    typedef eByte uint8_t;
    typedef uint32_t hash_t;

    BASELIB_API uint32_t JenkinsHashMix(uint32_t hash, uint32_t data);

    BASELIB_API uint32_t JenkinsHashMixBytes(uint32_t hash, const uint8_t* bytes, size_t size);

    template <typename TKey>
    hash_t hash_type(const TKey& key)
    {
        return JenkinsHashMixBytes(0,(const uint8_t*)&key,sizeof(key));
    }
    
}


#endif /* OGSE_BASELIB_JENKINSHASH_INCLUDE_H */
