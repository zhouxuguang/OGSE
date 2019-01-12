//
//  EColorFormat.h
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/12.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef EColorFormat_h
#define EColorFormat_h


enum ECOLOR_FORMAT
{
    //! 16 bit color format used by the software driver.
    /** It is thus preferred by all other irrlicht engine video drivers.
     There are 5 bits for every color component, and a single bit is left
     for alpha information. */
    ECF_A1R5G5B5 = 0,
    
    //! Standard 16 bit color format.
    ECF_R5G6B5,
    
    //! 24 bit color, no alpha channel, but 8 bit for red, green and blue.
    ECF_R8G8B8,
    
    //! Default 32 bit color format. 8 bits are used for every component: red, green, blue and alpha.
    ECF_A8R8G8B8,
    
    /** Floating Point formats. The following formats may only be used for render target textures. */
    
    //! 16 bit floating point format using 16 bits for the red channel.
    ECF_R16F,
    
    //! 32 bit floating point format using 16 bits for the red channel and 16 bits for the green channel.
    ECF_G16R16F,
    
    //! 64 bit floating point format 16 bits are used for the red, green, blue and alpha channels.
    ECF_A16B16G16R16F,
    
    //! 32 bit floating point format using 32 bits for the red channel.
    ECF_R32F,
    
    //! 64 bit floating point format using 32 bits for the red channel and 32 bits for the green channel.
    ECF_G32R32F,
    
    //! 128 bit floating point format. 32 bits are used for the red, green, blue and alpha channels.
    ECF_A32B32G32R32F,
    
    //! Unknown color format:
    ECF_UNKNOWN
};


#endif /* EColorFormat_h */
