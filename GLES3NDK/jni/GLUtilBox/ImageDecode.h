//
//  ImageDecode.h
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/11/19.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#ifndef ImageDecode_hpp
#define ImageDecode_hpp

#include <stdio.h>

class TextureBitmap;

class ImageDecode
{
public:
    static void DecodeFile(const char* pszFileName, TextureBitmap& bitmap);
    
    static void DecodeMemeory(const void* pData, int nDataLength, TextureBitmap& bitmap);
};

#endif /* ImageDecode_hpp */
