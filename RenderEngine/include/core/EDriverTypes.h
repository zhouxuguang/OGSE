//
//  DriverType.h
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/6.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef DriverType_h
#define DriverType_h

//! An enum for all types of drivers the Render Engine supports.
enum E_DRIVER_TYPE
{
    //! Null driver, useful for applications to run the engine without visualisation.
    /** The null device is able to load textures, but does not
     render and display any graphics. */
    EDT_NULL,
    
    //! OpenGL device, available on most platforms.
    /** Performs hardware accelerated rendering of 3D and 2D
     primitives. */
    EDT_OPENGL,
    
    //! OpenGL-ES device, for embedded and mobile systems
    EDT_OGLES1,
    
    //! OpenGL-ES 2 device, for embedded and mobile systems
    EDT_OGLES2,
    
    //! vulakn driver
    EDT_VULKAN,
    
    //! METAL driver
    EDT_METAL
};


#endif /* DriverType_h */
