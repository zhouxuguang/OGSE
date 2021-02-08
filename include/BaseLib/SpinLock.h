#ifndef BASELIB_SPINLOCK_INCLUDE_H_DKF87G
#define BASELIB_SPINLOCK_INCLUDE_H_DKF87G

//璺ㄥ钩鍙拌嚜鏃嬮攣鐨勫疄鐜?

#include "PreCompile.h"

NS_BASELIB_BEGIN

class BASELIB_API SpinLock
{
public:
	SpinLock(void);

	~SpinLock(void);

	void Lock();

	bool TryLock();

	void UnLock();

private:
	spin_lock_t m_SpinLock;

	SpinLock(const SpinLock&);
	SpinLock& operator= (const SpinLock&);
};

NS_BASELIB_END

#endif
