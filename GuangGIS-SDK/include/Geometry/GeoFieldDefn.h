/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoFieldDefn.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年6月3日
* 内容摘要	:
* 修改记录	:
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOFIELDDEFN_H_1A40E847_C8E7_4243_99E4_0B0E7E474D2E__
#define __GEOFIELDDEFN_H_1A40E847_C8E7_4243_99E4_0B0E7E474D2E__

/**********************************  头文件 ************************************/

#include <algorithm>
#if defined _WIN32 || defined _WIN64
	#include <xutility>
#endif
#include "GeoDefine.h"

class GEOMETRY_API GeoFieldDefn
{
private:
	char					*pszName;			   /*字段名*/
	GeoFieldType	        eType;                 /*字段类型*/
	GeoJustification	    eJustify;              /*对齐方式*/
	int						nWidth;                /*字段宽度 */
	int						nPrecision;			   /*字段精度*/
	GeoField				uDefault;			   /*字段值*/

	int						bIgnore;			   /*是否被忽略*/
public:
						GeoFieldDefn(void);
						GeoFieldDefn( GeoFieldDefn * );
						~GeoFieldDefn(void);

	void				SetName(const char* pszNameIn);
	const char*			GetName() { return pszName; }

	GeoFieldType		GetType() { return eType; }
	void				SetType( GeoFieldType eTypeIn ) { eType = eTypeIn; }
	static const char*	GetFieldTypeName( GeoFieldType eTypeIn );

	GeoJustification	GetJustify() { return eJustify; }
	void				SetJustify( GeoJustification eJustifyIn )
	{ eJustify = eJustifyIn; }

	int                 GetWidth() { return nWidth; }
	void                SetWidth( int nWidthIn ) { nWidth = max(0,nWidthIn); }

	int                 GetPrecision() { return nPrecision; }
	void                SetPrecision( int nPrecisionIn )
	{ nPrecision = nPrecisionIn; }

	void                SetDefault( const GeoField * );
	const GeoField	    *GetDefaultRef() { return &uDefault; }

	int                 IsIgnored() { return bIgnore; }
	void                SetIgnored( int bIgnore ) { this->bIgnore = bIgnore; }

	void                Set( const char *, GeoFieldType, int = 0, int = 0,
						GeoJustification = gWkbUndefined );
};

#endif // end of __GEOFIELDDEFN_H_1A40E847_C8E7_4243_99E4_0B0E7E474D2E__
