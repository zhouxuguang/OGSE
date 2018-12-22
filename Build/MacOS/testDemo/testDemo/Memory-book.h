//
//  Memory-book.h
//  TestDemo
//
//  Created by Zhou,Xuguang on 17/7/9.
//  Copyright © 2017年 Zhou,Xuguang. All rights reserved.
//

#ifndef Memory_book_h
#define Memory_book_h

template<typename T>
class CMemoryPool
{
public:
    enum { EXPANSION_SIZE = 32};
    
    CMemoryPool(unsigned int nItemCount = EXPANSION_SIZE)
    {
        ExpandFreeList(nItemCount);
    }
    
    ~CMemoryPool()
    {
        //free all memory in the list
        CMemoryPool<T>* pNext = NULL;
        for(pNext = m_pFreeList; pNext != NULL; pNext = m_pFreeList)
        {
            m_pFreeList = m_pFreeList->m_pFreeList;
            delete [](char*)pNext;
        }
    }
    
    void* Alloc(unsigned int size)
    {
        if(m_pFreeList == NULL)
        {
            ExpandFreeList();
        }
        
        //get free memory from head
        CMemoryPool<T>* pHead = m_pFreeList;
        m_pFreeList = m_pFreeList->m_pFreeList;
        return pHead;
    }
    
    void Free(void* p)
    {
        //push the free memory back to list
        CMemoryPool<T>* pHead = static_cast<CMemoryPool<T>*>(p);
        pHead->m_pFreeList = m_pFreeList;
        m_pFreeList = pHead;
    }
    
protected:
    //allocate memory and push to the list
    void ExpandFreeList(unsigned nItemCount = EXPANSION_SIZE)
    {
        unsigned int nSize = sizeof(T) > sizeof(CMemoryPool<T>*) ? sizeof(T) : sizeof(CMemoryPool<T>*);
        CMemoryPool<T>* pLastItem = static_cast<CMemoryPool<T>*>(static_cast<void*>(new char[nSize]));
        m_pFreeList = pLastItem;
        for(int i=0; i<nItemCount-1; ++i)
        {
            pLastItem->m_pFreeList = static_cast<CMemoryPool<T>*>(static_cast<void*>(new char[nSize]));
            pLastItem = pLastItem->m_pFreeList;
        }
        
        pLastItem->m_pFreeList = NULL;
    }
    
private:
    CMemoryPool<T>* m_pFreeList;
};


#define CHUNK_SIZE 16

class MemoryPoolImpl
{
public:
    MemoryPoolImpl(size_t nChunkCount = 128)
    {
        m_pData = (unsigned char*)malloc(nChunkCount * CHUNK_SIZE);
        for (size_t i = 0; i < nChunkCount; i ++)
        {
            m_vecFreeList.push_back(m_pData + i * CHUNK_SIZE);
        }
    }
    
    void* Alloc(size_t size)
    {
        size_t nChunkCount = size / CHUNK_SIZE;
        if (size % CHUNK_SIZE)
        {
            nChunkCount += 1;
        }
        if(nChunkCount > m_vecFreeList.size())
        {
            //ExpandFreeList();
            //扩展内存
        }
        
        //get free memory from head
        unsigned char* pHead = m_vecFreeList[0];
        m_vecFreeList.erase(m_vecFreeList.begin());
        return pHead;
    }
    
    void Free(void* p)
    {
        //push the free memory back to list
        unsigned char* pHead = (unsigned char*)p;
        m_vecFreeList.push_back(pHead);
    }

    
private:
    unsigned char* m_pData;
    std::vector<unsigned char*> m_vecFreeList;
};


#endif /* Memory_book_h */
