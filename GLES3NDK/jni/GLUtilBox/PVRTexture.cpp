
#include "PVRTexture.h"
#include "BytesSwap.h"

#define PVR_TEXTURE_FLAG_TYPE_MASK	0xff

static char gPVRTexIdentifier[5] = "PVR!";

enum
{
	kPVRTextureFlagTypePVRTC_2 = 24,
	kPVRTextureFlagTypePVRTC_4 = 25,
};

unsigned char* LoadPvrImage(const char* pszFileName, PVRTexHeader& header)
{
    if (NULL == pszFileName)
    {
        return NULL;
    }
    FILE* fpTexture = fopen(pszFileName, "rb");
    if (NULL == fpTexture)
    {
        return NULL;
    }
    
    fseek(fpTexture,0,SEEK_END);
    int size = ftell(fpTexture);
    fseek(fpTexture,0,SEEK_SET);
    
    unsigned char* pData = (unsigned char*)malloc(size);
    fread(pData, size, 1, fpTexture);
    fclose(fpTexture);
    
    PVRTexHeader *pHeader = &header;
    pHeader = (PVRTexHeader *)pData;
    
    uint32_t pvrTag = pHeader->pvrTag = baselib::BytesSwap::SwapInt32LittleToHost(pHeader->pvrTag);
    
    if (gPVRTexIdentifier[0] != ((pvrTag >>  0) & 0xff) ||
        gPVRTexIdentifier[1] != ((pvrTag >>  8) & 0xff) ||
        gPVRTexIdentifier[2] != ((pvrTag >> 16) & 0xff) ||
        gPVRTexIdentifier[3] != ((pvrTag >> 24) & 0xff))
    {
        free(pData);
        return NULL;
    }
    
    uint32_t flags = pHeader->flags = baselib::BytesSwap::SwapInt32LittleToHost(pHeader->flags);
    uint32_t formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;  //分别是24和25
    pHeader->flags = formatFlags;
    
    if (formatFlags != kPVRTextureFlagTypePVRTC_2 && formatFlags != kPVRTextureFlagTypePVRTC_4)
    {
        free(pData);
        return NULL;
    }
    
    pHeader->width = baselib::BytesSwap::SwapInt32LittleToHost(pHeader->width);
    pHeader->width = baselib::BytesSwap::SwapInt32LittleToHost(pHeader->height);
    
    if (baselib::BytesSwap::SwapInt32LittleToHost(pHeader->bitmaskAlpha))
        pHeader->bitmaskAlpha = 1;
    else
        pHeader->bitmaskAlpha = 0;
    
    pHeader->dataLength = baselib::BytesSwap::SwapInt32LittleToHost(pHeader->dataLength);
    
    return pData + sizeof(PVRTexHeader);
}

void UnLoadPvrImage(unsigned char* pData)
{
    if (pData != NULL)
    {
        free(pData - sizeof(PVRTexHeader));
    }
}


// Calculate the data size for each texture level and respect the minimum number of blocks
//while (dataOffset < dataLength)
//{
//    if (formatFlags == kPVRTextureFlagTypePVRTC_4)
//    {
//        blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
//        widthBlocks = width / 4;
//        heightBlocks = height / 4;
//        bpp = 4;
//    }
//    else
//    {
//        blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
//        widthBlocks = width / 8;
//        heightBlocks = height / 4;
//        bpp = 2;
//    }
//    
//    // Clamp to minimum number of blocks
//    if (widthBlocks < 2)
//        widthBlocks = 2;
//        if (heightBlocks < 2)
//            heightBlocks = 2;
//            
//            dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
//            
//            [_imageData addObject:[NSData dataWithBytes:bytes+dataOffset length:dataSize]];
//    
//    dataOffset += dataSize;
//    
//    width = MAX(width >> 1, 1);
//    height = MAX(height >> 1, 1);
//}

