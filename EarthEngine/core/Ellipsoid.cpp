//
//  Ellipsoid.cpp
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#include "Ellipsoid.h"
#include "Geodetic3D.h"

EARTH_CORE_NAMESPACE_BEGIN

const Ellipsoid Ellipsoid::Wgs84 = Ellipsoid(6378137.0, 6378137.0, 6356752.314245);
const Ellipsoid Ellipsoid::ScaledWgs84 = Ellipsoid(1.0, 1.0, 6356752.314245 / 6378137.0);
const Ellipsoid Ellipsoid::UnitSphere = Ellipsoid(1.0, 1.0, 1.0);

Ellipsoid::Ellipsoid(double x, double y, double z)
{
    mAxisLength.x = x;
    mAxisLength.y = y;
    mAxisLength.z = z;
    
    mOneOverRadiiSquared.x = 1.0 / (mAxisLength.x * mAxisLength.x);
    mOneOverRadiiSquared.y = 1.0 / (mAxisLength.y * mAxisLength.y);
    mOneOverRadiiSquared.z = 1.0 / (mAxisLength.z * mAxisLength.z);
}

Ellipsoid::Ellipsoid(const Vector3d& vec3)
{
    mAxisLength = vec3;
    
    mOneOverRadiiSquared.x = 1.0 / (mAxisLength.x * mAxisLength.x);
    mOneOverRadiiSquared.y = 1.0 / (mAxisLength.y * mAxisLength.y);
    mOneOverRadiiSquared.z = 1.0 / (mAxisLength.z * mAxisLength.z);
}

Vector3d Ellipsoid::GetAxis() const
{
    return mAxisLength;
}

Vector3d Ellipsoid::GetmOneOverRadiiSquared() const
{
    return mOneOverRadiiSquared;
}

Vector3d Ellipsoid::GeodeticSurfaceNormal(const Vector3d &positionOnEllipsoid) const
{
    Vector3d vecNormal = positionOnEllipsoid.Multiply(mOneOverRadiiSquared);
    vecNormal.Normalize();
    
    return vecNormal;
}

Vector3d Ellipsoid::GeodeticSurfaceNormal(const Geodetic3D& geodetic) const
{
    double cosLatitude = cos(geodetic.Latitude());
    
    return Vector3d(
                        cosLatitude * cos(geodetic.Longitude()),
                        cosLatitude * sin(geodetic.Longitude()),
                        sin(geodetic.Latitude()));
}

EARTH_CORE_NAMESPACE_END
