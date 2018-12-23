//
//  GeoGridTessellator.cpp
//  earthEngineCore
//
//  Created by Zhou,Xuguang on 2018/12/23.
//  Copyright © 2018年 Zhou,Xuguang. All rights reserved.
//

#include "GeoGridTessellator.h"

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
                                std::vector<math3d::Vector2> &vecTexturePoint)
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
    
//    Mesh mesh = new Mesh();
//    mesh.PrimitiveType = PrimitiveType.Triangles;
//    mesh.FrontFaceWindingOrder = WindingOrder.Counterclockwise;
//
//    int numberOfVertices = NumberOfVertices(numberOfSlicePartitions, numberOfStackPartitions);
//    VertexAttributeDoubleVector3 positionsAttribute = new VertexAttributeDoubleVector3("position", numberOfVertices);
//    mesh.Attributes.Add(positionsAttribute);
//
//    IndicesUnsignedInt indices = new IndicesUnsignedInt(3 * NumberOfTriangles(numberOfSlicePartitions, numberOfStackPartitions));
//    mesh.Indices = indices;
//
//    IList<Vector3H> normals = null;
//    if ((vertexAttributes & GeographicGridEllipsoidVertexAttributes.Normal) == GeographicGridEllipsoidVertexAttributes.Normal)
//    {
//        VertexAttributeHalfFloatVector3 normalsAttribute = new VertexAttributeHalfFloatVector3("normal", numberOfVertices);
//        mesh.Attributes.Add(normalsAttribute);
//        normals = normalsAttribute.Values;
//    }
//
//    IList<Vector2H> textureCoordinates = null;
//    if ((vertexAttributes & GeographicGridEllipsoidVertexAttributes.TextureCoordinate) == GeographicGridEllipsoidVertexAttributes.TextureCoordinate)
//    {
//        VertexAttributeHalfFloatVector2 textureCoordinateAttribute = new VertexAttributeHalfFloatVector2("textureCoordinate", numberOfVertices);
//        mesh.Attributes.Add(textureCoordinateAttribute);
//        textureCoordinates = textureCoordinateAttribute.Values;
//    }
    
    //
    // Create lookup table
    //
    double *cosTheta = new double[numberOfSlicePartitions];
    double *sinTheta = new double[numberOfSlicePartitions];
    
    for (int j = 0; j < numberOfSlicePartitions; ++j)
    {
        double theta = (M_PI * 2.0) * (((double)j) / numberOfSlicePartitions);
        cosTheta[j] = cos(theta);
        sinTheta[j] = sin(theta);
    }
    
    //
    // Create positions
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
            vecPosition.emplace_back(cosTheta[j] * xSinPhi, sinTheta[j] * ySinPhi, zCosPhi);
        }
    }
    vecPosition.emplace_back(0, 0, -ellipsoid.GetAxis().z);
    
    //计算法向量和纹理坐标
//    if ((normals != null) || (textureCoordinates != null))
//    {
//        for (int i = 0; i < positions.Count; ++i)
//        {
//            Vector3D deticSurfaceNormal = ellipsoid.GeodeticSurfaceNormal(positions[i]);
//
//            if (normals != null)
//            {
//                normals.Add(deticSurfaceNormal.ToVector3H());
//            }
//
//            if (textureCoordinates != null)
//            {
//                textureCoordinates.Add(SubdivisionUtility.ComputeTextureCoordinate(deticSurfaceNormal));
//            }
//        }
//    }
    
    //
    // Triangle fan top row
    //
//    for (int j = 1; j < numberOfSlicePartitions; ++j)
//    {
//        indices.AddTriangle(new TriangleIndicesUnsignedInt(0, j, j + 1));
//    }
//    indices.AddTriangle(new TriangleIndicesUnsignedInt(0, numberOfSlicePartitions, 1));
    
    //
    // Middle rows are triangle strips
    //
//    for (int i = 0; i < numberOfStackPartitions - 2; ++i)
//    {
//        int topRowOffset = (i * numberOfSlicePartitions) + 1;
//        int bottomRowOffset = ((i + 1) * numberOfSlicePartitions) + 1;
//
//        for (int j = 0; j < numberOfSlicePartitions - 1; ++j)
//        {
//            indices.AddTriangle(new TriangleIndicesUnsignedInt(bottomRowOffset + j, bottomRowOffset + j + 1, topRowOffset + j + 1));
//            indices.AddTriangle(new TriangleIndicesUnsignedInt(bottomRowOffset + j, topRowOffset + j + 1, topRowOffset + j));
//        }
//        indices.AddTriangle(new TriangleIndicesUnsignedInt(bottomRowOffset + numberOfSlicePartitions - 1, bottomRowOffset, topRowOffset));
//        indices.AddTriangle(new TriangleIndicesUnsignedInt(bottomRowOffset + numberOfSlicePartitions - 1, topRowOffset, topRowOffset + numberOfSlicePartitions - 1));
//    }
    
    //
    // Triangle fan bottom row
    //
//    int lastPosition = positions.Count - 1;
//    for (int j = lastPosition - 1; j > lastPosition - numberOfSlicePartitions; --j)
//    {
//        indices.AddTriangle(new TriangleIndicesUnsignedInt(lastPosition, j, j - 1));
//    }
//    indices.AddTriangle(new TriangleIndicesUnsignedInt(lastPosition, lastPosition - numberOfSlicePartitions, lastPosition - 1));
//
//    return mesh;
}


EARTH_CORE_NAMESPACE_END
