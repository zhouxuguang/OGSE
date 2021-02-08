/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
* �ļ�����	: GeoGeometryFactory.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��6��3��
* ����ժҪ	: ��������󹤳���
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

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
