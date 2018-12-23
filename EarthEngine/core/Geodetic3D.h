//
//  Geodetic3D.h
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#ifndef Geodetic3D_hpp
#define Geodetic3D_hpp

#include <stdio.h>
#include "EarthEngineDefine.h"

EARTH_CORE_NAMESPACE_BEGIN

class Geodetic3D
{
public:
    Geodetic3D(double longitude, double latitude, double height)
    {
        _longitude = longitude;
        _latitude = latitude;
        _height = height;
    }
    
    Geodetic3D(double longitude, double latitude)
    {
        _longitude = longitude;
        _latitude = latitude;
        _height = 0;
    }
    
    double Longitude() const
    {
        return _longitude;
    }
    
    double Latitude() const
    {
        return _latitude;
    }
    
    double Height() const
    {
        return _height;
    }
    
private:
    double _longitude;
    double _latitude;
    double _height;
};


EARTH_CORE_NAMESPACE_END

#endif /* Geodetic3D_hpp */
