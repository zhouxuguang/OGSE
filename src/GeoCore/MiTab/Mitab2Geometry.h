/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: Mitab2Geometry.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��11��30��
@ ����ժҪ	: mapinfo����ת��Ϊ���ζ���
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __MITAB2GEOMETRY_H_9B5CFB22_225B_4D2C_85DD_63AC625776EC__
#define __MITAB2GEOMETRY_H_9B5CFB22_225B_4D2C_85DD_63AC625776EC__

#include "mitab_capi.h"

class GeoFeature;
class GeoGeometry;
class GeoFeatureDefn;

#ifdef __cplusplus
extern "C"
{
#endif

	GeoGeometry *GeoReadMitabObject(/* mitab_handle hMitabHandle, int iShape, */mitab_feature psShape );

	GeoFeature *GeoReadMitabFeature( mitab_handle hMitabHandle, int iShape,GeoFeatureDefn * poDefn);

	GeoFeatureDefn *GeoReadMitabFeatureDefn(const char * pszName,mitab_handle hMitabHandle);

#ifdef __cplusplus
}
#endif

#endif // end of __MITAB2GEOMETRY_H_9B5CFB22_225B_4D2C_85DD_63AC625776EC__


