/*******************************************************************************
* 版权所有(C) pyhcx 2012
* 文件名称	: GeoGeometry.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年6月3日
* 内容摘要	: 几何体抽象类
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOGEOMETRY_H_DF951436_3D5E_452D_9739_693086E94D5A__
#define __GEOGEOMETRY_H_DF951436_3D5E_452D_9739_693086E94D5A__


#include <string>
#include "GeoDefine.h"


class GeoPoint;
class GeoEnvelope;
class GeoSpatialReference;

class GEOMETRY_API GeoGeometry
{
public:
	GeoGeometry(void);
	virtual ~GeoGeometry(void);

	/*标准接口*/
	virtual int GetDimension() const = 0;   //几何体的维数
	virtual bool Empty() = 0;	  //清空几何体
	virtual GeoWkbGeometryType GetGeometryType() const = 0;  //获取几何体的类型
	virtual const char *GetGeometryName() const = 0;//获取几何体的名字
	virtual void GetEnvelope(GeoEnvelope *env) const = 0; //获取几何体的最小外包矩形
	virtual void Move(double dx,double dy) = 0;			//移动几何体
	virtual double GetArea() const = 0;							//计算面积
	virtual double GetLength() const = 0;							//计算长度
	//virtual void ToKML(std::string &strKML) = 0;							//转换为KML

	//克隆函数
	virtual GeoGeometry *Clone() const = 0;

	//OGC格式交换
	virtual int WkbByteSize() const = 0;
	virtual std::string AsText() = 0;							//转换为WKT
	virtual int AsBinary( GeoByteOrder eByteOrder,unsigned char *poData ) const
	{
		return 1;
	}

	void SetSpatialReference( GeoSpatialReference *poSR );
	GeoSpatialReference *GetSpatialReference( ) const;

	// 空间关系
	virtual bool  Intersects( GeoGeometry * ) const;
	virtual bool  Equals( GeoGeometry * ) const;
	virtual bool  Disjoint( const GeoGeometry * ) const;
	virtual bool  Touches( const GeoGeometry * ) const;
	virtual bool  Crosses( const GeoGeometry * ) const;
	virtual bool  Within( const GeoGeometry * ) const;
	virtual bool  Contains( const GeoGeometry * ) const;
	virtual bool  Overlaps( const GeoGeometry * ) const;
	virtual bool  Relate( const GeoGeometry *, const char * ) const;

	//空间分析
	virtual GeoGeometry *Boundary() const;
	virtual double  Distance( const GeoGeometry * ) const;
	virtual GeoGeometry *ConvexHull() const;
	virtual GeoGeometry *Buffer( double dfDist, int nQuadSegs = 30 ) const;
	virtual GeoGeometry *Intersection( const GeoGeometry *) const;
	virtual GeoGeometry *Union( const GeoGeometry * ) const;
	virtual GeoGeometry *UnionCascaded() const;
	virtual GeoGeometry *Difference( const GeoGeometry * ) const;
	virtual GeoGeometry *SymDifference( const GeoGeometry * ) const;
	virtual int       Centroid( GeoPoint * poPoint ) const;
	virtual GeoGeometry *Simplify(double dTolerance) const;
	GeoGeometry *SimplifyPreserveTopology(double dTolerance) const;

	virtual GeoGeometry *Polygonize() const;

	// backward compatibility to non-standard method names. 
	bool  Intersect( GeoGeometry * ) const;
	bool  Equal( GeoGeometry * ) const;
	virtual GeoGeometry *SymmetricDifference( const GeoGeometry * ) const;
	virtual GeoGeometry *getBoundary() const;   


private:
	GeoSpatialReference * m_poSRS;                // 空间参考系统，可能为空

protected:
	int m_nCoordDimension;				//坐标的维数
	
};


#endif // end of __GEOGEOMETRY_H_DF951436_3D5E_452D_9739_693086E94D5A__
