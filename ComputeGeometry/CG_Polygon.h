/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: CG_Polygon.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年11月4日
@ 内容摘要	: 多边形算法
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __CG_POLYGON_H_BF9B7E8F_69F3_44C4_8CB5_FD344EA626DD__
#define __CG_POLYGON_H_BF9B7E8F_69F3_44C4_8CB5_FD344EA626DD__

#ifdef  __cplusplus
extern "C" {
#endif

/*******************************************************************************
@ 函数名称	: IsIntersectAnt
@ 功能描述	: 向Y轴作垂线，并指向左边
@ 参　　数	: double x
@ 参　　数	: double y
@ 参　　数	: double X1
@ 参　　数	: double Y1
@ 参　　数	: double X2
@ 参　　数	: double Y2
@ 返 回 值	: int
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
int IsIntersectAnt(double x,double y,double X1,double Y1,double X2,double Y2);


/*******************************************************************************
@ 函数名称	: PointInPolygon
@ 功能描述	: 判断点与多边形关系（简单多边形）
@ 参　　数	: const SPOINT& poPoint
@ 参　　数	: const SPOINT* lpPoints
@ 参　　数	: int nCnt
@ 参　　数	: int &PtCount
@ 返 回 值	: int 
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API int PointInPolygon(const SPOINT& poPoint,const SPOINT* lpPoints,int nCnt,int &PtCount);


/*******************************************************************************
@ 函数名称	: PointInPolyPolygon
@ 功能描述	: 判断点与多边形关系（带岛多边形）
@ 参　　数	: const SPOINT& poPoint
@ 参　　数	: const SPOINT* lpPoints
@ 参　　数	: const int *lpPolyCounts
@ 参　　数	: int nCount
@ 返 回 值	: int
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API int PointInPolyPolygon(const SPOINT& poPoint,const SPOINT* lpPoints,
										   const int *lpPolyCounts,int nCount);


/*******************************************************************************
@ 函数名称	: PolyCentroid
@ 功能描述	: 计算多边形中心点
@ 参　　数	: const SPOINT* points
@ 参　　数	: int nCount
@ 参　　数	: double *xCentroid 中心点x坐标
@ 参　　数	: double *yCentroid 中心点y坐标
@ 返 回 值	: void
@ 作　　者	: guang
@ 设计日期	: 2013年11月4日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
COMPUTEGEOMETRY_API void PolyCentroid(const SPOINT* points,int nCount,double *xCentroid,double *yCentroid);

//多边形是否自相交
COMPUTEGEOMETRY_API int IsPolygonSelfIntersect(const SPOINT* points,int nCount);


#ifdef  __cplusplus
}
#endif


#endif // end of __CG_POLYGON_H_BF9B7E8F_69F3_44C4_8CB5_FD344EA626DD__
