//
//  ETCTexture.h
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/7/2.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#ifndef ETCTEXTURE_LOADER_INCLUDE_DFKNJS_H
#define ETCTEXTURE_LOADER_INCLUDE_DFKNJS_H

struct ETC2Header
{
    char name[4];               //pkm
    char version[2];            //20 FOR ETC2
    unsigned short format;
    unsigned short paddedWidth;  // Texture WIDTH,(big-endian)
    unsigned short paddedHeight;  // Texture height,(big-endian)
    unsigned short origWidth;    // Original width(big-endian)
    unsigned short origHeight;   // Original height(big-endian)
};

struct ETC2PKMData
{
    unsigned int imageSize;     //图像大小
    int width;
    int height;
    unsigned char* bitsraw;
};

bool LoadEtc2Texture(const char* pszFileName, ETC2Header& header, ETC2PKMData& pkmData);

#endif /* ETCTEXTURE_LOADER_INCLUDE_DFKNJS_H */
