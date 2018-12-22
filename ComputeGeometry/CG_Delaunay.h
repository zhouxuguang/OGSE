/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2013
* 文件名称	: CG_Delaunay.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2013年10月26日
* 内容摘要	: DELAUNAY三角网算法
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __CG_DELAUNAY_H_B667D47E_F4E2_4DE6_8F7F_B3C138F473D6__
#define __CG_DELAUNAY_H_B667D47E_F4E2_4DE6_8F7F_B3C138F473D6__

#include "CG_Common.h"

typedef struct D_Triangle D_Triangle;

//DELAUNAY三角网的顶点结构
struct D_Vertex 
{
	int vertexID;
	double x;
	double y;
	double z;
};

//DELAUNAY三角网的边结构
struct D_Edge
{
	int edgeID;		//
	D_Vertex* pBeginVertex;		//开始点
	D_Vertex* pEndVertex;		//结束点
	D_Triangle* pLTria;			//所属的三角形,只属于它左边的或右边的
	D_Edge* pREdge;				//所对应的反向边
};

//DELAUNAY三角网的三角形结构
struct D_Triangle
{
	int triangleID;
	D_Edge* ppEdge[3];		//三条边
};
COMPUTEGEOMETRY_API bool CreateDelaunayNet(const CG_Point* pPoints,int nCount);

#endif // end of __CG_DELAUNAY_H_B667D47E_F4E2_4DE6_8F7F_B3C138F473D6__

