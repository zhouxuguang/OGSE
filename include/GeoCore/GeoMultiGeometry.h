/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
* �ļ�����	: GeoMultiGeometry.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��7��26��
* ����ժҪ	: �༸����
* �޸ļ�¼	:
* ��    ��		��    ��		�޸���		�޸�ժҪ

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

	virtual GeoWkbGeometryType	GetGeometryType() const;  //��ȡ�����������
	virtual const char *		GetGeometryName() const;//��ȡ�����������

	virtual GeoGeometry *		Clone() const;			//��¡������

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

	virtual GeoWkbGeometryType	GetGeometryType() const;  //��ȡ�����������
	virtual const char *		GetGeometryName() const;//��ȡ�����������

	virtual GeoGeometry *		Clone() const;			//��¡������

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

	virtual GeoWkbGeometryType	GetGeometryType() const;  //��ȡ�����������
	virtual const char * GetGeometryName() const;			//��ȡ�����������

	virtual GeoGeometry *Clone() const;			//��¡������

	bool AddGeometry(GeoGeometry *poGeom);
};

#endif // end of __GEOMULTIGEOMETRY_H_6577BD95_F6BE_44F6_81DD_EBE8054B356D__
