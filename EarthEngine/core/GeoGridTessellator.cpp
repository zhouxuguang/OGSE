//
//  GeoGridTessellator.cpp
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#include "GeoGridTessellator.h"
#include "SubdivisionUtility.h"
#include "GeoEllipsoid.h"

EARTH_CORE_NAMESPACE_BEGIN

static int NumberOfTriangles(int numberOfSlicePartitions, int numberOfStackPartitions)
{
    int numberOfTriangles = 2 * numberOfSlicePartitions;                                // Top and bottom fans
    numberOfTriangles += 2 * ((numberOfStackPartitions - 2) * numberOfSlicePartitions); // Middle triangle strips
    return numberOfTriangles;
}

static int NumberOfVertices(int numberOfSlicePartitions, int numberOfStackPartitions)
{
    return 2 + ((numberOfStackPartitions - 1) * numberOfSlicePartitions);
}

void GeoGridTessellator::Compute(Ellipsoid& ellipsoid,
                                 int numberOfSlicePartitions,
                                 int numberOfStackPartitions,
                                 GeoGridVertexAttributes vertexAttributes,
                                 std::vector<Vector3d>& vecPosition,
                                 std::vector<Vector3d>& vecNormal,
                                 std::vector<math3d::Vector2> &vecTexturePoint,
                                 std::vector<unsigned int> &vecVertexIndice)
{
    //经度划分网格数
    if (numberOfSlicePartitions < 3)
    {
        return;
    }
    
    //纬度划分数
    if (numberOfStackPartitions < 2)
    {
        return;
    }
    
    //
    // 创建查找表
    //
    std::vector<double> cosTheta(numberOfSlicePartitions);
    std::vector<double> sinTheta(numberOfSlicePartitions);
    
    for (int j = 0; j < numberOfSlicePartitions; ++j)
    {
        double theta = (M_PI * 2.0) * (((double)j) / numberOfSlicePartitions);
        cosTheta[j] = cos(theta);
        sinTheta[j] = sin(theta);
    }
    
    //
    // 创建顶点
    //
    
    int numberOfVertices = NumberOfVertices(numberOfSlicePartitions, numberOfStackPartitions);
    vecPosition.reserve(numberOfVertices);
    vecPosition.emplace_back(0, 0, ellipsoid.GetAxis().z);
    
    for (int i = 1; i < numberOfStackPartitions; ++i)
    {
        double phi = M_PI * (((double)i) / numberOfStackPartitions);
        double sinPhi = sin(phi);
        
        double xSinPhi = ellipsoid.GetAxis().x * sinPhi;
        double ySinPhi = ellipsoid.GetAxis().y * sinPhi;
        double zCosPhi = ellipsoid.GetAxis().z * cos(phi);
        
        for (int j = 0; j < numberOfSlicePartitions; ++j)
        {
            double x = cosTheta[j] * xSinPhi;
            double y = sinTheta[j] * ySinPhi;
            double z = zCosPhi;
            
            double theta = (M_PI * 2.0) * (((double)j) / numberOfSlicePartitions);
            
            //经度
            if (theta > M_PI)
            {
                theta -= (M_PI * 2.0);
            }
            
            theta *= RAD_TO_DEG;
            
            //纬度
            double phi = M_PI * (((double)i) / numberOfStackPartitions);
            phi = -(phi - M_PI_2);
            
            phi *= RAD_TO_DEG;
            
            GeoEllipsoid elli = GeoEllipsoid(ellipsoid.GetAxis().x, ellipsoid.GetAxis().z);
            
            double x1 = 0;
            double y1= 0;
            double z1 = 0;
            elli.LatLonHeightToXYZ(phi, theta, 0, x1, y1, z1);
            
            vecPosition.emplace_back(cosTheta[j] * xSinPhi, sinTheta[j] * ySinPhi, zCosPhi);
        }
    }
    vecPosition.emplace_back(0, 0, -ellipsoid.GetAxis().z);
    
    //计算法向量和纹理坐标
    if ((vertexAttributes & GeoGridVertexAttributes::Normal) == GeoGridVertexAttributes::Normal ||
        (vertexAttributes & GeoGridVertexAttributes::TextureCoordinate) == GeoGridVertexAttributes::TextureCoordinate)
    {
        vecNormal.reserve(numberOfVertices);
        vecTexturePoint.reserve(numberOfVertices);
        for (int i = 0; i < vecPosition.size(); ++i)
        {
            Vector3d deticSurfaceNormal = ellipsoid.GeodeticSurfaceNormal(vecPosition[i]);
            
            //需要转换为halffloat吗？
            vecNormal.push_back(deticSurfaceNormal);
            
            vecTexturePoint.push_back(SubdivisionUtility::ComputeTextureCoordinate(deticSurfaceNormal));
        }
    }
    
    //计算索引数组
    int nIndiceSize = 3 * NumberOfTriangles(numberOfSlicePartitions, numberOfStackPartitions);
    vecVertexIndice.reserve(nIndiceSize);
    
    //
    // Triangle fan top row
    for (int j = 1; j < numberOfSlicePartitions; ++j)
    {
        vecVertexIndice.push_back(0);
        vecVertexIndice.push_back(j);
        vecVertexIndice.push_back(j+1);
    }
    vecVertexIndice.push_back(0);
    vecVertexIndice.push_back(numberOfSlicePartitions);
    vecVertexIndice.push_back(1);
    
    //
    // Middle rows are triangle strips
    //
    for (int i = 0; i < numberOfStackPartitions - 2; ++i)
    {
        int topRowOffset = (i * numberOfSlicePartitions) + 1;
        int bottomRowOffset = ((i + 1) * numberOfSlicePartitions) + 1;
        
        for (int j = 0; j < numberOfSlicePartitions - 1; ++j)
        {
            //左下三角形
            vecVertexIndice.push_back(bottomRowOffset + j);
            vecVertexIndice.push_back(bottomRowOffset + j + 1);
            vecVertexIndice.push_back(topRowOffset + j + 1);
            
            //右上三角形
            vecVertexIndice.push_back(bottomRowOffset + j);
            vecVertexIndice.push_back(topRowOffset + j + 1);
            vecVertexIndice.push_back(topRowOffset + j);
        }
        vecVertexIndice.push_back(bottomRowOffset + numberOfSlicePartitions - 1);
        vecVertexIndice.push_back(bottomRowOffset);
        vecVertexIndice.push_back(topRowOffset);
        
        vecVertexIndice.push_back(bottomRowOffset + numberOfSlicePartitions - 1);
        vecVertexIndice.push_back(topRowOffset);
        vecVertexIndice.push_back(topRowOffset + numberOfSlicePartitions - 1);
    }
    
    //
    // Triangle fan bottom row
    //
    unsigned int lastPosition = (unsigned int)vecPosition.size() - 1;
    for (unsigned int j = lastPosition - 1; j > lastPosition - numberOfSlicePartitions; --j)
    {
        vecVertexIndice.push_back(lastPosition);
        vecVertexIndice.push_back(j);
        vecVertexIndice.push_back(j-1);
    }
    vecVertexIndice.push_back(lastPosition);
    vecVertexIndice.push_back(lastPosition - numberOfSlicePartitions);
    vecVertexIndice.push_back(lastPosition - 1);
}


EARTH_CORE_NAMESPACE_END
