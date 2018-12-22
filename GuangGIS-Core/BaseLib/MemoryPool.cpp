//
//  MemoryPool.cpp
//  BaseLib
//
//  Created by Zhou,Xuguang on 17/7/9.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#include "MemoryPool.h"
#include "nedmalloc/nedmalloc.h"

NS_BASELIB_BEGIN

static nedalloc::nedpool* pNedPoll = nedalloc::nedcreatepool(1024*1024*50, 2);

MemoryPool::MemoryPool()
{
    //nedcreatepool
    //nedalloc::nedprealloc(nedpool *p, void *mem, size_t size)
}

void* MemoryPool::Allocate(size_t size)
{
    return nedalloc::nedpmalloc(pNedPoll, size);
}

void* MemoryPool::ReAllocate(void *mem, size_t size)
{
    return nedalloc::nedprealloc(pNedPoll, mem, size);
}

void* MemoryPool::AlignedAllocate(size_t alignment, size_t bytes)
{
    return nedalloc::nedpmemalign(pNedPoll, alignment, bytes);
}

void MemoryPool::Free(void *mem)
{
    return nedalloc::nedpfree(pNedPoll, mem);
}

NS_BASELIB_END
