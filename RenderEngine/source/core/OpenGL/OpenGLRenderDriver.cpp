//
//  OpenGLRenderDriver.cpp
//  RenderEngine
//
//  Created by Zhou,Xuguang on 2019/1/7.
//  Copyright © 2019年 Zhou,Xuguang. All rights reserved.
//

#include "OpenGLRenderDriver.h"
#include "RenderDevice.h"
#include "RenderEngineDefine.h"

// -----------------------------------
// MACOSX VERSION
// -----------------------------------
RenderDriver* CreateOpenGLDriver(const RenderDeviceCreationParameters& params, RenderDevice *device)
{
    return new OpenGLRenderDriver(params, device);
}

OpenGLRenderDriver::OpenGLRenderDriver(const RenderDeviceCreationParameters& createPara, RenderDevice* device)
{
    m_nWindowWidth = createPara.nWidth;
    m_nWindowHeight = createPara.nHeight;
    
    glViewport(0, 0, m_nWindowWidth, m_nWindowHeight);
    
    glClearStencil(0);
    
    //配置投影矩阵
    
    glClearDepth(1.0);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);
    // adjust flat coloring scheme to DirectX version
#if defined(GL_ARB_provoking_vertex) || defined(GL_EXT_provoking_vertex)
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION_EXT);
#endif
}

OpenGLRenderDriver::~OpenGLRenderDriver()
{
    //
}

void OpenGLRenderDriver::ClearBuffers(bool backBuffer, bool zBuffer, bool stencilBuffer, math3d::ColorRGBA color)
{
    GLbitfield mask = 0;
    if (backBuffer)
    {
        glClearColor(color.GetR(), color.GetG(),
                     color.GetB(), color.GetA());
        
        mask |= GL_COLOR_BUFFER_BIT;
    }
    
    if (zBuffer)
    {
        glDepthMask(GL_TRUE);
        mask |= GL_DEPTH_BUFFER_BIT;
    }
    
    if (stencilBuffer)
        mask |= GL_STENCIL_BUFFER_BIT;
    
    if (mask)
        glClear(mask);
    
}

bool OpenGLRenderDriver::BeginScene(bool backBuffer, bool zBuffer, bool stencilBuffer,
                                    math3d::ColorRGBA color)
{
    ClearBuffers(backBuffer, zBuffer, stencilBuffer, color);
    return true;
}

bool OpenGLRenderDriver::EndScene()
{
    RenderNullDriver::EndScene();
    
    glFlush();
    
    return true;
}

void OpenGLRenderDriver::OnResize(int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
}
