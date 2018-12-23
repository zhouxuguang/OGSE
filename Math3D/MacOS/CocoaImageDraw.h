//
//  CocoaImageDraw.hpp
//  TestCocoa
//
//  Created by zhouxuguang on 16/5/31.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#ifndef CocoaImageDraw_hpp
#define CocoaImageDraw_hpp

#include <CoreGraphics/CoreGraphics.h>

class CocoaImageDraw
{
public:
    CocoaImageDraw();
    ~CocoaImageDraw();
    
    void CreatedDeviceImage(unsigned char* pData,int nWidth,int nHeight,int nPerPixBytes = 4);
    
    void DestroyDeviceImage();
    
    void Draw(const CGRect& rect) const;
    
private:
    unsigned char* m_pBytes;        //图像数据
    long 	mWidth;			//图像宽度
    long	mHeight;		//图像高度
    long    mPerPixBytes;   //每个像素的字节数
    
    CGColorSpaceRef mColorSpace;        //coregraphic颜色空间
    CGContextRef mContext;              //图形上下文
    CGImageRef mImage;                  //图像对象
};

#endif /* CocoaImageDraw_hpp */
