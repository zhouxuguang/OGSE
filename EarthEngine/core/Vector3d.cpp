//
//  Vector3d.cpp
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#include "Vector3d.h"

EARTH_CORE_NAMESPACE_BEGIN

const Vector3d Vector3d::ZERO( 0, 0, 0 );
const Vector3d Vector3d::UNIT_SCALE(1, 1, 1);
const Vector3d Vector3d::UNIT_X(1, 0, 0);
const Vector3d Vector3d::UNIT_Y(0, 1, 0);
const Vector3d Vector3d::UNIT_Z(0, 0, 1);

const Real Vector3d::Length() const
{
    return sqrt(x * x + y * y + z*z);
}

Real Vector3d::Length()
{
    return sqrt(x * x + y * y + z*z);
}

Real Vector3d::DotProduct(const Vector3d &a) const
{
    return x*a.x + y*a.y + z*a.z;
}

void Vector3d::Normalize()
{
    Real magSq = x*x + y*y + z*z;
    if (magSq > 0.0f)
    {
        float oneOverMag = 1.0f / sqrt(magSq);
        x *= oneOverMag;
        y *= oneOverMag;
        z *= oneOverMag;
    }
}

Real Vector3d::LengthSq() const
{
    return x * x + y * y + z*z;
}

Vector3d Vector3d::Reflection(const Vector3d& vecLight,const Vector3d& vecNormal)
{
    return vecNormal*(vecNormal.DotProduct(vecNormal))*2 - vecLight;
}

Vector3d Vector3d::Refraction(const Vector3d& vecLight,const Vector3d& vecNormal,float eta)
{
    Real k = 1.0 - eta*eta *(1.0-vecNormal.DotProduct(vecLight)*vecNormal.DotProduct(vecLight));
    if ( k < 0)
    {
        return Vector3d::ZERO;
    }
    
    else
        return vecLight*eta - vecNormal * (eta*vecNormal.DotProduct(vecLight)+sqrt(k));
}

Vector3d CrossProduct(const Vector3d &a, const Vector3d &b)
{
    return Vector3d(
                   a.y*b.z - a.z*b.y,
                   a.z*b.x - a.x*b.z,
                   a.x*b.y - a.y*b.x
                   );
}

Real Distance(const Vector3d &a, const Vector3d &b)
{
    Real dx = a.x - b.x;
    Real dy = a.y - b.y;
    Real dz = a.z - b.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

Real DistanceSquared(const Vector3d &a, const Vector3d &b)
{
    Real dx = a.x - b.x;
    Real dy = a.y - b.y;
    Real dz = a.z - b.z;
    return dx*dx + dy*dy + dz*dz;
}

EARTH_CORE_NAMESPACE_END
