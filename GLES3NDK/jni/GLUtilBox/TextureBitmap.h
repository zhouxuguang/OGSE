//
//  TextureBitmap.h
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/10/15.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#ifndef TextureBitmap_hpp
#define TextureBitmap_hpp

#include "Precompile.h"

//纹理图像的格式
enum BitmapFormat
{
    eUnkown,
    eRGBA,
    eRGB,
    eALPHA,
    eLUMINANCE_ALPHA
};

class TextureBitmap
{
public:
    TextureBitmap();
    
    TextureBitmap(eUInt32 nWidth,eUInt32 nHeight,BitmapFormat format);
    
    //给图像分配数据
    void AllocPixels(eUInt32 nWidth,eUInt32 nHeight,BitmapFormat format);
    
    void Reset();
    
    eByte* GetPixels() const;
    
    eUInt32 GetWidth() const;
    
    eUInt32 GetHeight() const;
    
    eUInt16 GetBitCount() const;
    
    BitmapFormat GetFormat() const;
    
private:
    eUInt32 m_nWidth;
    eUInt32 m_nHeight;
    eUInt16 m_bitCount;   //一个像素的字节大小
    
    eByte *m_pData;
    
    BitmapFormat m_eFormat;
    
};

#endif /* TextureBitmap_hpp */
