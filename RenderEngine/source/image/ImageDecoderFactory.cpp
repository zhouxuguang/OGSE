//
// Created by Zhou,Xuguang on 2018/10/25.
//

#include "ImageDecoderFactory.h"
#include "ImageDecoderPNG.h"
#include "ImageDecoderTGA.h"
#include "ImageDecoderJPEG.h"
#include "ImageDecoderBMP.h"

ImageDecoderFactory *ImageDecoderFactory::m_pInstance = NULL;
std::once_flag ImageDecoderFactory::m_OnceFlag;

ImageDecoderFactory* ImageDecoderFactory::GetInstance()
{
    std::call_once(m_OnceFlag, []()
    {
        m_pInstance = new(std::nothrow) ImageDecoderFactory();
        m_pInstance->AddImageDecoder(CreatePNGDecoder());
        m_pInstance->AddImageDecoder(CreateTGADecoder());
        m_pInstance->AddImageDecoder(CreateJPEGDecoder());
        m_pInstance->AddImageDecoder(CreateBMPDecoder());
    });

    return m_pInstance;
}

ImageDecoderFactory::ImageDecoderFactory()
{
    mArrDecoders.clear();
}

void ImageDecoderFactory::AddImageDecoder(ImageDecoderImpl *pDecoder)
{
    mArrDecoders.push_back(pDecoder);
}

ImageDecoderImpl* ImageDecoderFactory::GetImageDecoder(const void *buffer, size_t size)
{
    size_t nSize = mArrDecoders.size();
    for (size_t i = 0; i < nSize; ++i)
    {
        ImageDecoderImpl* pDecoder = mArrDecoders[i];
        if (pDecoder && pDecoder->IsFormat(buffer, size))
        {
            return pDecoder;
        }
    }

    return NULL;
}
