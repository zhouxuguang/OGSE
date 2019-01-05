
//
// Created by Zhou,Xuguang on 2018/10/22.
//

#ifndef RENDERENGINE_IMAGE_H
#define RENDERENGINE_IMAGE_H

//图像的像素格式
enum ImagePixelFormat
{
    FORMAT_UNKNOWN,
    FORMAT_GRAY,       //gray
    FORMAT_GRAYALPHA,  //GRAY AND ALPHA
    FORMAT_RGBA32,     //RGBA32
    FORMAT_RGB24,      //RGB
};

//图像的存储格式  bmp/png等
enum ImageStoreFormat
{
    kUnknown_Format,
    kBMP_Format,
    kJPEG_Format,
    kPNG_Format,
    kTGA_Format,
};

class Image
{
public:
    Image();

    Image(ImagePixelFormat format,
          unsigned int nWidth,
          unsigned int nHeight,
          unsigned int nBitCount,
          const void* pData);

    ~Image();

    void* GetPixels() const;

    ImagePixelFormat GetFormat() const;

    unsigned int GetWidth() const;

    unsigned int GetHeight() const;

    unsigned int GetBitCount() const;

    void SetPixels(void* pPixels);

    void SetFormat(ImagePixelFormat format);

    void SetWidth(unsigned int width);

    void SetHeight(unsigned int height);

    void SetBitCount(unsigned int bitCount);

    //设置删除函数
    typedef void(*DeleteFun)(void *);
    void SetDeleteFunc(DeleteFun pFunc);

private:
    unsigned int                mWidth;            //宽
    unsigned int                mHeight;           //高
    unsigned int                mBitCount;         //位数  16,24,32
    ImagePixelFormat            mFormat;            //数据格式

    void *m_pData;                  //数据

    DeleteFun m_pDeleteFunc;

};


#endif //RENDERENGINE_IMAGE_H
