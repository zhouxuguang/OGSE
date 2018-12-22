/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoFeatureDefn.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年10月4日
* 内容摘要	: 要素的定义
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOFEATUREDEFN_H_E204F6E1_B0AA_41F1_9E2C_ACA565197FB8__
#define __GEOFEATUREDEFN_H_E204F6E1_B0AA_41F1_9E2C_ACA565197FB8__

#include "GeoDefine.h"

class GeoGeometry;
class GeoFieldDefn;

class GEOMETRY_API GeoFeatureDefn
{
private:
	int         m_nFieldCount;				//字段个数
	GeoFieldDefn **m_papoFieldDefn;			//字段列表
	GeoWkbGeometryType m_eGeomType;			//几何体类型
	char        *m_pszFeatureClassName;		//要素类名字

	bool         m_bIgnoreGeometry;
	bool         m_bIgnoreStyle;

public:
	explicit GeoFeatureDefn( const char * pszName );
	virtual ~GeoFeatureDefn(void);

	const char* GetName() { return m_pszFeatureClassName; }

	GeoFeatureDefn* Clone() const;

	int GetFieldCount()	{ return m_nFieldCount; }
	GeoFieldDefn * GetFieldDefn(int nIndex) const;
	int GetFieldIndex( const char * ) const;

	void AddFieldDefn( GeoFieldDefn * );

	GeoWkbGeometryType GetGeomType() { return m_eGeomType; }
	void SetGeomType( GeoWkbGeometryType );

	bool IsGeometryIgnored() { return m_bIgnoreGeometry; }
	void SetGeometryIgnored( bool bIgnore ) { m_bIgnoreGeometry = bIgnore; }
	bool IsStyleIgnored() { return m_bIgnoreStyle; }
	void SetStyleIgnored( bool bIgnore ) { m_bIgnoreStyle = bIgnore; }
};

#endif // end of __GEOFEATUREDEFN_H_E204F6E1_B0AA_41F1_9E2C_ACA565197FB8__
