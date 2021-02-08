/*******************************************************************************
* ��Ȩ����(C) ����ʡ�ռ���Ϣ�����о����� 2012
* �ļ�����	: GeoShapeLayer.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ����� (zhouxuguang236@126.com)
* �������	: 2012��8��23��
* ����ժҪ	: shapefile���ݵĶ�д��ʹ��shapelib
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __GEOSHAPELAYER_H_9CD9BF1E_D6F4_40A5_B629_35B31B2C8D81__
#define __GEOSHAPELAYER_H_9CD9BF1E_D6F4_40A5_B629_35B31B2C8D81__

/**********************************  ͷ�ļ� ************************************/

//#include "GeoDefine.h"
#include "shapefil.h"
#include "GeoVectorLayer.h"

class GeoEnvelope;
class GeoDataSource;
class GeoFeatureDefn;
class GeoSpatialReference;
class GeoGeometry;
class GeoFieldDefn;

class GeoShapeLayer : public GeoVectorLayer
{
public:
	GeoShapeLayer( GeoDataSource* poDSIn,
		const char * pszName,
		SHPHandle hSHP, DBFHandle hDBF,
		GeoSpatialReference *poSRS, int bSRSSet,
		int bUpdate, 
		GeoWkbGeometryType eReqType );
	~GeoShapeLayer(void);

private:
	GeoDataSource  *m_poDS;
	GeoSpatialReference *m_poSRS;				/* �ӳټ��� */
	GeoFeatureDefn  *m_poFeatureDefn;			//Ҫ�ض���
	GeoWkbGeometryType m_eGeomType;				//����������
	int iNextShapeId;
	int m_nFeatureCount;						//�ܹ�ͼ�θ���

	char *m_pszFullName;						//ͼ�������

	SHPHandle  m_hSHP;			//shp�ļ���д
	DBFHandle  m_hDBF;			//dbf�ļ���д

	bool  m_bUpdateAccess;		//�Ƿ��������
	bool m_bSRSSet;				//SRS�Ƿ���������


public:
	bool CreateSpatialIndex( int nMaxDepth );
	bool DropSpatialIndex();
	bool RecomputeExtent();
	bool ResizeDBF();


	const char *GetFullName() { return m_pszFullName; }

	GeoGeometry *FetchShape(int iShapeId);

	void ResetReading();
	GeoFeature *GetNextFeature();

	//����һ���µ�Ҫ�ز����뵽�ļ���
	bool CreateFeature( GeoFeature *poFeature );
	GeoFeature *GetFeature( long nFID );
	GeoGeometry *GetGeometryRef(long nFID);
	bool UpdateFeature( GeoFeature *poFeature );	//����һ��Ҫ��
	bool DeleteFeature( int nIndex,bool bDelete );			//ɾ��һ��Ҫ��

	GeoFeatureDefn *GetLayerDefn() { return m_poFeatureDefn; }

	int GetFetureCount() const;
	bool GetEnvelope(GeoEnvelope* );
	void SetEnvelope(GeoEnvelope* pEnv);

	virtual bool  CreateField( GeoFieldDefn *poField, bool bApproxOK = true );

	virtual GeoSpatialReference *GetSpatialRef() const;

	//���ͼ�������
	GeoWkbGeometryType GetGeometryType() const;
};


#endif // end of __GEOSHAPELAYER_H_9CD9BF1E_D6F4_40A5_B629_35B31B2C8D81__
