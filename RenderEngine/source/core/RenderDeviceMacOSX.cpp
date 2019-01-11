//
//  RenderDeviceMacOSX.cpp
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/6.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#include "RenderDeviceMacOSX.h"
#include "OpenGLRenderDriver.h"

RenderDeviceMacOSX::RenderDeviceMacOSX(const RenderDeviceCreationParameters& createPara) : m_pRenderDriver(NULL)
{
    CreateDriver(createPara, this);
}

RenderDeviceMacOSX::~RenderDeviceMacOSX()
{
    //
}

bool RenderDeviceMacOSX::Run()
{
    return true;
}

RenderDriver* RenderDeviceMacOSX::GetRenderDriver()
{
    return m_pRenderDriver;
}

void RenderDeviceMacOSX::CloseDevice()
{
    return;
}

E_DEVICE_TYPE RenderDeviceMacOSX::GetType() const
{
    return EIDT_OSX;
}

void RenderDeviceMacOSX::CreateDriver(const RenderDeviceCreationParameters& params, RenderDeviceMacOSX *device)
{
    if (params.DriverType == EDT_OPENGL)
    {
        m_pRenderDriver = new(std::nothrow) OpenGLRenderDriver(params, device);
    }
    
    else if (params.DriverType == EDT_OGLES2)
    {
        //
    }
}
