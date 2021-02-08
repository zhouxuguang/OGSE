//�������
#ifndef BASELIB_TASKQUEUE_INCLUDE_H
#define BASELIB_TASKQUEUE_INCLUDE_H

#include "TaskRunner.h"
#include "MutexLock.h"
#include "Condition.h"
#include "SharedPtr.h"

NS_BASELIB_BEGIN

class BASELIB_API TaskQueue
{
public:
    
    enum TaskStrategy
    {
        REMOVE_LAST = 0,        //�Ƴ���������
        REMOVE_FIRST = 1,       //�Ƴ���һ������
        NO_REMOVE = 2           //���Ƴ�����
    };
    
	explicit TaskQueue(int nMaxTaskCount = 100);

	~TaskQueue(void);
    
    /**
     *  �������
     *
     *  @param task         ����
     *  @param TaskStrategy ��������
     */
    bool AddTask(const SharedPtr<TaskRunner>& task,TaskStrategy strategy = NO_REMOVE);
    
    /**
     *  �Ƴ�����
     *
     *  @param task ����
     *
     *  @return �ɹ�����true
     */
    bool RemoveTask(const SharedPtr<TaskRunner>& task);
    
    /**
     *  �������ػ����ǰ���Task
     *
     *  @return ����
     */
    SharedPtr<TaskRunner> GetHeadTask();
    
    /**
     *  �Ƿ�Ϊ��
     *
     *  @return Ϊ�շ���true
     */
    bool IsEmpty() const;
    
    /**
     *  ����������
     */
    void Clear();
    
    /**
     *  ���������������
     *
     *  @param nMaxTaskCount ��������
     */
    void SetMaxTaskCount(int nMaxTaskCount);
    
    /**
     *  �����������
     *
     *  @return ��������
     */
    int GetTaskCount() const;

private:
	typedef std::deque< SharedPtr<TaskRunner> > TaskList;
	TaskList mTaskList;         //�����б�
    MutexLock mLock;            //������
    Condition mFullCondition;       //����������������
    bool     m_bFullQueue;
    Condition mEmptyCondition;      //����Ϊ�յ���������
    bool     m_bEmptyQueue;
    
    int mMaxTaskCount;          //�����������
};

NS_BASELIB_END

#endif
