/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoShapeLayer.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年8月23日
* 内容摘要	: shapefile数据的读写，使用shapelib
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOSHAPELAYER_H_9CD9BF1E_D6F4_40A5_B629_35B31B2C8D81__
#define __GEOSHAPELAYER_H_9CD9BF1E_D6F4_40A5_B629_35B31B2C8D81__

/**********************************  头文件 ************************************/

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
	GeoSpatialReference *m_poSRS;				/* 延迟加载 */
	GeoFeatureDefn  *m_poFeatureDefn;			//要素定义
	GeoWkbGeometryType m_eGeomType;				//几何体类型
	int iNextShapeId;
	int m_nFeatureCount;						//总共图形个数

	char *m_pszFullName;						//图层的名字

	SHPHandle  m_hSHP;			//shp文件读写
	DBFHandle  m_hDBF;			//dbf文件读写

	bool  m_bUpdateAccess;		//是否更新数据
	bool m_bSRSSet;				//SRS是否重新设置


public:
	bool CreateSpatialIndex( int nMaxDepth );
	bool DropSpatialIndex();
	bool RecomputeExtent();
	bool ResizeDBF();


	const char *GetFullName() { return m_pszFullName; }

	GeoGeometry *FetchShape(int iShapeId);

	void ResetReading();
	GeoFeature *GetNextFeature();

	//创建一个新的要素并加入到文件中
	bool CreateFeature( GeoFeature *poFeature );
	GeoFeature *GetFeature( long nFID );
	GeoGeometry *GetGeometryRef(long nFID);
	bool UpdateFeature( GeoFeature *poFeature );	//更新一个要素
	bool DeleteFeature( int nIndex,bool bDelete );			//删除一个要素

	GeoFeatureDefn *GetLayerDefn() { return m_poFeatureDefn; }

	int GetFetureCount() const;
	bool GetEnvelope(GeoEnvelope* );
	void SetEnvelope(GeoEnvelope* pEnv);

	virtual bool  CreateField( GeoFieldDefn *poField, bool bApproxOK = true );

	virtual GeoSpatialReference *GetSpatialRef() const;

	//获得图层的类型
	GeoWkbGeometryType GetGeometryType() const;
};


#endif // end of __GEOSHAPELAYER_H_9CD9BF1E_D6F4_40A5_B629_35B31B2C8D81__
