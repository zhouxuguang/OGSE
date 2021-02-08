/*******************************************************************************
* 版权所有(C) 福建省空间信息工程研究中心 2012
* 文件名称	: GeoDataSource.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年10月7日
* 内容摘要	: 数据源
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEODATASOURCE_H_F0FECA57_C4E4_4999_A8D4_7D4C5233D71E__
#define __GEODATASOURCE_H_F0FECA57_C4E4_4999_A8D4_7D4C5233D71E__

/**********************************  头文件 ************************************/

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

	//销毁数据源
	static void DestroyGeoDataSource( GeoDataSource * );

	//获得数据源名字
	virtual const char *GetName() = 0;

	//获得图层个数
	virtual int GetLayerCount() = 0;

	//根据索引获得图层
	virtual GeoLayer *GetLayer(int) = 0;
	
	//根据名字获得图层
	virtual GeoLayer *GetLayerByName(const char *);

	//删除图层
	//virtual bool      DeleteLayer(int);

	//创建一个新的图层
	virtual GeoLayer *CreateGeoLayer( const char *pszName, 
		GeoSpatialReference *poSpatialRef = NULL,
		GeoWkbGeometryType eGType = gWkbUnKnow );

	GeoDataDriver *GetDriver() const;
	void SetDriver( GeoDataDriver *poDriver );

private:

	GeoDataDriver        *m_poDriver;		//数据驱动
};


//注册数据驱动
GEOMETRY_API void GEOMETRY_API_CALL RegisterShapefile();

GEOMETRY_API void GEOMETRY_API_CALL RegisterMapinfo();

#endif // end of __GEODATASOURCE_H_F0FECA57_C4E4_4999_A8D4_7D4C5233D71E__
