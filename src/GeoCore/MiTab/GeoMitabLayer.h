/*******************************************************************************
@ ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2013
@ �ļ�����	: GeoMitabLayer.h
@ ��ǰ�汾	: 1.0.0.1
@ ��    ��	: guang (zhouxuguang236@126.com)
@ �������	: 2013��11��30��
@ ����ժҪ	: mapinfoͼ�����ݵĶ�ȡ
@ �޸ļ�¼	:
@ ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOMITABLAYER_H_7A912BA7_25B9_4CA5_9050_9E59475FEABA__
#define __GEOMITABLAYER_H_7A912BA7_25B9_4CA5_9050_9E59475FEABA__

//#include <io.h>
#include "mitab_capi.h"
#include "GeoVectorLayer.h"

class GeoEnvelope;
class GeoDataSource;
class GeoSpatialReference;
class GeoFeatureDefn;
class GeoFieldDefn;

class GeoMitabLayer : public GeoVectorLayer
{
public:
	GeoMitabLayer(GeoDataSource  *poDS,GeoSpatialReference *poSRS,const char* pszLayerName
		,mitab_handle hMitab,bool bUpdate);
	~GeoMitabLayer(void);

private:
	GeoDataSource  *m_poDS;
	GeoSpatialReference *m_poSRS;				/* �ӳټ��� */
	GeoFeatureDefn  *m_poFeatureDefn;			//Ҫ�ض���
	GeoWkbGeometryType m_eGeomType;				//����������
	int iNextShapeId;
	int m_nFeatureCount;						//�ܹ�ͼ�θ���

	char *m_pszFullName;						//ͼ�������

	mitab_handle  m_hMitabHandle;			//mapinfo�ļ���д

	bool  m_bUpdateAccess;		//�Ƿ��������
	bool m_bSRSSet;				//SRS�Ƿ���������


public:
	bool CreateSpatialIndex( int nMaxDepth );
	bool DropSpatialIndex();
	bool RecomputeExtent();
	bool ResizeDBF();


	const char *GetFullName() { return m_pszFullName; }

	GeoFeature *FetchShape(int iShapeId);

	void ResetReading();
	GeoFeature *GetNextFeature();

	GeoFeature *GetFeature( long nFID );
	GeoGeometry *GetGeometryRef(long nFID)
	{
		return NULL;
	}
	bool UpdateFeature( GeoFeature *poFeature );
	bool DeleteFeature( int nIndex,bool bDelete );			//?
	bool CreateFeature( GeoFeature *poFeature );

	GeoFeatureDefn *GetLayerDefn() { return m_poFeatureDefn; }

	int GetFetureCount() const;
	bool GetEnvelope(GeoEnvelope* );
	void SetEnvelope(GeoEnvelope* pEnv);

	virtual bool  CreateField( GeoFieldDefn *poField, bool bApproxOK = true );

	virtual GeoSpatialReference *GetSpatialRef() const;

	//���ͼ�������
	GeoWkbGeometryType GetGeometryType() const;

};

#endif // end of __GEOMITABLAYER_H_7A912BA7_25B9_4CA5_9050_9E59475FEABA__
