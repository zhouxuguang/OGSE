/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: Shape2Geometry.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��2��15��
@ ����ժҪ	: ����ͼ�ζ���͵ײ�shapefile�ļ��Ľ���
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/

#ifndef __SHAPE2GEOMETRY_H_C6CF923D_701E_4055_8536_1627064FEAA5__
#define __SHAPE2GEOMETRY_H_C6CF923D_701E_4055_8536_1627064FEAA5__

#include "shapefil.h"


class GeoGeometry;
class GeoPolygon;
class GeoPoint;
class GeoLineString;
class GeoFeatureDefn;
class GeoFeature;

#ifdef __cplusplus
extern "C"
{
#endif


/*******************************************************************************
* ��������	: *GeoReadSHPObject
* ��������	: 
* �Ρ�����	:  SHPHandle hSHP
* �Ρ�����	: int iShape
* �Ρ�����	: SHPObject *psShape
* �� �� ֵ	: GeoGeometry
* ��������	: �����
* �������	: 2013��11��17��
* �޸�����		   �޸���		  �޸�����
*******************************************************************************/
GeoGeometry *GeoReadSHPObject( SHPHandle hSHP, int iShape, SHPObject *psShape );


/*******************************************************************************
* ��������	: *GeoReadFeatureDefn
* ��������	: 
* �Ρ�����	:  const char * pszName
* �Ρ�����	: SHPHandle hSHP
* �Ρ�����	: DBFHandle hDBF
* �� �� ֵ	: GeoFeatureDefn
* ��������	: �����
* �������	: 2013��11��17��
* �޸�����		   �޸���		  �޸�����
*******************************************************************************/
GeoFeatureDefn *GeoReadFeatureDefn( const char * pszName,
									  SHPHandle hSHP, DBFHandle hDBF );

/*******************************************************************************
* ��������	: *GeoReadFeature
* ��������	: 
* �Ρ�����	:  SHPHandle hSHP
* �Ρ�����	: DBFHandle hDBF
* �Ρ�����	: GeoFeatureDefn * poDefn
* �Ρ�����	: int iShape
* �Ρ�����	: SHPObject *psShape
* �� �� ֵ	: GeoFeature
* ��������	: �����
* �������	: 2013��11��17��
* �޸�����		   �޸���		  �޸�����
*******************************************************************************/
GeoFeature *GeoReadFeature( SHPHandle hSHP, DBFHandle hDBF,
							  GeoFeatureDefn * poDefn, int iShape, 
							  SHPObject *psShape );

/*******************************************************************************
@ ��������	: GeoWriteSHPObject
@ ��������	: 
@ �Ρ�����	: SHPHandle hSHP
@ �Ρ�����	: int iShape
@ �Ρ�����	: GeoGeometry* poGeometry
@ �� �� ֵ	: bool
@ ��������	: guang
@ �������	: 2014��2��15��
@ �޸�����		   �޸���		  �޸�����
*******************************************************************************/
bool GeoWriteSHPObject(SHPHandle hSHP, int iShape, GeoGeometry* poGeometry);

/*******************************************************************************
@ ��������	: GeoWriteFeature
@ ��������	: 
@ �Ρ�����	: SHPHandle hSHP
@ �Ρ�����	: DBFHandle hDBF
@ �Ρ�����	: GeoFeatureDefn * poDefn
@ �Ρ�����	: int iShape
@ �Ρ�����	: GeoFeature* poFeature
@ �Ρ�����	: char* pszEncode
@ �� �� ֵ	: bool
@ ��������	: guang
@ �������	: 2014��2��15��
@ �޸�����		   �޸���		  �޸�����
*******************************************************************************/
bool GeoWriteFeature(SHPHandle hSHP,DBFHandle hDBF,GeoFeatureDefn * poDefn,
			GeoFeature* poFeature,char* pszEncode);

#ifdef __cplusplus
}
#endif


#endif // end of __SHAPE2GEOMETRY_H_C6CF923D_701E_4055_8536_1627064FEAA5__