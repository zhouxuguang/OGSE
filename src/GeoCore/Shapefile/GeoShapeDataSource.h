/*******************************************************************************
* 版权所有(C) pyhcx 2012
* 文件名称	: GeoShapeDataSource.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年8月24日
* 内容摘要	: shapefile数据源
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOSHAPEDATASOURCE_H_943A2524_F4EF_4AAE_9408_CA60D3E2555E__
#define __GEOSHAPEDATASOURCE_H_943A2524_F4EF_4AAE_9408_CA60D3E2555E__

/**********************************  头文件 ************************************/
#include <vector>
#include <string>
#include "GeoDataSource.h"

class GeoShapeLayer;

class GeoShapeDataSource : public GeoDataSource
{
public:
	GeoShapeDataSource(void);
	~GeoShapeDataSource(void);

	//打开文件
	virtual int Open( const char * pszFilename, int bUpdate = 0);

	//获得数据源名字
	virtual const char *GetName() { return m_pszName; };

	//获得图层个数
	virtual int GetLayerCount();

	//根据索引获得图层
	virtual GeoLayer *GetLayer(int);

	//根据名字获得图层
	virtual GeoLayer *GetLayerByName(const char *);

	//创建一个新的图层
	virtual GeoLayer *CreateGeoLayer( const char *pszName, 
		GeoSpatialReference *poSpatialRef = NULL,
		GeoWkbGeometryType eGType = gWkbUnKnow );

private:
	void  AddLayer(GeoShapeLayer* poLayer);

private:
	GeoShapeLayer     **m_papoLayers;		//shape图层
	int m_nLayers;						//图层数目
	
	char* m_pszName;						//数据源名称
	std::vector<std::string> m_vecLayerName;	//各个图层的名称

};

#endif // end of __GEOSHAPEDATASOURCE_H_943A2524_F4EF_4AAE_9408_CA60D3E2555E__
