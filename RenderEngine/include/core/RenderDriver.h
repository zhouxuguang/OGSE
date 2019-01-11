//
//  RenderDriver.hpp
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/6.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#ifndef RenderDriver_hpp
#define RenderDriver_hpp

#include "ColorRGBA.h"

class RenderDriver
{
public:
    RenderDriver();
    
    ~RenderDriver();
    
    virtual bool BeginScene(bool backBuffer = true, bool zBuffer = true, bool stencilBuffer = true,
                            math3d::ColorRGBA color = math3d::ColorRGBA(1,0,0,0)) = 0;
    
    virtual bool EndScene() = 0;
    
    //! Event handler for resize events. Only used by the engine internally.
    /** Used to notify the driver that the window was resized.
     Usually, there is no need to call this method. */
    virtual void OnResize(int nWidth, int nHeight) = 0;
    
    int GetWindowWidth() const;
    
    int GetWindowHeight() const;
    
protected:
    
    int m_nWindowWidth;       //窗口宽度
    int m_nWindowHeight;      //窗口高度
};

#endif /* RenderDriver_hpp */
