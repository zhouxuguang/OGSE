/*******************************************************************************
* ��Ȩ����(C) pyhcx 2012
* �ļ�����	: GeoLayer.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��10��6��
* ����ժҪ	: ͼ����
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOLAYER_H_D549FDB0_42B6_44D8_BAE3_7784FB71F3EE__
#define __GEOLAYER_H_D549FDB0_42B6_44D8_BAE3_7784FB71F3EE__

#include <stdlib.h>
#include <string.h>
#include "GeoDefine.h"

class GeoEnvelope;
class GeoSpatialReference;


//ͼ����ĳ�����
class GEOMETRY_API GeoLayer
{
	
public:
	GeoLayer(void);
	virtual ~GeoLayer(void);

	char* GetLayerName() const;
	void SetLayerName(char* pszLayerName);

	bool GetSelectable() const;
	void SetSelectable(bool& bSelectable);

	//virtual bool GetEnvelope(GeoEnvelope* ) = 0;
	//virtual void SetEnvelope(GeoEnvelope* m_pEnv) = 0;


	virtual GeoSpatialReference *GetSpatialRef() const { return NULL; }

private:
	char* m_pszLayerName;		//ͼ������,·��
	char* m_pszDataSource;	//����Դ

};


#endif // end of __GEOLAYER_H_D549FDB0_42B6_44D8_BAE3_7784FB71F3EE__
