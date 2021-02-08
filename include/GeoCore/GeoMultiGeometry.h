/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoMultiGeometry.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年7月26日
* 内容摘要	: 多几何体
* 修改记录	:
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOMULTIGEOMETRY_H_6577BD95_F6BE_44F6_81DD_EBE8054B356D__
#define __GEOMULTIGEOMETRY_H_6577BD95_F6BE_44F6_81DD_EBE8054B356D__

#include "GeoGeometryCollection.h"

class GeoGeometry;
class GeoGeometryCollection;


/************************************************************************/
/*                            GeoMultiPoint                             */
/************************************************************************/
class GEOMETRY_API GeoMultiPoint : public GeoGeometryCollection
{
public:
								GeoMultiPoint(void);
								virtual ~GeoMultiPoint(void);

	virtual GeoWkbGeometryType	GetGeometryType() const;  //获取几何体的类型
	virtual const char *		GetGeometryName() const;//获取几何体的名字

	virtual GeoGeometry *		Clone() const;			//克隆几何体

	bool						AddGeometry(GeoGeometry *poGeom);
};

/************************************************************************/
/*                          GeoMultiLineString                          */
/************************************************************************/

class GEOMETRY_API GeoMultiLineString : public GeoGeometryCollection
{
public:
	GeoMultiLineString(void);
	virtual ~GeoMultiLineString(void);

	virtual GeoWkbGeometryType	GetGeometryType() const;  //获取几何体的类型
	virtual const char *		GetGeometryName() const;//获取几何体的名字

	virtual GeoGeometry *		Clone() const;			//克隆几何体

	bool						AddGeometry(GeoGeometry *poGeom);
};

/************************************************************************/
/*                           GeoMultiPolygon                            */
/************************************************************************/

class GEOMETRY_API GeoMultiPolygon : public GeoGeometryCollection
{
public:
	GeoMultiPolygon(void);
	virtual ~GeoMultiPolygon(void);

	virtual GeoWkbGeometryType	GetGeometryType() const;  //获取几何体的类型
	virtual const char * GetGeometryName() const;			//获取几何体的名字

	virtual GeoGeometry *Clone() const;			//克隆几何体

	bool AddGeometry(GeoGeometry *poGeom);
};

#endif // end of __GEOMULTIGEOMETRY_H_6577BD95_F6BE_44F6_81DD_EBE8054B356D__
