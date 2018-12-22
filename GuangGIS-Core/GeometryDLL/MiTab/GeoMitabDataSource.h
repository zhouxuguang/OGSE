/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: GeoMitabDataSource.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年11月30日
@ 内容摘要	: mapinfo数据源
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

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

	//打开文件
	int Open( const char * pszFilename, int bUpdate = 0);

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
	void  AddLayer(GeoMitabLayer* poLayer);

private:
	GeoMitabLayer     **m_papoLayers;		//mapinfo图层
	int m_nLayers;						//图层数目

	char* m_pszName;						//数据源名称
	std::vector<std::string> m_vecLayerName;	//各个图层的名称
};

#endif // end of __GEOMITABDATASOURCE_H_BF43EBCC_F3E5_42E9_A0DB_52FEB2BDEC5F__
