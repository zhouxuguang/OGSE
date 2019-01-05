//
// Created by Zhou,Xuguang on 2018/11/2.
//

#include "ImageDecoderJPEG.h"
#include "ImageDecoderFactory.h"
#include <memory.h>
#include "stb_image/stb_image.h"

void ImageDecoderJPEG::onDecode(const void *buffer, size_t size, Image *bitmap)
{
    int x = 0;
    int y = 0;
    int nBitCount = 0;
    stbi_uc *pImage = stbi_load_from_memory((stbi_uc const *)buffer, size, &x, &y, &nBitCount, STBI_rgb_alpha);
    bitmap->SetBitCount(STBI_rgb_alpha * 8);
    bitmap->SetPixels(pImage);
    bitmap->SetWidth(x);
    bitmap->SetHeight(y);

    if (nBitCount == 1)
    {
        bitmap->SetFormat(FORMAT_GRAY);
    }
    else if (nBitCount == 2)
    {
        bitmap->SetFormat(FORMAT_GRAYALPHA);
    }
    else if (nBitCount == 3)
    {
        bitmap->SetFormat(FORMAT_RGB24);
    }
    else if (nBitCount == 4)
    {
        bitmap->SetFormat(FORMAT_RGBA32);
    }

    bitmap->SetDeleteFunc(stbi_image_free);
}

ImageStoreFormat ImageDecoderJPEG::GetFormat() const
{
    return kJPEG_Format;
}

static int pngHeaderCheck(const unsigned char* sig, size_t start, size_t num_to_check)
{
    unsigned char png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};

    if (num_to_check > 8)
        num_to_check = 8;

    else if (num_to_check < 1)
        return (-1);

    if (start > 7)
        return (-1);

    if (start + num_to_check > 8)
        num_to_check = 8 - start;

    return ((int)(memcmp(&sig[start], &png_signature[start], num_to_check)));
}

bool ImageDecoderJPEG::IsFormat(const void *buffer, size_t size)
{
    int is_png = pngHeaderCheck((const unsigned char*)buffer, 0, 8);
    //return is_png == 0;

    return 1;
}

ImageDecoderJPEG* CreateJPEGDecoder()
{
    return new(std::nothrow) ImageDecoderJPEG();
}
