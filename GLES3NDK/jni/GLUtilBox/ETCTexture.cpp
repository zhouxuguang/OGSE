//
//  ETCTexture.cpp
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/7/2.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#include "ETCTexture.h"
#include "GLPrecompile.h"

#include "BytesSwap.h"

bool LoadEtc2Texture(const char* pszFileName, ETC2Header& header, ETC2PKMData& pkmData)
{
    if (NULL == pszFileName)
    {
        return false;
    }
    FILE *fp = fopen(pszFileName, "rb");
    if (NULL == fp)
    {
        return false;
    }
    ETC2Header pkmfile;
    fread(&pkmfile, sizeof(ETC2Header), 1, fp);
    
    pkmfile.format      = baselib::BytesSwap::SwapInt16BigToHost(pkmfile.format);
    pkmfile.paddedWidth = baselib::BytesSwap::SwapInt16BigToHost(pkmfile.paddedWidth);
    pkmfile.paddedHeight = baselib::BytesSwap::SwapInt16BigToHost(pkmfile.paddedHeight);
    pkmfile.origWidth   =  baselib::BytesSwap::SwapInt16BigToHost(pkmfile.origWidth);
    pkmfile.origHeight  = baselib::BytesSwap::SwapInt16BigToHost(pkmfile.origHeight);
    
    //读取图像文件
    pkmData.width   = pkmfile.paddedWidth;  // Texture Width
    pkmData.height  = pkmfile.paddedHeight; // Texture Height
    // This only handles the pkmfile format
    unsigned int imageSize = ceil(pkmData.width/4)*ceil(pkmData.height/4)*8;
    pkmData.bitsraw = (unsigned char*) malloc(imageSize);
    fread(pkmData.bitsraw, imageSize, 1, fp); //Load Payload
    fclose(fp);
    if (!pkmData.bitsraw)
    {
        return false;
    }
    
    glEnable(GL_TEXTURE);
    GLuint texID;
    glGenTextures( 1,&texID );
    glBindTexture( GL_TEXTURE_2D,texID );
    glCompressedTexImage2D(GL_TEXTURE_2D, 0,
                           GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2, pkmData.width,pkmData.height,
                           0,imageSize, pkmData.bitsraw);
    
    return true;
}
