/*
 * RenderTarget.h
 *
 *  Created on: 2016-11-14
 *      Author: zhouxuguang
 */

#ifndef RENDERTARGET_H_
#define RENDERTARGET_H_

#include "GLPrecompile.h"

class RenderTarget
{
protected:
    RenderTarget();
    virtual ~RenderTarget();
    bool init(unsigned int width, unsigned int height);

public:
    enum Type
    {
        RenderBuffer = 0,
        Texture2D,
    };

public:
    virtual GLuint getBuffer() const { return 0; }

    unsigned int getWidth() const { return mWidth; }
    unsigned int getHeight() const { return mHeight; }
    Type getType() const { return mType; }

protected:
    Type mType;
    unsigned int mWidth;
    unsigned int mHeight;
};

//渲染到纹理
class RenderTargetTexture : public RenderTarget
{
	//
};

//渲染到渲染缓冲区
class RenderTargetRenderBuffer : public RenderTarget
{
public:
	RenderTargetRenderBuffer();
	virtual ~RenderTargetRenderBuffer();

	static RenderTargetRenderBuffer* create(unsigned int width, unsigned int height);

	bool init(unsigned int width, unsigned int height);

	virtual GLuint getBuffer() const { return mColorBuffer; }

protected:
    GLenum mFormat;
    GLuint mColorBuffer;
};

//深度和模板缓冲区
class RenderTargetDepthStencil : public RenderTarget
{
public:
    RenderTargetDepthStencil();
    virtual ~RenderTargetDepthStencil();
    
    static RenderTargetDepthStencil* create(unsigned int width, unsigned int height);
    
    bool init(unsigned int width, unsigned int height);
    
    virtual GLuint getBuffer() const { return mDepthStencilBuffer; }
    
    virtual GLuint getDepthBuffer() const { return mDepthBuffer; }
    
    virtual GLuint getStencilBuffer() const { return mStencilBuffer; }
    
protected:
    GLuint mDepthStencilBuffer;
    GLuint mDepthBuffer;
    GLuint mStencilBuffer;
};

//帧缓冲区
class FrameBuffer
{
public:
    FrameBuffer();
    
    ~FrameBuffer();
    
    static FrameBuffer* create(unsigned int width,unsigned int height);
    
    bool init(unsigned int width,unsigned int height);
    
    void attachRenderBuffer(RenderTarget* renderBuf);
    
    void attachDepthStencilBuf(RenderTargetDepthStencil* depthStencilBuf);
protected:
    GLuint mFrameBuf;
    unsigned int mWidth;
    unsigned int mHeight;
    
    RenderTarget* mRenderBuf;
    RenderTargetDepthStencil* mDepthStencilBuf;
};

#endif /* RENDERTARGET_H_ */
