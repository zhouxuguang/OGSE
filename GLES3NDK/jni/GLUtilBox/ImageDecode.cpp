//
//  ImageDecode.cpp
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/11/19.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#include "ImageDecode.h"
#include "TextureBitmap.h"
#include "stb_image.h"
#include "FileResource.h"
#include <assert.h>

void ImageDecode::DecodeFile(const char *pszFileName, TextureBitmap &bitmap)
{
    if (NULL == pszFileName || strcmp(pszFileName, "") == 1)
    {
        return;
    }
    
//    int x,y,n;
//    unsigned char *data = stbi_load(pszFileName,&x,&y,&n,0);
    
    
    FileResource fileImage(pszFileName);
    eUInt64 nFileSize = fileImage.GetSize();
    eByte* pFileData = new eByte[nFileSize];
    
    fileImage.Read(pFileData, nFileSize);
    
    DecodeMemeory(pFileData,nFileSize,bitmap);
    
    delete [] pFileData;
    
    return;
}

void ImageDecode::DecodeMemeory(const void *pData, int nDataLength, TextureBitmap &bitmap)
{
    if (NULL == pData || 0 >= nDataLength)
    {
        return;
    }
    
    int width = 0;
    int height = 0;
    int numComponents = 0;
    unsigned char* pDecodeData = stbi_load_from_memory((eByte*)pData, nDataLength, &width, &height, &numComponents, 0 );
    
    BitmapFormat format = eUnkown;
    
    switch (numComponents)
    {
        case 1:
        {
            // Gray
            format = eALPHA;
            break;
        }
        case 2:
        {
            // Gray and Alpha
            format = eLUMINANCE_ALPHA;
            break;
        }
        case 3:
        {
            // RGB
            format = eRGB;
            break;
        }
        case 4:
        {
            // RGBA
            format = eRGBA;
            break;
        }
        default:
        {
            // Unknown format
            assert(0);
            stbi_image_free(pDecodeData);
            return;
        }
    }
    
    bitmap.AllocPixels(width, height, format);
    memcpy(bitmap.GetPixels(), pDecodeData, width * height * numComponents);
    
    
    // clean up
    stbi_image_free(pDecodeData);
}
