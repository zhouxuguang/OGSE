//
//  IResourceMannager.h
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/4/9.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#ifndef IResourceManager_hpp
#define IResourceManager_hpp

#include <stdio.h>
#include <string>

class IResourceManager
{
public:
    virtual std::string GetResourcePath() const = 0;
    
    //加载PNG图像
    virtual void LoadPngImage(const std::string& strFileName) = 0;
    
    virtual void LoadPvrImage(const std::string& strFileName) = 0;
    
    virtual void UnLoadImage() = 0;
    
    virtual int GetImageWidth() const = 0;
    
    virtual int GetImageHeight() const = 0;
    
    virtual ~IResourceManager(){}
};

IResourceManager* CreateResourceManager();

#endif /* IResourceManager_hpp */
