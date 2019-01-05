//
// Created by Zhou,Xuguang on 2018/10/22.
//

#include "ImageDecoder.h"
#include "ImageDecoderImpl.h"

bool ImageDecoder::DecodeFile(const char *fileName, Image *bitmap, ImageStoreFormat *format)
{
    return ImageDecoderImpl::DecodeFile(fileName, bitmap, format);

}

bool ImageDecoder::DecodeMemory(const void *buffer, size_t size, Image *bitmap,
                                ImageStoreFormat *format)
{
    return ImageDecoderImpl::DecodeMemory(buffer, size, bitmap, format);
}
