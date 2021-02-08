/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: GeoVectorLayer.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��12��23��
@ ����ժҪ	: ʸ������ͼ����
@ �޸ļ�¼	: 
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOVECTORLAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__
#define __GEOVECTORLAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__

#include "GeoLayer.h"

class GeoEnvelope;
class GeoFeature;
class GeoGeometry;

//ʸ��ͼ��
class GEOMETRY_API GeoVectorLayer : public GeoLayer
{
public:
	GeoVectorLayer(void);
	virtual ~GeoVectorLayer(void);

	//����һ��Ҫ�ؼ��뵽ͼ����
	virtual bool CreateFeature( GeoFeature *poFeature ) = 0;

	//���Ҫ��
	virtual GeoFeature *GetFeature( long nFID ) = 0;

	//��ü�����
	virtual GeoGeometry *GetGeometryRef(long nFID) = 0;

	//����Ҫ��
	virtual bool UpdateFeature( GeoFeature *poFeature ) = 0;

	//ɾ��һ��Ҫ��
	virtual bool DeleteFeature(int nIndex,bool bDelete) = 0;

	//���úͻ������ͼ�����С�������
	virtual bool GetEnvelope(GeoEnvelope* pEnv) = 0;
	virtual void SetEnvelope(GeoEnvelope* pEnv) = 0;

	//���ͼ�������
	virtual GeoWkbGeometryType GetGeometryType() const = 0;

	//���Ҫ�صĸ���
	virtual int GetFetureCount() const = 0;
};

#endif // end of __GEOVECTORLAYER_H_769AF0EF_CF0C_4FBF_8995_6A22281462EA__
