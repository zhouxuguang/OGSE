//
// Created by Zhou,Xuguang on 2018/10/27.
//

#include <new>
#include "ImageDecoderTGA.h"
#include "libtga/TGAlib.h"

void ImageDecoderTGA::onDecode(const void *buffer, size_t size, Image *bitmap)
{
    tImageTGA *imageTGA = tgaLoadBuffer((unsigned char*)buffer, size);
    if (NULL == imageTGA)
    {
        return;
    }
    bitmap->SetPixels(imageTGA->imageData);
    bitmap->SetBitCount(imageTGA->pixelDepth);
    bitmap->SetWidth(imageTGA->width);
    bitmap->SetHeight(imageTGA->height);
    bitmap->SetDeleteFunc(tgaDestroyImage);
    imageTGA->imageData = NULL;

    tgaDestroy(imageTGA);
    //bitmap->SetFormat(imageTGA->type);
}

bool ImageDecoderTGA::IsFormat(const void *buffer, size_t size)
{
//    tImageTGA info;
//    return tgaLoadHeader((unsigned char*)buffer, size, &info);
    return 0;
}

ImageStoreFormat ImageDecoderTGA::GetFormat() const
{
    return kTGA_Format;
}

ImageDecoderTGA* CreateTGADecoder()
{
    return new(std::nothrow) ImageDecoderTGA();
}