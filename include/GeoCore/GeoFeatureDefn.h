/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
* �ļ�����	: GeoFeatureDefn.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��10��4��
* ����ժҪ	: Ҫ�صĶ���
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOFEATUREDEFN_H_E204F6E1_B0AA_41F1_9E2C_ACA565197FB8__
#define __GEOFEATUREDEFN_H_E204F6E1_B0AA_41F1_9E2C_ACA565197FB8__

#include "GeoDefine.h"

class GeoGeometry;
class GeoFieldDefn;

class GEOMETRY_API GeoFeatureDefn
{
private:
	int         m_nFieldCount;				//�ֶθ���
	GeoFieldDefn **m_papoFieldDefn;			//�ֶ��б�
	GeoWkbGeometryType m_eGeomType;			//����������
	char        *m_pszFeatureClassName;		//Ҫ��������

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
