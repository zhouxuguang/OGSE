//
//  GeoGridTessellator.h
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#ifndef EARTH_ENGINE_CORE_GEOGRID_TESSELLATOR_INCLUD_H
#define EARTH_ENGINE_CORE_GEOGRID_TESSELLATOR_INCLUD_H

#include <vector>
#include "EarthEngineDefine.h"
#include "Ellipsoid.h"
#include "Vector2.h"

EARTH_CORE_NAMESPACE_BEGIN

class GeoGridTessellator
{
public:
    enum GeoGridVertexAttributes
    {
        Position = 1,
        Normal = 2,
        TextureCoordinate = 4,
        All = Position | Normal | TextureCoordinate
    };
    
public:
    
    //计算地球网格
    static void Compute(Ellipsoid& ellipsoid,
                        int numberOfSlicePartitions,
                        int numberOfStackPartitions,
                        GeoGridVertexAttributes vertexAttributes,
                        std::vector<Vector3d>& vecPosition,
                        std::vector<Vector3d>& vecNormal,
                        std::vector<math3d::Vector2> &vecTexturePoint,
                        std::vector<unsigned int> &vecVertexIndice);
};


EARTH_CORE_NAMESPACE_END

#endif /* EARTH_ENGINE_CORE_GEOGRID_TESSELLATOR_INCLUD_H */
