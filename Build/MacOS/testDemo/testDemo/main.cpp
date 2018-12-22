//
//  main.cpp
//  TestDemo
//
//  Created by Zhou,Xuguang on 17/7/8.
//  Copyright © 2017年 Zhou,Xuguang. All rights reserved.
//

#include <iostream>

#include "BytesSwap.h"
#include "MemoryPool_++.h"
#include "MemoryPool.h"
#include "Memory-book.h"
#include "Thread.h"

#include "arena.h"

#include <dispatch/dispatch.h>

//using namespace ClipperLib;
//
//void TestClipper()
//{
//    Paths subj(2), clip(1), solution;
//
//    //define outer blue 'subject' polygon
//    subj[0] <<
//    IntPoint(180,200) << IntPoint(230,200) <<
//    IntPoint(230,150) << IntPoint(180,150);
//
//    //define subject's inner triangular 'hole' (with reverse orientation)
//    subj[1] << IntPoint(215,160) << IntPoint(225,190) << IntPoint(200,190);
//
//    //define orange 'clipping' polygon
//    clip[0] <<
//    IntPoint(190,210) << IntPoint(240,210) <<
//    IntPoint(240,130) << IntPoint(190,130);
//
//    Path TriPath;
//    TriPath << IntPoint(180,150) << IntPoint(210,230) << IntPoint(250,150);
//
//    //perform intersection ...
//    Clipper c;
//    //c.AddPath(TriPath, ptSubject, false);
//    c.AddPath(subj[0], ptSubject, false);
//    c.AddPaths(clip, ptClip, true);
//
//    PolyTree polyTree;
//    c.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd);
//    PolyTreeToPaths(polyTree,solution);
//}

void* TestFuncs(void*)
{
    pthread_setname_np("周旭光-------------------------------------------------");
    
    char szBus[256];
    pthread_getname_np(pthread_self(), szBus, 256);
    return 0;
}

int main(int argc, const char * argv[])
{
    // insert code here...
    std::cout << "Hello, World!\n";
    
    char szBus[16];
    pthread_getname_np(pthread_self(), szBus, 16);
    
    dispatch_queue_t queue = dispatch_queue_create("test.queue", DISPATCH_QUEUE_CONCURRENT);
    
    dispatch_async(queue, ^
    {
        pthread_setname_np("周旭光");
        for (int i = 0; i < 2; ++i)
        {
            printf("");
        }
    });
    
    dispatch_queue_t queue1 = dispatch_queue_create("test.queue", DISPATCH_QUEUE_SERIAL);
    
    //dispatch_once(NULL, NULL);
    
    dispatch_sync(queue1, ^
    {
        pthread_setname_np("我的主线程");
        for (int i = 0; i < 2; ++i)
        {
            //NSLog(@"1------%@",[NSThread currentThread]);
        }
    });
    
    //pthread_getname_np(pthread_t _Nonnull, char * _Nonnull, size_t)
    
    baselib::Thread trd = baselib::Thread();
    trd.Start(TestFuncs, NULL);
    
    int arrStyles[] = {894, 895, 896};
    
    //»Áπ˚ «“™π˝¬ÀµƒPOI
    if (std::end(arrStyles) != std::find(std::begin(arrStyles), std::end(arrStyles), 12) )
    {
        //return 0;
    }
    
    int nWch = sizeof(wchar_t);
    
    int aa = 90;
    int bb = baselib::BytesSwap::SwapInt32HostToLittle(aa);
    printf("%d\n",bb);
    
    clock_t t1 = clock();
    MemoryPool<int> memPool;
    CMemoryPool<int> memBook;
    int *p1 = (int*)memBook.Alloc(10000*10000);
    
    MemoryPoolImpl pPoolImpl;
    leveldb::Arena anea;
    
    for (int i = 0; i < 10000000; i ++)
    {
        void* pMem = baselib::MemoryPool::Allocate(4);
        memset(pMem, 0, sizeof(int));
        //baselib::MemoryPool::Free(pMem);
//        int *p1 = memPool.allocate();
//        *p1 = 19;
//        memPool.deallocate(p1);
        
//        int *p1 = (int*)memBook.Alloc(10000*10000);
//        memset(p1, 0, sizeof(int)*10000*10000);
//        *p1 = 19;
//        memBook.Free(p1);
        
//        int *p1 = (int*)pPoolImpl.Alloc(sizeof(int) * 10);
//        memset(p1, 0, sizeof(int)*10);
//        *p1 = 19;
//        pPoolImpl.Free(p1);
        
//        int *p1 = (int*)anea.Allocate(sizeof(int)*10000*10000);
//        memset(p1, 0, sizeof(int));
//        *p1 = 19;
    }
    clock_t t2 = clock();
    printf("nedmalloc : %d\n",int(t2 - t1));
    
    t1 = clock();
    for (int i = 0; i < 10000000; i ++)
    {
        int* pMem = (int*)new int(4);
        *pMem = 19;
        //free(pMem);
    }
    t2 = clock();
    printf("malloc : %d\n",int(t2 - t1));
    
    getchar();
    
    
    return 0;
}
