//
//  SubdivisionUtility.cpp
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#include "SubdivisionUtility.h"

EARTH_CORE_NAMESPACE_BEGIN

int SubdivisionUtility::NumberOfTriangles(int numberOfSubdivisions)
{
    int numberOfTriangles = 0;
    for (int i = 0; i <= numberOfSubdivisions; ++i)
    {
        numberOfTriangles += pow(4.0, i);
    }
    numberOfTriangles *= 4;
    
    return numberOfTriangles;
}

int SubdivisionUtility::NumberOfVertices(int numberOfSubdivisions)
{
    int numberOfVertices = 0;
    for (int i = 0; i < numberOfSubdivisions; ++i)
    {
        numberOfVertices += pow(4.0, i);
    }
    numberOfVertices = 4 + (12 * numberOfVertices);
    
    return numberOfVertices;
}

math3d::Vector2 SubdivisionUtility::ComputeTextureCoordinate(Vector3d position)
{
    return math3d::Vector2((atan2(position.y, position.x) / (M_PI * 2.0)) + 0.5,
                   (asin(position.z) / M_PI) + 0.5);
    
//    double x = atan2(position.y, position.x) / (M_PI * 2.0) + 0.5;
//    double y = asin(position.z) / M_PI + 0.5;
//    
//    math3d::Vector2 vecResult;
//    vecResult.x = x;
//    vecResult.y = y;
//    
//    return vecResult;
}

EARTH_CORE_NAMESPACE_END
