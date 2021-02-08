/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2014
@ 文件名称	: GeoCoordinateTransformation.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2012年2月10日
@ 内容摘要	: 空间参考系统转换的接口
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOCOORDINATETRANSFORMATION_H_FD2A029E_08EC_47FB_8381_30B7C992862E__
#define __GEOCOORDINATETRANSFORMATION_H_FD2A029E_08EC_47FB_8381_30B7C992862E__

#include <stdlib.h>
#include "GeoDefine.h"

class GeoSpatialReference;

class GEOMETRY_API GeoCoordinateTransformation
{
public:
	virtual ~GeoCoordinateTransformation(void){}

	static void DestroyCT(GeoCoordinateTransformation* poCT);

    virtual GeoSpatialReference *GetSourceSRS() = 0;

    virtual GeoSpatialReference *GetTargetSRS() = 0;

    virtual int Transform( int nCount, 
                           double *padX, double *padY, double *padZ = NULL ) = 0;

    virtual int TransformEx( int nCount, 
                             double *padX, double *padY, double *padZ = NULL,
                             int *pabSuccess = NULL ) = 0;
};

GeoCoordinateTransformation GEOMETRY_API *
GeoCreateCoordinateTransformation( GeoSpatialReference *poSource, 
								  GeoSpatialReference *poTarget );

#endif // end of __GEOCOORDINATETRANSFORMATION_H_0B839B58_4209_4052_8E4D_98A28C91DD2C__
