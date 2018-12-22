/**
* @file              SysSmartPointer.h
* @brief             智能指针
* @details           
* @author            zxg (zhouxuguang@aeroimginfo.com)
* @date              2015年5月14日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2015
* @par               History:
*/
#ifndef __SMARTPOINTERPRIV_INCLUDE_H_47709FE9_0B67_4F2C_9448_9DC850E62468__
#define __SMARTPOINTERPRIV_INCLUDE_H_47709FE9_0B67_4F2C_9448_9DC850E62468__


#include "Reference.h"

NS_BASELIB_BEGIN


template <class T,class D>
SharedPtr<T,D>::SharedPtr() : m_ptr(NULL)
{
	m_pUse = new(std::nothrow) Reference();
}

template <class T,class D>
SharedPtr<T,D>::SharedPtr(T *p) : m_ptr(p)
{
	m_pUse = new(std::nothrow) Reference();
}

template <class T,class D>
SharedPtr<T,D>::SharedPtr(const SharedPtr<T,D> &rhs)
{
	m_ptr = rhs.m_ptr;
	m_pUse = rhs.m_pUse;
    if (m_ptr != NULL)
    {
        m_pUse->AddReference();
    }
}

template <class T,class D>
SharedPtr<T,D>& SharedPtr<T,D>::operator = (const SharedPtr<T,D> &rhs)
{
	if (rhs == *this)
	{
		return *this;
	}
    
    SharedPtr<T> tmp(rhs);
    swap(tmp);
    if (m_ptr != NULL)
    {
        m_pUse->AddReference();
    }

	return *this;
}

template <class T,class D>
SharedPtr<T,D>& SharedPtr<T,D>::operator = (const void* pValue)
{
    if (pValue == *this)
    {
        return *this;
    }
    
    m_ptr = pValue;
    //m_pUse = rhs.m_pUse;
    m_pUse->AddReference();

}

template <class T,class D>
SharedPtr<T,D>::~SharedPtr(void)
{
	unsigned long nCount = m_pUse->ReleaseReference();
	if (0 == nCount)
	{
		if (m_ptr != NULL)
		{
			delete m_ptr;
			m_ptr = NULL;
		}

		if (m_pUse != NULL)
		{
			delete m_pUse;
			m_pUse = NULL;
		}
	}
	
}

//重载*运算符
template <class T,class D>
T& SharedPtr<T,D>::operator *()
{
	return *m_ptr;
}

//重载->运算符
template <class T,class D>
T* SharedPtr<T,D>::operator ->()
{
	return m_ptr;
}

template <class T,class D>
bool SharedPtr<T,D>::IsEqualTo(const SharedPtr<T> &rhs) const
{
	if (m_ptr == rhs.m_ptr && m_pUse == rhs.m_pUse)
	{
		return true;
	}

	return false;
}

template <class T,class D>
T* SharedPtr<T,D>::get() const
{
    return m_ptr;
}

template <class T,class D>
bool SharedPtr<T,D>::IsNull() const
{
	return m_ptr == NULL;
}

//重载自由函数
template <class T,class D>
bool operator ==(const SharedPtr<T,D> &lhs,const SharedPtr<T,D> &rhs)
{
	return lhs.IsEqualTo(rhs);
}

template <class T,class D>
bool operator !=(const SharedPtr<T,D> &lhs,const SharedPtr<T,D> &rhs)
{
    return !lhs.IsEqualTo(rhs);
}

template <class T,class D>
bool operator ==(const SharedPtr<T,D> &lhs,void* pValue)
{
    return lhs.get() == pValue;
}

template <class T,class D>
bool operator !=(const SharedPtr<T,D> &lhs,void* pValue)
{
    return lhs.get() != pValue;
}

template <class T,class D>
bool operator ==(void* pValue,const SharedPtr<T,D> &rhs)
{
    return pValue == rhs.get();
}

template <class T,class D>
bool operator !=(void* pValue,const SharedPtr<T,D> &rhs)
{
    return pValue != rhs.get();
}

template <class T,class D>
bool operator < (const SharedPtr<T,D> &lhs,const SharedPtr<T,D> &rhs)
{
    return lhs.get() < rhs.get();
}

NS_BASELIB_END

#endif
