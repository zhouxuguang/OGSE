//
//  DeviceType.h
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/6.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef DeviceType_h
#define DeviceType_h

//! An enum for the different device types supported by the Render Engine.
enum E_DEVICE_TYPE
{
    
    //! A device native to Microsoft Windows
    /** This device uses the Win32 API and works in all versions of Windows. */
    EIDT_WIN32,
    
    //! A device native to Windows CE devices
    /** This device works on Windows Mobile, Pocket PC and Microsoft SmartPhone devices */
    EIDT_WINCE,
    
    //! A device native to Mac OSX
    /** This device uses Apple's Cocoa API and works in Mac OSX 10.2 and above. */
    EIDT_OSX,
    
    //! A device native to the IPhone/IPod touch
    /** This device should be used with the OpenGL-ES driver. */
    EIDT_IPHONE,
    
    //! A device native to Android mobiles
    /** This device should be used with the OpenGL-ES driver. */
    EIDT_ANDROID,
    
    //! A device native to stub
    /** This device should be used with the OpenGL-ES driver. */
    EIDT_STUB
};


#endif /* DeviceType_h */
