/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoGeometryFactory.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年6月3日
* 内容摘要	: 几何体抽象工厂类
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/

#ifndef __GEOGEOMETRYFACTORY_H_DF951436_3D5E_452D_9739_693086E94D5A__
#define __GEOGEOMETRYFACTORY_H_DF951436_3D5E_452D_9739_693086E94D5A__

#include "GeoDefine.h"

class GeoGeometry;
class GeoSpatialReference;

class GEOMETRY_API GeoGeometryFactory
{
public:

	static int CreateFromWkb( unsigned char *pabData, GeoSpatialReference *pSrs,
		GeoGeometry **ppoGeometry, int = -1 );
	static int CreateFromWkt( char **ppszWkt, GeoSpatialReference *pSrs,
		GeoGeometry ** ppoGeometry);
	static GeoGeometry *CreateFromGML( const char * ppszGml);

	static void   DestroyGeometry( GeoGeometry * );
	static GeoGeometry *CreateGeometry( const char* pszGeomName );
	static GeoGeometry *CreateGeometry(GeoWkbGeometryType eGeomType);

private:
	GeoGeometryFactory(void);
	~GeoGeometryFactory(void);
};

#endif // end of __GEOGEOMETRY_H_DF951436_3D5E_452D_9739_693086E94D5A__
