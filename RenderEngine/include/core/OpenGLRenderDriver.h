//
//  OpenGLRenderDriver.h
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/7.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef OpenGLRenderDriver_hpp
#define OpenGLRenderDriver_hpp

#include "RenderNullDriver.h"
#include "RenderEngine.h"

class OpenGLRenderDriver : public RenderNullDriver
{
public:
    explicit OpenGLRenderDriver(const RenderDeviceCreationParameters& createPara,
                                RenderDevice* device);
    
    ~OpenGLRenderDriver();
    
    virtual bool BeginScene(bool backBuffer = true, bool zBuffer = true, bool stencilBuffer = true,
                            math3d::ColorRGBA color = math3d::ColorRGBA(1,0,0,0));
    
    virtual bool EndScene();
    
    virtual void OnResize(int nWidth, int nHeight);
};

#endif /* OpenGLRenderDriver_hpp */
