//
// Created by Zhou,Xuguang on 2018/11/2.
//

#ifndef RENDERENGINE_IMAGEDECODERJPEG_H
#define RENDERENGINE_IMAGEDECODERJPEG_H

#include "ImageDecoderImpl.h"

class ImageDecoderJPEG : public ImageDecoderImpl
{
private:
    virtual void onDecode(const void* buffer, size_t size, Image* bitmap);

    virtual bool IsFormat(const void* buffer, size_t size);

    virtual ImageStoreFormat GetFormat() const;
};

ImageDecoderJPEG* CreateJPEGDecoder();


#endif //RENDERENGINE_IMAGEDECODERJPEG_H
