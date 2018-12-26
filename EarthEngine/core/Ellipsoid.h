//
//  Ellipsoid.h
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#ifndef EARTH_ENGINE_ELLIPSOID_INCLUDE_H
#define EARTH_ENGINE_ELLIPSOID_INCLUDE_H

#include "EarthEngineDefine.h"
#include "Vector3d.h"


//大地椭球体

EARTH_CORE_NAMESPACE_BEGIN

class Geodetic3D;

class Ellipsoid
{
public:
    Ellipsoid(double x, double y, double z);
    
    Ellipsoid(const Vector3d &vec3);
    
    Vector3d& GetAxis();
    
    Vector3d& GetmOneOverRadiiSquared();
    
    //计算椭球上一点某一点处的法向量
    Vector3d GeodeticSurfaceNormal(const Vector3d& positionOnEllipsoid);
    
    //根据经纬度和大地高计算椭球体上的法线向量
    Vector3d GeodeticSurfaceNormal(const Geodetic3D& geodetic);
    
private:
    Vector3d mAxisLength;  //三轴的长度
    
    Vector3d mOneOverRadiiSquared;   // 1/a2
    
public:
    static Ellipsoid Wgs84;
    static const Ellipsoid ScaledWgs84;
    static const Ellipsoid UnitSphere;
};

EARTH_CORE_NAMESPACE_END

#endif /* EARTH_ENGINE_ELLIPSOID_INCLUDE_H */
