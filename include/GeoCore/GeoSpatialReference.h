/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
* �ļ�����	: GeoSpatialReference.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang126@126.com)
* �������	: 2012��6��9��
* ����ժҪ	: �ռ�ο�ϵͳ(������������ϵͳ�Լ�ͶӰ����ϵͳ��)
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOSPATIALREFERENCE_H_EC4670EE_AA58_4D83_B700_573180A066B9__
#define __GEOSPATIALREFERENCE_H_EC4670EE_AA58_4D83_B700_573180A066B9__

#include <stdlib.h>
#include <string>
#include "GeoDefine.h"


class GEOMETRY_API GeoSpatialReference
{
public:
	GeoSpatialReference();
	virtual ~GeoSpatialReference();

	GeoSpatialReference& operator=(const GeoSpatialReference& sref);

	//����һ������ϵ
	static void DestroySpatialReference(GeoSpatialReference* poSRS);

	//�������ϵ������
	std::string GetName() const;

	//��¡һ������ϵͳ
	GeoSpatialReference* Clone() const;

	//��¡һ����������ϵͳ
	GeoSpatialReference* CloneGeogCS() const;

	void Release();

	bool IsValid() const;

	//����ΪWKT
	bool ExportToWkt( std::string& strWkt ) const;

	//����Ϊ��ʽ����WKT
	bool ExportToPrettyWkt( std::string &strWkt) const;

	//��WKT����
	bool ImportFromWkt( const std::string& strWkt );

	//������Proj4
	bool ExportToProj4(std::string &strProj4) const;

	//��proj4�ַ�������
	bool ImportFromProj4(const std::string &strProj4);

	//����EPSG�ı�ŵ���
	bool ImportFromEPSG(int nEpsgCode);

	//ת��ΪESRI���Ŀռ�ο�ϵ
	bool MorphToESRI();


	//�Ƿ�Ϊ��������ϵ
	bool IsGeographic() const;

	//�Ƿ�ΪͶӰ����ϵ
	bool IsProjected() const;

	//�Ƿ�Ϊ������ά����ϵ
	bool IsGeocentric() const;

	//�Ƿ�Ϊ��ֱ����ϵ
	bool IsVertical() const;

	//�Ƿ�Ϊ�ֲ�����ϵ��CAD�Ƚϳ���
	bool IsLocal() const;

	//�Ƿ�Ϊ��������ϵ������ϸ߳�����ϵ����ֱ����ϵ��
	bool IsCompound() const;

	//�Ƿ���ͬһ��ͶӰ
	bool IsSame(const GeoSpatialReference &cSrs) const;

	bool IsSame(const GeoSpatialReference *pSrs) const;

	//�Ƿ���ͬһ����������ϵ
	bool IsSameGeogCS(const GeoSpatialReference &cSrs) const;

	bool IsSameGeogCS(const GeoSpatialReference *pSrs) const;


	//��ýڵ������ֵ
	std::string GetAttrValue( std::string strName, int iChild  = 0  ) const;

	//������Ե�λ�ĳ���
	double GetLinearUnits(char** pszName = NULL ) const;

	//��ýǶȵ�λ
	double GetAngularUnits(char** pszName = NULL ) const;

	//��ñ���������
	double GetPrimeMeridian(char ** pszOption) const;

	//��ó�����뾶
	double GetSemiMajor( int *pErr /* = NULL */) const;

	//��ö̰���뾶
	double GetSemiMinor( int *pErr /* = NULL */) const;

	//���ƫ���ʵ���
	double GetInvFlattening( int *pErr /*=NULL*/) const;

	//����ת��
	bool Transform(const GeoSpatialReference &cSrsTarget, int nCount, double *x, double *y, double *z = NULL) const;

	bool Transform(const GeoSpatialReference *pSrsTarget, int nCount, double *x, double *y, double *z = NULL) const;

private:

	void *m_hSRS;
};

#endif // end of __GEOSPATIALREFERENCE_H_EC4670EE_AA58_4D83_B700_573180A066B9__
