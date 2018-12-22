//
//  TextureBitmap.cpp
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/10/15.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#include "TextureBitmap.h"

eUInt32 static GetFormatSize(BitmapFormat format)
{
    if (format == eRGB)
    {
        return 3;
    }
    
    else if (format == eRGBA)
    {
        return 4;
    }
    
    else if (format == eALPHA)
    {
        return 1;
    }
    
    else if (format == eLUMINANCE_ALPHA)
    {
        return 2;
    }
    
    return 0;
}

TextureBitmap::TextureBitmap()
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_eFormat = eUnkown;
    m_bitCount = 0;
}


TextureBitmap::TextureBitmap(eUInt32 nWidth,eUInt32 nHeight,BitmapFormat format)
{
    AllocPixels(nWidth,nHeight,format);
}

eUInt32 TextureBitmap::GetWidth() const
{
    return m_nWidth;
}

eUInt32 TextureBitmap::GetHeight() const
{
    return m_nHeight;
}

eUInt16 TextureBitmap::GetBitCount() const
{
    return m_bitCount;
}

BitmapFormat TextureBitmap::GetFormat() const
{
    return m_eFormat;
}

void TextureBitmap::AllocPixels(eUInt32 nWidth,eUInt32 nHeight,BitmapFormat format)
{
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_eFormat = format;
    m_bitCount = GetFormatSize(format);
    
    m_pData = (eByte*)malloc(m_nWidth*m_nHeight*m_bitCount);
}

void TextureBitmap::Reset()
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_eFormat = eUnkown;
    m_bitCount = 0;
    
    if (m_pData != NULL)
    {
        free(m_pData);
        m_pData = NULL;
    }
}

eByte* TextureBitmap::GetPixels() const
{
    return m_pData;
}

