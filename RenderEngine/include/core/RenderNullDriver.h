//
//  RenderNullDriver.h
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/7.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef RenderNullDriver_hpp
#define RenderNullDriver_hpp

#include "RenderDriver.h"

class RenderNullDriver : public RenderDriver
{
public:
    virtual bool BeginScene(bool backBuffer = true, bool zBuffer = true, bool stencilBuffer = true,
                            math3d::ColorRGBA color = math3d::ColorRGBA(1,0,0,0))
    {
        return true;
    }
    
    virtual bool EndScene()
    {
        return true;
    }
    
    virtual void OnResize(int nWidth, int nHeight)
    {
        //
    }
};

#endif /* RenderNullDriver_hpp */
