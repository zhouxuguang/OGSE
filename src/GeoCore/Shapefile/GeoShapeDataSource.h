/*******************************************************************************
* ��Ȩ����(C) pyhcx 2012
* �ļ�����	: GeoShapeDataSource.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��8��24��
* ����ժҪ	: shapefile����Դ
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOSHAPEDATASOURCE_H_943A2524_F4EF_4AAE_9408_CA60D3E2555E__
#define __GEOSHAPEDATASOURCE_H_943A2524_F4EF_4AAE_9408_CA60D3E2555E__

/**********************************  ͷ�ļ� ************************************/
#include <vector>
#include <string>
#include "GeoDataSource.h"

class GeoShapeLayer;

class GeoShapeDataSource : public GeoDataSource
{
public:
	GeoShapeDataSource(void);
	~GeoShapeDataSource(void);

	//���ļ�
	virtual int Open( const char * pszFilename, int bUpdate = 0);

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
	void  AddLayer(GeoShapeLayer* poLayer);

private:
	GeoShapeLayer     **m_papoLayers;		//shapeͼ��
	int m_nLayers;						//ͼ����Ŀ
	
	char* m_pszName;						//����Դ����
	std::vector<std::string> m_vecLayerName;	//����ͼ�������

};

#endif // end of __GEOSHAPEDATASOURCE_H_943A2524_F4EF_4AAE_9408_CA60D3E2555E__
