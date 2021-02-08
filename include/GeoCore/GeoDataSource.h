/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
* �ļ�����	: GeoDataSource.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��10��7��
* ����ժҪ	: ����Դ
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEODATASOURCE_H_F0FECA57_C4E4_4999_A8D4_7D4C5233D71E__
#define __GEODATASOURCE_H_F0FECA57_C4E4_4999_A8D4_7D4C5233D71E__

/**********************************  ͷ�ļ� ************************************/

#include <stdio.h>
#include "GeoDefine.h"

class GeoLayer;
class GeoSpatialReference;
class GeoDataDriver;


class GEOMETRY_API GeoDataSource
{
public:
	GeoDataSource(void);
	virtual ~GeoDataSource(void);

	void        *m_hMutex;

	virtual int Open( const char * pszFilename, int bUpdate = 0) = 0;

	//��������Դ
	static void DestroyGeoDataSource( GeoDataSource * );

	//�������Դ����
	virtual const char *GetName() = 0;

	//���ͼ�����
	virtual int GetLayerCount() = 0;

	//�����������ͼ��
	virtual GeoLayer *GetLayer(int) = 0;
	
	//�������ֻ��ͼ��
	virtual GeoLayer *GetLayerByName(const char *);

	//ɾ��ͼ��
	//virtual bool      DeleteLayer(int);

	//����һ���µ�ͼ��
	virtual GeoLayer *CreateGeoLayer( const char *pszName, 
		GeoSpatialReference *poSpatialRef = NULL,
		GeoWkbGeometryType eGType = gWkbUnKnow );

	GeoDataDriver *GetDriver() const;
	void SetDriver( GeoDataDriver *poDriver );

private:

	GeoDataDriver        *m_poDriver;		//��������
};


//ע����������
GEOMETRY_API void GEOMETRY_API_CALL RegisterShapefile();

GEOMETRY_API void GEOMETRY_API_CALL RegisterMapinfo();

#endif // end of __GEODATASOURCE_H_F0FECA57_C4E4_4999_A8D4_7D4C5233D71E__
