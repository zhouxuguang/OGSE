//
//  Vector3d.h
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#ifndef Vector3d_hpp
#define Vector3d_hpp

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "EarthEngineDefine.h"

EARTH_CORE_NAMESPACE_BEGIN

typedef double Real;

class Vector3d
{
public:
    
    Real x,y,z;
    
    Vector3d()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    
    Vector3d(const Vector3d &a) : x(a.x), y(a.y), z(a.z) {}
    
    Vector3d(Real nx, Real ny, Real nz) : x(nx), y(ny), z(nz) {}
    
    Vector3d(const Vector3d& startVec,const Vector3d& endVec)
    {
        x = endVec.x - startVec.x;
        y = endVec.y - startVec.y;
        z = endVec.z - startVec.z;
    }
    
    
    Vector3d &operator =(const Vector3d &a)
    {
        x = a.x; y = a.y; z = a.z;
        return *this;
    }
    
    bool operator ==(const Vector3d &a) const
    {
        return x==a.x && y==a.y && z==a.z;
    }
    
    bool operator !=(const Vector3d &a) const
    {
        return x!=a.x || y!=a.y || z!=a.z;
    }
    
    
    void Zero() { x = y = z = 0.0f; }
    
    Vector3d Multiply(const Vector3d& a) const
    {
        return Vector3d(x*a.x, y*a.y, z*a.z);
    }
    
    Vector3d operator -() const
    {
        return Vector3d(-x,-y,-z);
    }
    
    
    Vector3d operator +(const Vector3d &a) const
    {
        return Vector3d(x + a.x, y + a.y, z + a.z);
    }
    
    Vector3d operator -(const Vector3d &a) const
    {
        return Vector3d(x - a.x, y - a.y, z - a.z);
    }
    
    Vector3d operator *(Real a) const
    {
        return Vector3d(x*a, y*a, z*a);
    }
    
    Vector3d operator /(Real a) const
    {
        Real oneOverA = 1.0f / a;
        return Vector3d(x*oneOverA, y*oneOverA, z*oneOverA);
    }
    
    Vector3d &operator +=(const Vector3d &a)
    {
        x += a.x; y += a.y; z += a.z;
        return *this;
    }
    
    Vector3d &operator -=(const Vector3d &a)
    {
        x -= a.x; y -= a.y; z -= a.z;
        return *this;
    }
    
    Vector3d &operator *=(Real a)
    {
        x *= a; y *= a; z *= a;
        return *this;
    }
    
    Vector3d &operator /=(Real a)
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
    Vector3d operator *(const Vector3d &a) const
    {
        return Vector3d(x*a.x , y*a.y , z*a.z);
    }
    
    Real DotProduct(const Vector3d &a) const;
    
    //根据入射光线和法线计算反射向量，必须要注意的是，入射光线和法线必须是单位向量
    static Vector3d Reflection(const Vector3d& vecLight,const Vector3d& vecNormal);
    
    static Vector3d Refraction(const Vector3d& vecLight,const Vector3d& vecNormal,float eta);
    
    //常量向量定义
    static const Vector3d ZERO;
    static const Vector3d UNIT_SCALE;
    static const Vector3d UNIT_X;
    static const Vector3d UNIT_Y;
    static const Vector3d UNIT_Z;
};

inline Real vectorMag(const Vector3d &a)
{
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

Vector3d CrossProduct(const Vector3d &a, const Vector3d &b);

inline Vector3d operator *(float k, const Vector3d &v)
{
    return Vector3d(k*v.x, k*v.y, k*v.z);
}

Real Distance(const Vector3d &a, const Vector3d &b);

Real DistanceSquared(const Vector3d &a, const Vector3d &b);


extern const Vector3d kZeroVector;

EARTH_CORE_NAMESPACE_END

#endif /* Vector3d_hpp */
