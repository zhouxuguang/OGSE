//
// Created by Zhou,Xuguang on 2018/10/22.
//

#ifndef RENDERENGINE_IMAGELOADER_H
#define RENDERENGINE_IMAGELOADER_H

#include <stddef.h>
#include "Image.h"

class ImageDecoder
{
public:

    /**
     * 从文件解码图像文件
     *
     * @param fileName android的assets目录下的文件必须以assets/开头
     * @param bitmap
     * @param format
     * @return
     */
    static bool DecodeFile(const char *fileName,
                           Image* bitmap,
                           ImageStoreFormat *format = NULL);

    /**
     * 从内存缓冲区解码图像
     *
     * @param buffer
     * @param size
     * @param bitmap
     * @param format
     * @return
     */
    static bool DecodeMemory(const void* buffer,
                             size_t size,
                             Image* bitmap,
                             ImageStoreFormat* format = NULL);

private:
    ImageDecoder();

    ~ImageDecoder();

    ImageDecoder(const ImageDecoder&);

    ImageDecoder& operator = (const ImageDecoder&);

};


#endif //RENDERENGINE_IMAGELOADER_H
