//
// Created by Zhou,Xuguang on 2018/10/22.
//

#ifndef RENDERENGINE_IMAGELOADERPNG_H
#define RENDERENGINE_IMAGELOADERPNG_H

#include "ImageDecoderImpl.h"

class ImageDecoderPNG : public ImageDecoderImpl
{
private:
    virtual void onDecode(const void* buffer, size_t size, Image* bitmap);

    virtual bool IsFormat(const void* buffer, size_t size);

    virtual ImageStoreFormat GetFormat() const;
};

ImageDecoderPNG* CreatePNGDecoder();


#endif //RENDERENGINE_IMAGELOADERPNG_H
