//
//  RenderDeviceMacOSX.h
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/6.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef RenderDeviceMacOSX_hpp
#define RenderDeviceMacOSX_hpp

#include "RenderDeviceStub.h"
#include "RenderEngine.h"

class RenderDeviceMacOSX : public RenderDeviceStub
{
public:
    explicit RenderDeviceMacOSX(const RenderDeviceCreationParameters& createPara);
    
    ~RenderDeviceMacOSX();
    
    virtual bool Run();
    
    virtual RenderDriver* GetRenderDriver();
    
    virtual void CloseDevice();
    
    virtual E_DEVICE_TYPE GetType() const;
    
private:
    RenderDriver* m_pRenderDriver;
    
private:
    void CreateDriver(const RenderDeviceCreationParameters& params, RenderDeviceMacOSX *device);    //创建驱动器
};

#endif /* RenderDeviceMacOSX_hpp */
