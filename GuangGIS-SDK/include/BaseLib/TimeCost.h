#ifndef BASELIB_TIMECOST_INCLUDE_H
#define BASELIB_TIMECOST_INCLUDE_H


#include "PreCompile.h"

NS_BASELIB_BEGIN

class BASELIB_API TimeCost
{
public:
	TimeCost(void);

	~TimeCost(void);

	void Begin();

	void End();

    //获得的时间是微秒
	eUInt64 GetCostTime() const;
    
    //获得的时间是纳秒
    eUInt64 GetCostTimeNano() const;

private:
	eUInt64 m_nStart;
	eUInt64 m_nEnd;
};

NS_BASELIB_END

#endif
