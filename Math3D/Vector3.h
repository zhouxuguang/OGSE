/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2014
@ 文件名称	: vector3.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2014年12月1日
@ 内容摘要	: 
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __VECTOR3_H_CD2C4FC7_29F5_439D_9403_106DAA2DC4CA__
#define __VECTOR3_H_CD2C4FC7_29F5_439D_9403_106DAA2DC4CA__

#include "Math3DCommon.h"

NS_MATH3D_BEGIN


class MATH3D_API Vector3 
{
public:

	Real x,y,z;

	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3(const Vector3 &a) : x(a.x), y(a.y), z(a.z) {}

	Vector3(Real nx, Real ny, Real nz) : x(nx), y(ny), z(nz) {}

	Vector3(const Vector3& startVec,const Vector3& endVec)
	{
		x = endVec.x - startVec.x;
		y = endVec.y - startVec.y;
		z = endVec.z - startVec.z;
	}


	Vector3 &operator =(const Vector3 &a) 
	{
		x = a.x; y = a.y; z = a.z;
		return *this;
	}

	bool operator ==(const Vector3 &a) const 
	{
		return x==a.x && y==a.y && z==a.z;
	}

	bool operator !=(const Vector3 &a) const 
	{
		return x!=a.x || y!=a.y || z!=a.z;
	}


	void Zero() { x = y = z = 0.0f; }

	Vector3 Multiply(const Vector3& a) const 
	{
		return Vector3(x*a.x, y*a.y, z*a.z);
	}

	Vector3 operator -() const 
	{ 
		return Vector3(-x,-y,-z); 
	}


	Vector3 operator +(const Vector3 &a) const 
	{
		return Vector3(x + a.x, y + a.y, z + a.z);
	}

	Vector3 operator -(const Vector3 &a) const 
	{
		return Vector3(x - a.x, y - a.y, z - a.z);
	}

	Vector3 operator *(Real a) const 
	{
		return Vector3(x*a, y*a, z*a);
	}

	Vector3 operator /(Real a) const 
	{
		Real oneOverA = 1.0f / a; 
		return Vector3(x*oneOverA, y*oneOverA, z*oneOverA);
	}

	Vector3 &operator +=(const Vector3 &a) 
	{
		x += a.x; y += a.y; z += a.z;
		return *this;
	}

	Vector3 &operator -=(const Vector3 &a) 
	{
		x -= a.x; y -= a.y; z -= a.z;
		return *this;
	}

	Vector3 &operator *=(Real a) 
	{
		x *= a; y *= a; z *= a;
		return *this;
	}

	Vector3 &operator /=(Real a) 
	{
		Real oneOverA = 1.0f / a;
		x *= oneOverA; y *= oneOverA; z *= oneOverA;
		return *this;
	}
    
    inline const Real& operator [](size_t nIndex) const
    {
        assert(nIndex < 4);
        return *(&x+nIndex);
    }
    
    inline Real& operator [](size_t nIndex)
    {
        assert(nIndex < 4);
        return *(&x+nIndex);
    }

	void Normalize();

	//求向量的模
	const Real Length() const;

	Real Length();

	//模的平方
	Real LengthSq() const;

    //乘积，对应分量相乘
	Vector3 operator *(const Vector3 &a) const
	{
		return Vector3(x*a.x , y*a.y , z*a.z);
	}

	Real DotProduct(const Vector3 &a) const;

	//根据入射光线和法线计算反射向量，必须要注意的是，入射光线和法线必须是单位向量
	static Vector3 Reflection(const Vector3& vecLight,const Vector3& vecNormal);

	static Vector3 Refraction(const Vector3& vecLight,const Vector3& vecNormal,float eta);

    //常量向量定义
	static const Vector3 ZERO;
	static const Vector3 UNIT_SCALE;
    static const Vector3 UNIT_X;
    static const Vector3 UNIT_Y;
    static const Vector3 UNIT_Z;
};

inline Real vectorMag(const Vector3 &a) 
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vector3 CrossProduct(const Vector3 &a, const Vector3 &b);

inline Vector3 operator *(float k, const Vector3 &v) 
{
	return Vector3(k*v.x, k*v.y, k*v.z);
}

Real Distance(const Vector3 &a, const Vector3 &b);

Real DistanceSquared(const Vector3 &a, const Vector3 &b);


extern const Vector3 kZeroVector;

NS_MATH3D_END

#endif 
