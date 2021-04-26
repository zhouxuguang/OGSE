//
//  RenderEngine.cpp
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/6.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#include "RenderEngine.h"
#include "EDriverTypes.h"
#include "RenderDevice.h"
#include "EDeviceTypes.h"

#ifdef __APPLE__
#include "RenderDeviceMacOSX.h"
#endif

RenderDevice* CreateDeviceEx(const RenderDeviceCreationParameters& params);

//! stub for calling createDeviceEx
RenderDevice* CreateDevice(E_DRIVER_TYPE driverType,
                            const int nWidth, const int nHeight,
                            unsigned int bits, bool fullscreen,
                            bool stencilbuffer, bool vsync)
{
    RenderDeviceCreationParameters p;
    p.DriverType = driverType;
    p.nWidth = nWidth;
    p.nHeight = nHeight;
    p.Bits = bits;
    p.Fullscreen = fullscreen;
    p.Stencilbuffer = stencilbuffer;
    p.Vsync = vsync;
    
#ifdef __APPLE__
    p.DeviceType = EIDT_OSX;
#endif
    
    return CreateDeviceEx(p);
}

RenderDevice* CreateDeviceEx(const RenderDeviceCreationParameters& params)
{
    
    RenderDevice* dev = 0;
    
#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_
    if (params.DeviceType == EIDT_WIN32 || (!dev && params.DeviceType == EIDT_BEST))
        dev = new CIrrDeviceWin32(params);
#endif
    
#ifdef __APPLE__
    if (params.DeviceType == EIDT_OSX)
        dev = new RenderDeviceMacOSX(params);
#endif
    
#ifdef _IRR_COMPILE_WITH_WINDOWS_CE_DEVICE_
    if (params.DeviceType == EIDT_WINCE || (!dev && params.DeviceType == EIDT_BEST))
        dev = new CIrrDeviceWinCE(params);
#endif
    
#ifdef _IRR_COMPILE_WITH_X11_DEVICE_
    if (params.DeviceType == EIDT_X11 || (!dev && params.DeviceType == EIDT_BEST))
        dev = new CIrrDeviceLinux(params);
#endif
    
#ifdef _IRR_COMPILE_WITH_SDL_DEVICE_
    if (params.DeviceType == EIDT_SDL || (!dev && params.DeviceType == EIDT_BEST))
        dev = new CIrrDeviceSDL(params);
#endif
    
#ifdef _IRR_COMPILE_WITH_FB_DEVICE_
    if (params.DeviceType == EIDT_FRAMEBUFFER || (!dev && params.DeviceType == EIDT_BEST))
        dev = new CIrrDeviceFB(params);
#endif
    
#ifdef _IRR_COMPILE_WITH_CONSOLE_DEVICE_
    if (params.DeviceType == EIDT_CONSOLE || (!dev && params.DeviceType == EIDT_BEST))
        dev = new CIrrDeviceConsole(params);
#endif
    
//    if (dev && !dev->getVideoDriver() && params.DriverType != video::EDT_NULL)
//    {
//        dev->closeDevice(); // destroy window
//        dev->run(); // consume quit message
//        dev->drop();
//        dev = 0;
//    }
    
    return dev;
}
