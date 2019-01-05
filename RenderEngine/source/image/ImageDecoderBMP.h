//
// Created by Zhou,Xuguang on 2018/11/4.
//

#ifndef RENDERENGINE_IMAGEDECODERBMP_H
#define RENDERENGINE_IMAGEDECODERBMP_H

#include "ImageDecoderImpl.h"

class ImageDecoderBMP : public ImageDecoderImpl
{
private:
    virtual void onDecode(const void* buffer, size_t size, Image* bitmap);

    virtual bool IsFormat(const void* buffer, size_t size);

    virtual ImageStoreFormat GetFormat() const;

};

ImageDecoderBMP* CreateBMPDecoder();


#endif //RENDERENGINE_IMAGEDECODERBMP_H
