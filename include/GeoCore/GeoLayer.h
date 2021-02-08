/*******************************************************************************
* 版权所有(C) pyhcx 2012
* 文件名称	: GeoLayer.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年10月6日
* 内容摘要	: 图层类
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOLAYER_H_D549FDB0_42B6_44D8_BAE3_7784FB71F3EE__
#define __GEOLAYER_H_D549FDB0_42B6_44D8_BAE3_7784FB71F3EE__

#include <stdlib.h>
#include <string.h>
#include "GeoDefine.h"

class GeoEnvelope;
class GeoSpatialReference;


//图层类的抽象类
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
	char* m_pszLayerName;		//图层名字,路径
	char* m_pszDataSource;	//数据源

};


#endif // end of __GEOLAYER_H_D549FDB0_42B6_44D8_BAE3_7784FB71F3EE__
