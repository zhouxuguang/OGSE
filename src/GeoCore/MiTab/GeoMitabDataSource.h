/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: GeoMitabDataSource.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��11��30��
@ ����ժҪ	: mapinfo����Դ
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOMITABDATASOURCE_H_BF43EBCC_F3E5_42E9_A0DB_52FEB2BDEC5F__
#define __GEOMITABDATASOURCE_H_BF43EBCC_F3E5_42E9_A0DB_52FEB2BDEC5F__

#include <vector>
#include <string>
#include "GeoDataSource.h"

class GeoMitabLayer;

class GeoMitabDataSource : public GeoDataSource
{
public:
	GeoMitabDataSource(void);
	~GeoMitabDataSource(void);

	//���ļ�
	int Open( const char * pszFilename, int bUpdate = 0);

	//�������Դ����
	virtual const char *GetName() { return m_pszName; };

	//���ͼ�����
	virtual int GetLayerCount();

	//�����������ͼ��
	virtual GeoLayer *GetLayer(int);

	//�������ֻ��ͼ��
	virtual GeoLayer *GetLayerByName(const char *);

	//����һ���µ�ͼ��
	virtual GeoLayer *CreateGeoLayer( const char *pszName, 
		GeoSpatialReference *poSpatialRef = NULL,
		GeoWkbGeometryType eGType = gWkbUnKnow );

private:
	void  AddLayer(GeoMitabLayer* poLayer);

private:
	GeoMitabLayer     **m_papoLayers;		//mapinfoͼ��
	int m_nLayers;						//ͼ����Ŀ

	char* m_pszName;						//����Դ����
	std::vector<std::string> m_vecLayerName;	//����ͼ�������
};

#endif // end of __GEOMITABDATASOURCE_H_BF43EBCC_F3E5_42E9_A0DB_52FEB2BDEC5F__
