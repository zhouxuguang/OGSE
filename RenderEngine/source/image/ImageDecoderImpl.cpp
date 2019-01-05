//
// Created by Zhou,Xuguang on 2018/10/27.
//

#include "ImageDecoderImpl.h"
#include "ImageDecoderFactory.h"
#include <stdio.h>
#include <memory.h>

#ifdef __ANDROID__
#include "android_fopen.h"
#endif

bool ImageDecoderImpl::DecodeFile(const char *fileName, Image *bitmap, ImageStoreFormat *format)
{
    if (NULL == fileName || NULL == bitmap)
    {
        return false;
    }

#ifdef __ANDROID__

    FILE *fp = NULL;
    const char* pTmp = NULL;

    //判断路径名是否以assets/开头
    if ((pTmp = strstr(fileName, "assets/")) != NULL)
    {
        pTmp += 7;
        fp = android_fopen(pTmp, "rb");
    }
    else
    {
        fp = fopen(fileName, "rb");
    }

#else
    FILE *fp = fopen(fileName, "rb");
#endif


    if (NULL == fp)
    {
        return false;
    }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    unsigned char *pBuf = new unsigned char[size];

    fread(pBuf, size, 1, fp);
    fclose(fp);

    bool bRet = ImageDecoderImpl::DecodeMemory(pBuf, size, bitmap, format);
    delete[]pBuf;
    return bRet;

}

bool ImageDecoderImpl::DecodeMemory(const void *buffer, size_t size, Image *bitmap,
                                ImageStoreFormat *format)
{
    if (NULL == buffer || 0 == size || NULL == bitmap)
    {
        return false;
    }

    ImageDecoderFactory* pInstance = ImageDecoderFactory::GetInstance();
    ImageDecoderImpl* pDecoder = pInstance->GetImageDecoder(buffer, size);
    if (NULL == pDecoder)
    {
        return false;
    }

    pDecoder->onDecode(buffer, size, bitmap);
    if (format)
    {
        *format = pDecoder->GetFormat();
    }

    return true;
}

ImageStoreFormat ImageDecoderImpl::GetFormat() const
{
    return kUnknown_Format;
}
