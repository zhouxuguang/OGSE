#include "Reference.h"
#include "AtomicOps.h"

NS_BASELIB_BEGIN

Reference::Reference():m_nRef(1)
{
	
}

Reference::~Reference()
{
}

eUInt32 Reference::ReleaseReference()
{
	// 给指针记录独占地自减
	if (static_cast<long>(m_nRef) == 0)
		return 0;
	if (!CBLAtomicDecrement((volatile int*)(&m_nRef)))
	{
		return 0;
	}

	return m_nRef;
}

eUInt32 Reference::AddReference()
{
	CBLAtomicIncrement((volatile int*)(&m_nRef));
	return m_nRef;
}

eUInt32 Reference::GetReferenceCount() const
{
    return this->m_nRef;
}

NS_BASELIB_END
