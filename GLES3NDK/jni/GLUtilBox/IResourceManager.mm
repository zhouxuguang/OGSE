//
//  IResourceMannager.m
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/4/9.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#include "IResourceManager.h"

#include <UIKit/UIKit.h>

class ResourceManager : public IResourceManager
{
public:
    std::string GetResourcePath() const
    {
        NSString* bundlePath = [[NSBundle mainBundle] resourcePath];
        return [bundlePath UTF8String];
    }
    
    ResourceManager()
    {
        //
    }
    
    ~ResourceManager()
    {
        //
    }
    
    void LoadPngImage(const std::string& strFileName)
    {
        NSString* basePath = [NSString stringWithUTF8String:strFileName.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        
        UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
        CGImageRef cgImage = uiImage.CGImage;
        m_nWidth = CGImageGetWidth(cgImage);
        m_nHeight = CGImageGetHeight(cgImage);
        
        m_imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
    }
    
    void LoadPvrImage(const std::string& strFileName)
    {
        NSString* basePath = [NSString stringWithUTF8String:strFileName.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
    }
    
    int GetImageWidth() const
    {
        return m_nWidth;
    }
    
    int GetImageHeight() const
    {
        return m_nHeight;
    }
    
    virtual void UnLoadImage()
    {
        CFRelease(m_imageData);
        m_imageData = NULL;
    }
    
private:
    CFDataRef m_imageData;
    int m_nWidth;
    int m_nHeight;
};

IResourceManager* CreateResourceManager()
{
    return new(std::nothrow) ResourceManager();
}

