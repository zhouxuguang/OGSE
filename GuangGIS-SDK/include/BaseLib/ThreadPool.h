//
//  ThreadPool.h
//  BaseLib
//
//  Created by zhouxuguang on 16/9/28.
//  Copyright © 2016年 zhouxuguang. All rights reserved.
//

#ifndef BASELIB_THREADPOOL_INCLUDE_H_FFGD3FGGHR
#define BASELIB_THREADPOOL_INCLUDE_H_FFGD3FGGHR

#include "Thread.h"
#include "TaskQueue.h"

NS_BASELIB_BEGIN

class BASELIB_API ThreadPool
{
public:
    ThreadPool();
    
    ThreadPool(int nThreadCount, int nMaxTaskCount);
    
    ~ThreadPool();
    
    void ShutDown();
    
    /**
     *  将任务放入任务队列，线程池从中取任务
     *
     *  @param task        任务
     *  @param ::NO_REMOVE 策略类型
     */
    void Execute(const SharedPtr<TaskRunner>& task,
                 TaskQueue::TaskStrategy strategy = TaskQueue::NO_REMOVE);
    
    /**
     *  获得队列中第一个线程
     */
    SharedPtr<TaskRunner> GetHeadTask();
    
    /**
     *  启动函数
     */
    void Start();
    
    /**
     *  线程池是否在运行
     */
    bool IsRunning() const;
    
    /**
     *  线程池中线程的个数
     */
    int GetThreadCount() const;
    
    /**
     *  线程池中还没执行的任务个数
     */
    int GetTaskCount() const;
    
private:
    typedef std::vector<Thread> ThreadQueue;
    ThreadQueue mThreads;
    
    typedef std::deque< SharedPtr<TaskRunner> > TaskList;
    TaskList mTaskList;         //任务列表
    eUInt32  mMaxTasks;        //最大任务个数
    
    //TaskQueue mTaskQueue;       //任务队列
    bool mShutDown;             //是否关闭线程池
    
    MutexLock mLock;
    
    Condition mFullCondition;       //任务队列满的条件变量
    bool      m_bFullQueue;            //是否满的标志
    Condition mEmptyCondition;      //
    bool      m_bEmptyQueue;
    
    //下列函数禁止复制
private:
    ThreadPool(const ThreadPool&);
    ThreadPool& operator = (const ThreadPool&);
    
    friend void* WorkFunc(void* pArgs);
};

NS_BASELIB_END

#endif /* BASELIB_THREADPOOL_INCLUDE_H_FFGD3FGGHR */
