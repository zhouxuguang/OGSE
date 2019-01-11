//
//  RenderDevice.hpp
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/6.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef RenderDevice_hpp
#define RenderDevice_hpp

#include "EDeviceTypes.h"

class RenderDriver;

class RenderDevice
{
public:
    
    virtual bool Run() = 0;
    
    //! Provides access to the driver for drawing 3d and 2d geometry.
    /** \return Pointer the driver. */
    virtual RenderDriver* GetRenderDriver() = 0;
    
    //! Get the current color format of the window
    /** \return Color format of the window. */
    //virtual video::ECOLOR_FORMAT GetColorFormat() const = 0;
    
    //! Notifies the device that it should close itself.
    /** run() will always return false after closeDevice() was called. */
    virtual void CloseDevice() = 0;
    
    //! Get the type of the device.
    /** This allows the user to check which windowing system is currently being
     used. */
    virtual E_DEVICE_TYPE GetType() const = 0;
};

#endif /* RenderDevice_hpp */
