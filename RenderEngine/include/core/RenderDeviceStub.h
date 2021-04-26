//
//  RenderDeviceStub.hpp
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/6.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef RenderDeviceStub_hpp
#define RenderDeviceStub_hpp

//装对象

#include "RenderDevice.h"

class RenderDeviceStub : public RenderDevice
{
public:
    virtual RenderDriver* GetRenderDriver()
    {
        return nullptr;
    }

    virtual void CloseDevice()
    {
        return;
    }
    
    virtual E_DEVICE_TYPE GetType() const
    {
        return EIDT_STUB;
    }
};

#endif /* RenderDeviceStub_hpp */
