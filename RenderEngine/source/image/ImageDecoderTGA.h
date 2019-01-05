//
// Created by Zhou,Xuguang on 2018/10/27.
//

#ifndef RENDERENGINE_IMAGEDECODERTGA_H
#define RENDERENGINE_IMAGEDECODERTGA_H

#include "ImageDecoderImpl.h"

class ImageDecoderTGA : public ImageDecoderImpl
{
private:
    virtual void onDecode(const void* buffer, size_t size, Image* bitmap);

    virtual bool IsFormat(const void* buffer, size_t size);

    virtual ImageStoreFormat GetFormat() const;
};

ImageDecoderTGA* CreateTGADecoder();


#endif //RENDERENGINE_IMAGEDECODERTGA_H
