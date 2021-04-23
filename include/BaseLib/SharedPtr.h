/**
* @file              SysSmartPointer.h
* @brief             智能指针   废弃了，不用了，有了c++11的智能指针，还用这个干啥，这个是以前早轮子写的
* @details           
* @author            zxg (zhouxuguang@.com)
* @date              2014年3月14日
* @version           1.0.0.1
* @par               Copyright (c):周旭光 2014
* @par               History:
*/
#ifndef __SYSSMARTPOINTER_H_47709FE9_0B67_4F2C_9448_9DC850E62468__
#define __SYSSMARTPOINTER_H_47709FE9_0B67_4F2C_9448_9DC850E62468__

#include "PreCompile.h"
#include "HashFunction.h"

NS_BASELIB_BEGIN

template <class D>
struct DeleteObject //: public std::unary_function<D,void>
{
    void operator()(D* pObj)
    {
        delete pObj;
    }
};

class Reference;

template <class T,class D = DeleteObject<T>() >
class SharedPtr
{

public:
	SharedPtr();

	SharedPtr(T *p);

	~SharedPtr(void);

	SharedPtr(const SharedPtr<T,D> &rhs);

	SharedPtr<T,D>& operator =(const SharedPtr<T,D> &rhs);
    
    SharedPtr<T,D>& operator =(const void* pValue);

	//重载*运算符
	T& operator *();

	//重载->运算符
	T* operator ->();

	//测试是否相等
	bool IsEqualTo(const SharedPtr<T> &rhs) const;
    
    //获得原始指针
    T* get() const;

	//判断是否为空
	bool IsNull() const;

private:
	T *m_ptr;
	Reference* m_pUse;	//引用计数
    
    inline void swap(SharedPtr<T> &other)
    {
        std::swap(m_ptr, other.m_ptr);
        std::swap(m_pUse, other.m_pUse);
    }
};

//重载智能指针之间比较的函数
template <class T,class D>
bool operator ==(const SharedPtr<T,D> &lhs,const SharedPtr<T,D> &rhs);

template <class T,class D>
bool operator !=(const SharedPtr<T,D> &lhs,const SharedPtr<T,D> &rhs);

//重载和裸指针比较运算符
template <class T,class D>
bool operator ==(const SharedPtr<T,D> &lhs,void* pValue);

template <class T,class D>
bool operator !=(const SharedPtr<T,D> &lhs,void* pValue);

template <class T,class D>
bool operator ==(void* pValue,const SharedPtr<T,D> &rhs);

template <class T,class D>
bool operator !=(void* pValue,const SharedPtr<T,D> &rhs);

//重载小于运算符
template <class T,class D>
bool operator < (const SharedPtr<T,D> &lhs,const SharedPtr<T,D> &rhs);


//自定义等于函数子,默认的是equal_to
template<typename T>
struct SharedPtrEquality : std::binary_function<SharedPtr<T>, SharedPtr<T>, bool>
{
    bool operator()(SharedPtr<T> i1, SharedPtr<T> i2) const
    {
        return *i1 == *i2;
    }
};

//自定义等于函数子,默认的是std::hash
template<typename T>
struct SharedPtrHash : std::unary_function<SharedPtr<T>, std::size_t>
{
    std::size_t operator()(SharedPtr<T> x) const
    {
        return HashFunction(x.get(),sizeof(T));
    }
};

template <typename T>
std::size_t hash_value(SharedPtr<T> const & _ptr)
{
    return HashFunction(_ptr.get(),sizeof(T));
}

template<typename T>
size_t hash(const SharedPtr<T> & ptr)
{
    return ((size_t) ptr.get()) / sizeof(T);
}

//template<class Ty>
//struct std::hash<SharedPtr<Ty> >
//: public unary_function<SharedPtr<Ty>, size_t>
//{
//    typedef SharedPtr<Ty> _Kty;
//    typedef Ty *_Ptrtype;
//    
//    size_t operator()(const _Kty& _Keyval) const
//    {
//        return (hash<_Ptrtype>()(_Keyval.get()));
//    }
//};

//template <typename T >
//struct std::hash<SharedPtr<T> >
//: public unary_function<SharedPtr<T>, size_t>
//{
//    typedef SharedPtr<T> _Kty;
//    typedef T *_Ptrtype;
//    
//    size_t operator()(const _Kty& _Keyval) const
//    {
//        return (hash<_Ptrtype>()(_Keyval.get()));
//    }
//};

//template<class T, class E>
//using first = T;
//
//template <typename T>
//struct std::hash<first<std::pair<T, T>, std::enable_if<std::is_unsigned<T>::value> > >
//{
//    size_t operator()(const std::pair<T, T>& x) const
//    {
//        return x;
//    }
//};

//template<typename T>
//struct std::hash<SharedPtr<T> > :
//private std::hash<T*>
//{
//    size_t operator()(const SharedPtr<T>& __s) const
//    {
//        return std::hash<T*>()(__s.get());
//    }
//};


NS_BASELIB_END

#include "SharedPtrPriv.h"

#endif

