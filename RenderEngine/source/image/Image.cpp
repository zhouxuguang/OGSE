//
// Created by Zhou,Xuguang on 2018/10/22.
//

#include "Image.h"
#include <stddef.h>

Image::Image(ImagePixelFormat format, unsigned int nWidth, unsigned int nHeight,
             unsigned int nBitCount, const void *pData) :
                 mFormat(format),
                 mWidth(nWidth),
                 mHeight(nHeight),
                 mBitCount(nBitCount),
                 m_pDeleteFunc(NULL)
{
    m_pData = (void *)pData;
}

Image::Image()
{
    mFormat = FORMAT_UNKNOWN;
    mWidth = 0;
    mHeight = 0;
    mBitCount = 0;
    m_pDeleteFunc = NULL;
    m_pData = NULL;
}

Image::~Image()
{
    if (m_pDeleteFunc)
    {
        m_pDeleteFunc(m_pData);
        m_pData = NULL;
    }

    mFormat = FORMAT_UNKNOWN;
    mWidth = 0;
    mHeight = 0;
    mBitCount = 0;
    m_pDeleteFunc = NULL;
}

unsigned int Image::GetWidth() const
{
    return mWidth;
}

unsigned int Image::GetHeight() const
{
    return mHeight;
}

unsigned int Image::GetBitCount() const
{
    return mBitCount;
}

ImagePixelFormat Image::GetFormat() const
{
    return mFormat;
}

void* Image::GetPixels() const
{
    return m_pData;
}

void Image::SetDeleteFunc(DeleteFun pFunc)
{
    m_pDeleteFunc = pFunc;
}

void Image::SetPixels(void *pPixels)
{
    m_pData = pPixels;
}

void Image::SetWidth(unsigned int width)
{
    mWidth = width;
}

void Image::SetHeight(unsigned int height)
{
    mHeight = height;
}

void Image::SetBitCount(unsigned int bitCount)
{
    mBitCount = bitCount;
}

void Image::SetFormat(ImagePixelFormat format)
{
    mFormat = format;
}
