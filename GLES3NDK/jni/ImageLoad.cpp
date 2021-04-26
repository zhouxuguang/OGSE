////
////  ImageLoad.cpp
////  GLUtilBox
////
////  Created by zhouxuguang on 16/12/8.
////  Copyright © 2016年 zhouxuguang. All rights reserved.
////
//
//#include "ImageLoad.h"
//
//#include "GLPrecompile.h"
//
//#include "SkImageDecoder.h"
//#include "SkBitmap.h"
//
//void LoadTexture2dWithSkia(const char* fileName,unsigned int texId)
//{
//    SkBitmap bitmap;
//    SkImageDecoder::DecodeFile(fileName, &bitmap);
//
//    glBindTexture(GL_TEXTURE_2D, texId);
//    
//    SkBitmap::Config bmpConfig = bitmap.config();
//    
//    switch (bmpConfig) {
//        case SkBitmap::kRGB_565_Config:
//        {
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width(), bitmap.height(), 0,
//                         GL_RGB565, GL_UNSIGNED_SHORT_5_6_5 , bitmap.getPixels());
//            break;
//        }
//            
//        case SkBitmap::kARGB_8888_Config:
//        {
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.width(), bitmap.height(), 0,
//                         GL_RGBA, GL_UNSIGNED_BYTE, bitmap.getPixels());
//            break;
//        }
//            
//        default:
//            break;
//    }
//    
//    bitmap.reset();
//}
//
//void LoadImage(const char* fileName,SkBitmap& bitmap)
//{
//    SkImageDecoder::DecodeFile(fileName, &bitmap);
//}
