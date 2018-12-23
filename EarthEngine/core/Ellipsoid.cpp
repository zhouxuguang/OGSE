//
//  Ellipsoid.cpp
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#include "Ellipsoid.h"

EARTH_CORE_NAMESPACE_BEGIN

const Ellipsoid Ellipsoid::Wgs84 = Ellipsoid(6378137.0, 6378137.0, 6356752.314245);
const Ellipsoid Ellipsoid::ScaledWgs84 = Ellipsoid(1.0, 1.0, 6356752.314245 / 6378137.0);
const Ellipsoid Ellipsoid::UnitSphere = Ellipsoid(1.0, 1.0, 1.0);

Ellipsoid::Ellipsoid(double x, double y, double z)
{
    mAxisLength.x = x;
    mAxisLength.y = y;
    mAxisLength.z = z;
}

Ellipsoid::Ellipsoid(const Vector3d& vec3)
{
    mAxisLength = vec3;
}

Vector3d& Ellipsoid::GetAxis()
{
    return mAxisLength;
}

EARTH_CORE_NAMESPACE_END
