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
#define RENDERENGINE_APPLE_PLATFORM

#endif

#include <stdio.h>

#endif /* RenderEngineDefine_hpp */
