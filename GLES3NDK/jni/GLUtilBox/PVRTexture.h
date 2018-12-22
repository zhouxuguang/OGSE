
#ifndef PVRTEXTURE_LOADER_INCLUDE
#define PVRTEXTURE_LOADER_INCLUDE

#include <sys/types.h>

//PVR纹理加载工具

typedef unsigned int uint32_t;

typedef struct _PVRTexHeader
{
    uint32_t headerLength;
    uint32_t height;
    uint32_t width;
    uint32_t numMipmaps;
    uint32_t flags;
    uint32_t dataLength;
    uint32_t bpp;
    uint32_t bitmaskRed;
    uint32_t bitmaskGreen;
    uint32_t bitmaskBlue;
    uint32_t bitmaskAlpha;
    uint32_t pvrTag;
    uint32_t numSurfs;
} PVRTexHeader;

unsigned char* LoadPvrImage(const char* pszFileName, PVRTexHeader& header);

void UnLoadPvrImage(unsigned char* pData);


#endif
