/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: Shape2Geometry.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年2月15日
@ 内容摘要	: 几何图形对象和底层shapefile文件的交互
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

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
* 函数名称	: *GeoReadSHPObject
* 功能描述	: 
* 参　　数	:  SHPHandle hSHP
* 参　　数	: int iShape
* 参　　数	: SHPObject *psShape
* 返 回 值	: GeoGeometry
* 作　　者	: 周旭光
* 设计日期	: 2013年11月17日
* 修改日期		   修改人		  修改内容
*******************************************************************************/
GeoGeometry *GeoReadSHPObject( SHPHandle hSHP, int iShape, SHPObject *psShape );


/*******************************************************************************
* 函数名称	: *GeoReadFeatureDefn
* 功能描述	: 
* 参　　数	:  const char * pszName
* 参　　数	: SHPHandle hSHP
* 参　　数	: DBFHandle hDBF
* 返 回 值	: GeoFeatureDefn
* 作　　者	: 周旭光
* 设计日期	: 2013年11月17日
* 修改日期		   修改人		  修改内容
*******************************************************************************/
GeoFeatureDefn *GeoReadFeatureDefn( const char * pszName,
									  SHPHandle hSHP, DBFHandle hDBF );

/*******************************************************************************
* 函数名称	: *GeoReadFeature
* 功能描述	: 
* 参　　数	:  SHPHandle hSHP
* 参　　数	: DBFHandle hDBF
* 参　　数	: GeoFeatureDefn * poDefn
* 参　　数	: int iShape
* 参　　数	: SHPObject *psShape
* 返 回 值	: GeoFeature
* 作　　者	: 周旭光
* 设计日期	: 2013年11月17日
* 修改日期		   修改人		  修改内容
*******************************************************************************/
GeoFeature *GeoReadFeature( SHPHandle hSHP, DBFHandle hDBF,
							  GeoFeatureDefn * poDefn, int iShape, 
							  SHPObject *psShape );

/*******************************************************************************
@ 函数名称	: GeoWriteSHPObject
@ 功能描述	: 
@ 参　　数	: SHPHandle hSHP
@ 参　　数	: int iShape
@ 参　　数	: GeoGeometry* poGeometry
@ 返 回 值	: bool
@ 作　　者	: guang
@ 设计日期	: 2014年2月15日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
bool GeoWriteSHPObject(SHPHandle hSHP, int iShape, GeoGeometry* poGeometry);

/*******************************************************************************
@ 函数名称	: GeoWriteFeature
@ 功能描述	: 
@ 参　　数	: SHPHandle hSHP
@ 参　　数	: DBFHandle hDBF
@ 参　　数	: GeoFeatureDefn * poDefn
@ 参　　数	: int iShape
@ 参　　数	: GeoFeature* poFeature
@ 参　　数	: char* pszEncode
@ 返 回 值	: bool
@ 作　　者	: guang
@ 设计日期	: 2014年2月15日
@ 修改日期		   修改人		  修改内容
*******************************************************************************/
bool GeoWriteFeature(SHPHandle hSHP,DBFHandle hDBF,GeoFeatureDefn * poDefn,
			GeoFeature* poFeature,char* pszEncode);

#ifdef __cplusplus
}
#endif


#endif // end of __SHAPE2GEOMETRY_H_C6CF923D_701E_4055_8536_1627064FEAA5__