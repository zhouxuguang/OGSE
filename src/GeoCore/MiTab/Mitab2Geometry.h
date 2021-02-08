/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: Mitab2Geometry.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年11月30日
@ 内容摘要	: mapinfo数据转换为几何对象
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

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


