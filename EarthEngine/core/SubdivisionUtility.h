//
//  SubdivisionUtility.h
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#ifndef SubdivisionUtility_hpp
#define SubdivisionUtility_hpp

#include "EarthEngineDefine.h"
#include "Vector2.h"
#include "Vector3d.h"


//球面网格剖分的工具类

EARTH_CORE_NAMESPACE_BEGIN

class SubdivisionUtility
{
public:
    
    //计算有多少个三角形
    static int NumberOfTriangles(int numberOfSubdivisions);
    
    //计算有多少个顶点
    static int NumberOfVertices(int numberOfSubdivisions);
    
    //计算纹理坐标
    static math3d::Vector2 ComputeTextureCoordinate(Vector3d position);
    
private:
    SubdivisionUtility();
    ~SubdivisionUtility();
    SubdivisionUtility(const SubdivisionUtility&);
    SubdivisionUtility& operator = (const SubdivisionUtility&);
};

EARTH_CORE_NAMESPACE_END

#endif /* SubdivisionUtility_h */
