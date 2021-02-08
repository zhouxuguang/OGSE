/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
* �ļ�����	: GeoFieldDefn.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��6��3��
* ����ժҪ	:
* �޸ļ�¼	:
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOFIELDDEFN_H_1A40E847_C8E7_4243_99E4_0B0E7E474D2E__
#define __GEOFIELDDEFN_H_1A40E847_C8E7_4243_99E4_0B0E7E474D2E__

/**********************************  ͷ�ļ� ************************************/

#include <algorithm>
#if defined _WIN32 || defined _WIN64
	#include <xutility>
#endif
#include "GeoDefine.h"

class GEOMETRY_API GeoFieldDefn
{
private:
	char					*pszName;			   /*�ֶ���*/
	GeoFieldType	        eType;                 /*�ֶ�����*/
	GeoJustification	    eJustify;              /*���뷽ʽ*/
	int						nWidth;                /*�ֶο�� */
	int						nPrecision;			   /*�ֶξ���*/
	GeoField				uDefault;			   /*�ֶ�ֵ*/

	int						bIgnore;			   /*�Ƿ񱻺���*/
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
