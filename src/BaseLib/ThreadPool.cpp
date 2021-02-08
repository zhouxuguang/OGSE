//
//  ThreadPool.cpp
//  BaseLib
//
//  Created by zhouxuguang on 16/9/28.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#include "ThreadPool.h"
#include "AtomicOps.h"

NS_BASELIB_BEGIN

typedef std::vector<Thread> ThreadQueue;

//static void InitThreadPool(ThreadQueue& threads,TaskQueue& taskQueue,int nThreads,int nMaxTasks)
//{
//    threads.resize(nThreads);
////    for (int i = 0; i < nThreads; i ++)
////    {
////        threads[i].Resume();
////    }
//    
//    taskQueue.SetMaxTaskCount(nMaxTasks);
//}

ThreadPool::ThreadPool():mShutDown(true),mFullCondition(&mLock),mEmptyCondition(&mLock)
{
    mThreads.resize(8);
    mMaxTasks = 10000;
}

ThreadPool::ThreadPool(int nThreadCount, int nMaxTaskCount):mShutDown(true),mFullCondition(&mLock),mEmptyCondition(&mLock)
{
    mThreads.resize(8);
    mMaxTasks = 10000;
}

ThreadPool::~ThreadPool()
{
    ShutDown();
}

void ThreadPool::ShutDown()
{
    mShutDown = true;
    size_t nThreads = mThreads.size();
    for (size_t i = 0; i < nThreads; i ++)
    {
        mThreads[i].Stop();
    }
    mThreads.clear();
}

bool ThreadPool::IsRunning() const
{
    return !mShutDown;
}

int ThreadPool::GetThreadCount() const
{
    AutoLock lock_guard(&mLock);
    return (int)mThreads.size();
}

int ThreadPool::GetTaskCount() const
{
    AutoLock lock_guard(&mLock);
    return (int)mTaskList.size();
}

void ThreadPool::Execute(const SharedPtr<TaskRunner> &task,TaskQueue::TaskStrategy strategy)
{
    mLock.Lock();
    int nTaskCount = (int)mTaskList.size();
    mLock.UnLock();
    
    //如果超过最大任务数量，根据不同策略做不同的操作
    if(nTaskCount >= mMaxTasks)
    {
        if (strategy == TaskQueue::REMOVE_LAST)
        {
            AutoLock lockGuard(&mLock);
            mTaskList.pop_back();
            mTaskList.push_back(task);
        }
        
        else if (strategy == TaskQueue::REMOVE_FIRST)
        {
            AutoLock lockGuard(&mLock);
            mTaskList.pop_front();
            mTaskList.push_back(task);
        }
        
        else if (strategy == TaskQueue::NO_REMOVE)
        {
            AutoLock lockGuard(&mLock);
            
            while (m_bFullQueue)
            {
                mFullCondition.Wait();
            }
            
            m_bFullQueue = true;
            
        }
    }
    
    //正常插入
    else if (nTaskCount < mMaxTasks)
    {
        AutoLock lockGuard(&mLock);
        mTaskList.push_back(task);
        
        m_bEmptyQueue = false;
        mEmptyCondition.NotifyAll();
    }
}

SharedPtr<TaskRunner> ThreadPool::GetHeadTask()
{
    AutoLock lockGuard(&mLock);
    
    if (mTaskList.empty())
    {
        return NULL;
    }
    
    else
    {
        SharedPtr<TaskRunner> task = mTaskList.front();
        mTaskList.pop_front();
        m_bFullQueue = false;
        mFullCondition.NotifyAll();
        
        return task;
    }
}

void* WorkFunc(void* pArgs)
{
    ThreadPool* pThreadPool = (ThreadPool*)pArgs;
    while (pThreadPool && pThreadPool->IsRunning())
    {
        SharedPtr<TaskRunner> pTask = pThreadPool->GetHeadTask();
        if (NULL == pTask)
        {
            //表示队列里面没有数据了，线程阻塞
            MutexLock& lock = pThreadPool->mLock;
            Condition& cond = pThreadPool->mEmptyCondition;
            
            lock.Lock();
			while (pThreadPool->m_bEmptyQueue)
			{
				cond.Wait();
			}
            
            pThreadPool->m_bEmptyQueue = true;
            lock.UnLock();
        }
        
        else
        {
            assert(pTask != NULL);
            pTask->Run();
            printf("Task ID = %d\n",pTask->GetTaskId());
        }
        
    }
    
    return NULL;
}

void ThreadPool::Start()
{
    if (!mShutDown)
    {
        return;
    }

	mShutDown = false;

    size_t nThreads = mThreads.size();
    for (size_t i = 0; i < nThreads; i ++)
    {
        mThreads[i].Start(WorkFunc,this);
    }
    
}

NS_BASELIB_END
