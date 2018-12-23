//
//  CocoaImageDraw.cpp
//  TestCocoa
//
//  Created by zhouxuguang on 16/5/31.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#include "CocoaImageDraw.h"
#include <stdlib.h>
#include <memory.h>
#include <CoreGraphics/CoreGraphics.h>

CocoaImageDraw::CocoaImageDraw()
{
    mWidth = 0;
    mHeight = 0;
    mPerPixBytes = 0;
    m_pBytes = NULL;
    mColorSpace = NULL;
    mContext = NULL;
    mImage = NULL;
}

CocoaImageDraw::~CocoaImageDraw()
{
    DestroyDeviceImage();
}

void CocoaImageDraw::CreatedDeviceImage(unsigned char* pData,int nWidth,int nHeight,int nPerPixBytes)
{
    if (nWidth <= 0 || nHeight <= 0 || nPerPixBytes <= 0)
        return;
    
    if (NULL == mColorSpace)
        mColorSpace = CGColorSpaceCreateDeviceRGB();
    
    mWidth = nWidth;
    mHeight = nHeight;
    mPerPixBytes = nPerPixBytes;
    
    m_pBytes = (unsigned char*)malloc(nWidth*nHeight*nPerPixBytes);
    memcpy(m_pBytes, pData, nWidth*nHeight*nPerPixBytes);
    mContext = CGBitmapContextCreate(m_pBytes, nWidth, nHeight, 8, nPerPixBytes * nWidth, mColorSpace
                          , kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    
    if (NULL == mContext)
    {
        free(m_pBytes);
        return;
    }
    
    CGContextTranslateCTM(mContext, 0, nHeight);
    CGContextScaleCTM(mContext, 1.0, -1.0);
    
    //填充黑色
    CGContextSetRGBFillColor(mContext, (CGFloat)0.0, (CGFloat)0.0, (CGFloat)0.0, (CGFloat)1.0 );
    
    if (NULL ==  mImage)
    {
        mImage = CGBitmapContextCreateImage(mContext);
    }
}

void CocoaImageDraw::DestroyDeviceImage()
{
    mWidth = 0;
    mHeight = 0;
    mPerPixBytes = 0;
    if (m_pBytes != NULL)
    {
        free(m_pBytes);
        m_pBytes = NULL;
    }
    
    if (mContext != NULL) {
        CGContextRelease(mContext);
        mContext = NULL;
    }
    
    if (mColorSpace != NULL) {
        CGColorSpaceRelease(mColorSpace);
        mColorSpace = NULL;
    }
    
    if (mImage != NULL) {
        CGImageRelease(mImage);
        mImage = NULL;
    }
}

void CocoaImageDraw::Draw(const CGRect &rect) const
{
    CGContextDrawImage(mContext, rect, mImage);
}
