//
// Created by Zhou,Xuguang on 2018/10/25.
//

#ifndef RENDERENGINE_IMAGEDECODERFACTORY_H
#define RENDERENGINE_IMAGEDECODERFACTORY_H

#include <vector>
#include <mutex>
#include "ImageDecoderImpl.h"

class ImageDecoderFactory
{
public:
    ImageDecoderFactory();

    static ImageDecoderFactory* GetInstance();

    //找不到返回空
    ImageDecoderImpl* GetImageDecoder(const void* buffer, size_t size);

    //添加图像驱动
    void AddImageDecoder(ImageDecoderImpl* pDecoder);

private:
    std::vector<ImageDecoderImpl* > mArrDecoders;            //解码器集合
    static ImageDecoderFactory* m_pInstance;
    static std::once_flag m_OnceFlag;
};


#endif //RENDERENGINE_IMAGEDECODERFACTORY_H
