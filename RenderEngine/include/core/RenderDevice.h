//
//  RenderDevice.hpp
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/6.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef RenderDevice_hpp
#define RenderDevice_hpp

#include <stdio.h>

class RenderDriver;

class RenderDevice
{
public:
    
    virtual bool run() = 0;
    
    //! Provides access to the driver for drawing 3d and 2d geometry.
    /** \return Pointer the driver. */
    virtual RenderDriver* getRenderDriver() = 0;
    
    virtual bool isWindowActive() const = 0;
    
    //! Checks if the Irrlicht window has focus
    /** \return True if window has focus. */
    virtual bool isWindowFocused() const = 0;
    
    //! Checks if the Irrlicht window is minimized
    /** \return True if window is minimized. */
    virtual bool isWindowMinimized() const = 0;
    
    //! Checks if the Irrlicht window is running in fullscreen mode
    /** \return True if window is fullscreen. */
    virtual bool isFullscreen() const = 0;
    
    //! Get the current color format of the window
    /** \return Color format of the window. */
    //virtual video::ECOLOR_FORMAT getColorFormat() const = 0;
    
    //! Notifies the device that it should close itself.
    /** IrrlichtDevice::run() will always return false after closeDevice() was called. */
    virtual void closeDevice() = 0;
    
    //! Sets if the window should be resizable in windowed mode.
    /** The default is false. This method only works in windowed
     mode.
     \param resize Flag whether the window should be resizable. */
    virtual void setResizable(bool resize=false) = 0;
    
    //! Minimizes the window if possible.
    virtual void minimizeWindow() =0;
    
    //! Maximizes the window if possible.
    virtual void maximizeWindow() =0;
    
    //! Get the type of the device.
    /** This allows the user to check which windowing system is currently being
     used. */
    //virtual E_DEVICE_TYPE getType() const = 0;
    
    //! Check if a driver type is supported by the engine.
    /** Even if true is returned the driver may not be available
     for a configuration requested when creating the device. */
//    static bool isDriverSupported(video::E_DRIVER_TYPE driver)
//    {
//        switch (driver)
//        {
//            case video::EDT_NULL:
//                return true;
//            case video::EDT_SOFTWARE:
//#ifdef _IRR_COMPILE_WITH_SOFTWARE_
//                return true;
//#else
//                return false;
//#endif
//            case video::EDT_BURNINGSVIDEO:
//#ifdef _IRR_COMPILE_WITH_BURNINGSVIDEO_
//                return true;
//#else
//                return false;
//#endif
//            case video::EDT_DIRECT3D8:
//#ifdef _IRR_COMPILE_WITH_DIRECT3D_8_
//                return true;
//#else
//                return false;
//#endif
//            case video::EDT_DIRECT3D9:
//#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
//                return true;
//#else
//                return false;
//#endif
//            case video::EDT_OPENGL:
//#ifdef _IRR_COMPILE_WITH_OPENGL_
//                return true;
//#else
//                return false;
//#endif
//            default:
//                return false;
//        }
//    }
};

#endif /* RenderDevice_hpp */
