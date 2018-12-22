//
//  SwapPtr.h 交换指针，主要用于多线程之间交换数据
//  BaseLib
//
//  Created by zhouxuguang on 16/10/9.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#ifndef BASELIB_SWAPPTR_INCLUDE_H_DFKGJ3DFGL
#define BASELIB_SWAPPTR_INCLUDE_H_DFKGJ3DFGL

#include "PreCompile.h"

NS_BASELIB_BEGIN

template<class T>
class SwapPtr
{
private:
    T* p1;
    T* p2;
    bool use1;
    
public:
    SwapPtr()
    {
        p1 = p2 = NULL;
        use1 = true;
    }
    
    bool set(T* ptr)
    {
        if(NULL == p1)
        {
            p1 = ptr;
            return true;
        }
        else if(NULL == p2)
        {
            p2 = ptr;
            return true;
        }
        else
        {
            return false;
        }
    }
    
    /**
     *  read
     *  @return 指针
     */
    T* get()
    {
        if(p1 != NULL && p2 != NULL)
        {
            if(use1)
            {
                delete p1;
                p1 = NULL;
            }
            else
            {
                delete p2;
                p2 = NULL;
            }
            use1 = !use1;
        }
        
        return use1 ? p1 : p2;
    }
    
    /**
     * reader thread
     */
    void release()
    {
        if(p1 != NULL)
        {
            delete p1;
            p1 = NULL;
        }
        if(p2 != NULL)
        {
            delete p2;
            p2 = NULL;
        }
    }
    
};

NS_BASELIB_END

#endif /* BASELIB_SWAPPTR_INCLUDE_H_DFKGJ3DFGL */
