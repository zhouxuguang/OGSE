/*
 * RenderTarget.cpp
 *
 *  Created on: 2016-11-14
 *      Author: zhouxuguang
 */

#include "RenderTarget.h"
#include "GLConfiguration.h"

RenderTarget::RenderTarget()
{
	// TODO Auto-generated constructor stub
}

RenderTarget::~RenderTarget()
{
	// TODO Auto-generated destructor stub
}

bool RenderTarget::init(unsigned int width, unsigned int height)
{
	if (width <= 0 || height <= 0)
		return false;

	mWidth = width;
	mHeight = height;
	return true;
}

RenderTargetRenderBuffer::RenderTargetRenderBuffer():mFormat(GL_RGBA4),mColorBuffer(0)
{
	mType = RenderBuffer;
}

RenderTargetRenderBuffer::~RenderTargetRenderBuffer()
{
    if (glIsRenderbuffer(mColorBuffer))
    {
        glDeleteRenderbuffers(1, &mColorBuffer);
        mColorBuffer = 0;
    }
}

bool RenderTargetRenderBuffer::init(unsigned int width, unsigned int height)
{
    if(!RenderTarget::init(width, height)) return false;
    
    GLint oldRenderBuffer(0);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &oldRenderBuffer);
    
    //generate depthStencil
    glGenRenderbuffers(1, &mColorBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mColorBuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, mFormat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, oldRenderBuffer);
    
    return true;
}

RenderTargetRenderBuffer* RenderTargetRenderBuffer::create(unsigned int width, unsigned int height)
{
    RenderTargetRenderBuffer *render = new(std::nothrow) RenderTargetRenderBuffer();
    if (NULL != render && render->init(width, height))
    {
        return render;
    }
    
    else
    {
        delete render;
        return NULL;
    }
}

RenderTargetDepthStencil::RenderTargetDepthStencil(): mDepthStencilBuffer(0),mDepthBuffer(0),mStencilBuffer(0)
{
    mType = RenderBuffer;
}

RenderTargetDepthStencil::~RenderTargetDepthStencil()
{
    if(glIsRenderbuffer(mDepthStencilBuffer))
    {
        glDeleteRenderbuffers(1, &mDepthStencilBuffer);
        mDepthStencilBuffer = 0;
    }
    
    if (glIsRenderbuffer(mDepthBuffer))
    {
        glDeleteRenderbuffers(1, &mDepthBuffer);
        mDepthBuffer = 0;
    }
    
    if (glIsRenderbuffer(mStencilBuffer))
    {
        glDeleteRenderbuffers(1, &mStencilBuffer);
        mStencilBuffer = 0;
    }
}

bool RenderTargetDepthStencil::init(unsigned int width, unsigned int height)
{
    if(!RenderTarget::init(width, height)) return false;
    GLint oldRenderBuffer(0);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &oldRenderBuffer);
    
    GLConfiguration *config = GLConfiguration::GetInstance();
    if (config->IsSupportsOESPackedDepthStencil())      //支持深度缓存和模板缓存在一个缓冲区中
    {
        //generate depthStencil
        glGenRenderbuffers(1, &mDepthStencilBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);     //GL_DEPTH24_STENCIL8
        glBindRenderbuffer(GL_RENDERBUFFER, oldRenderBuffer);
    }
    
    else
    {
        glGenRenderbuffers(1, &mDepthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuffer);
        if (config->IsSupportsOESDepth24())     //支持24位深度缓冲
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, oldRenderBuffer);
        
        //另外生成stencil缓冲
        glGenRenderbuffers(1, &mStencilBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, mStencilBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, oldRenderBuffer);
    }
    
    return true;
}


RenderTargetDepthStencil* RenderTargetDepthStencil::create(unsigned int width, unsigned int height)
{
    RenderTargetDepthStencil *result = new (std::nothrow) RenderTargetDepthStencil();
    
    if(result && result->init(width, height))
    {
        return result;
    }
    else
    {
        delete result;
        return NULL;
    }
}

//framebuffer

FrameBuffer::FrameBuffer():mWidth(0),mHeight(0),mFrameBuf(0),
    mRenderBuf(NULL),mDepthStencilBuf(NULL)
{
    //
}

FrameBuffer::~FrameBuffer()
{
    //delete mRenderBuf;
    delete mDepthStencilBuf;
}

FrameBuffer* FrameBuffer::create(unsigned int width, unsigned int height)
{
    FrameBuffer* buf = new(std::nothrow) FrameBuffer();
    if (buf != NULL && buf->init(width, height))
    {
        return buf;
    }
    
    else
    {
        delete buf;
    }
    return NULL;
}

bool FrameBuffer::init(unsigned int width,unsigned int height)
{
    mWidth = width;
    mHeight = height;
    
    GLint oldfbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldfbo);
    
    glGenFramebuffers(1, &mFrameBuf);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuf);
    glBindFramebuffer(GL_FRAMEBUFFER, oldfbo);
    
    return true;
}

void FrameBuffer::attachRenderBuffer(RenderTarget *renderBuf)
{
    if (NULL == renderBuf)
    {
        return;
    }
    
    if (renderBuf->getWidth() != mWidth || renderBuf->getHeight() != mHeight)
    {
        return;
    }
    
    mRenderBuf = renderBuf;
}

void FrameBuffer::attachDepthStencilBuf(RenderTargetDepthStencil *depthStencilBuf)
{
    if (NULL == depthStencilBuf)
    {
        return;
    }
    
    if (depthStencilBuf->getWidth() != mWidth || depthStencilBuf->getHeight() != mHeight)
    {
        return;
    }
    
    mDepthStencilBuf = depthStencilBuf;
}


