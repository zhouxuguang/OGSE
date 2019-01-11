//
//  RenderEngine.h
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/6.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef RenderEngine_hpp
#define RenderEngine_hpp

#include <stdlib.h>
#include "RenderDevice.h"
#include "EDriverTypes.h"

struct RenderDeviceCreationParameters
{
    E_DEVICE_TYPE DeviceType;
    E_DRIVER_TYPE DriverType;
    int nWidth;
    int nHeight;
    unsigned int Bits;
    bool Fullscreen;
    bool Stencilbuffer;
    bool Vsync;
};

RenderDevice* CreateDevice(E_DRIVER_TYPE driverType,
                           const int nWidth, const int nHeight,
                           unsigned int bits, bool fullscreen,
                           bool stencilbuffer, bool vsync);

#endif /* RenderEngine_hpp */
