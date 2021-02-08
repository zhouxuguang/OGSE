/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: GeoMitabLayer.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年11月30日
@ 内容摘要	: mapinfo图层数据的读取
@ 修改记录	:
@ 日    期		版    本		修改人		修改摘要

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
	GeoSpatialReference *m_poSRS;				/* 延迟加载 */
	GeoFeatureDefn  *m_poFeatureDefn;			//要素定义
	GeoWkbGeometryType m_eGeomType;				//几何体类型
	int iNextShapeId;
	int m_nFeatureCount;						//总共图形个数

	char *m_pszFullName;						//图层的名字

	mitab_handle  m_hMitabHandle;			//mapinfo文件读写

	bool  m_bUpdateAccess;		//是否更新数据
	bool m_bSRSSet;				//SRS是否重新设置


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

	//获得图层的类型
	GeoWkbGeometryType GetGeometryType() const;

};

#endif // end of __GEOMITABLAYER_H_7A912BA7_25B9_4CA5_9050_9E59475FEABA__
