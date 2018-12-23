//
//  Ellipsoid.h
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#ifndef Ellipsoid_hpp
#define Ellipsoid_hpp

#include <stdio.h>
#include "EarthEngineDefine.h"
#include "Vector3d.h"

EARTH_CORE_NAMESPACE_BEGIN

class Ellipsoid
{
public:
    Ellipsoid(double x, double y, double z);
    
    Ellipsoid(const Vector3d &vec3);
    
private:
    Vector3d mAxisLength;  //三
    
public:
    static const Ellipsoid Wgs84;
    static const Ellipsoid ScaledWgs84;
    static const Ellipsoid UnitSphere;
};

EARTH_CORE_NAMESPACE_END

#endif /* Ellipsoid_hpp */
