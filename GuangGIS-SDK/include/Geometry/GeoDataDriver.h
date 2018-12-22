/*******************************************************************************
@ 版权所有(C) 福建省空间信息工程研究中心 2013
@ 文件名称	: GeoDataDriver.h
@ 当前版本	: 1.0.0.1
@ 作    者	: guang (zhouxuguang236@126.com)
@ 设计日期	: 2013年11月23日
@ 内容摘要	: 数据驱动类
@ 修改记录	: 
@ 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEODATADRIVER_H_2D1EC449_8F69_4153_82E3_438DAD8EAAC6__
#define __GEODATADRIVER_H_2D1EC449_8F69_4153_82E3_438DAD8EAAC6__

#include <stdlib.h>
#include "GeoDefine.h"


class GeoDataSource;

//数据驱动
class GEOMETRY_API GeoDataDriver
{
public:
	GeoDataDriver(void);
	virtual ~GeoDataDriver();

	virtual const char *GetName() = 0;

	virtual GeoDataSource *Open( const char *pszName, bool bUpdate=false ) = 0;

	virtual GeoDataSource *CreateGeoDataSource( const char *pszName,
		char ** = NULL );
	virtual bool DeleteGeoDataSource( const char *pszName );

	virtual GeoDataSource *CopyGeoDataSource( GeoDataSource *poSrcDS, 
		const char *pszNewName, 
		char **papszOptions = NULL );
};

#endif // end of __GEODATADRIVER_H_2D1EC449_8F69_4153_82E3_438DAD8EAAC6__
