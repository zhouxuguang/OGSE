/*******************************************************************************
* 版权所有(C) pyhcx 2012
* 文件名称	: GeoShapeDriver.h
* 当前版本	: 1.0.0.1
* 作    者	: 周旭光 (zhouxuguang236@126.com)
* 设计日期	: 2012年8月23日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __GEOSHAPEDRIVER_H_0A830A27_5A68_4962_B862_D31FEDE8F8E2__
#define __GEOSHAPEDRIVER_H_0A830A27_5A68_4962_B862_D31FEDE8F8E2__

/**********************************  头文件 ************************************/

#include <stdlib.h>
#include "GeoDataDriver.h"

class GeoDataSource;

class GeoShapeDriver : public GeoDataDriver
{
public:
	GeoShapeDriver(void);
	~GeoShapeDriver(void);

	virtual const char *GetName();

	virtual GeoDataSource *Open( const char *pszName, bool bUpdate=false );

	virtual GeoDataSource *CreateGeoDataSource( const char *pszName,
		char ** = NULL );
	virtual bool DeleteGeoDataSource( const char *pszName );

	virtual GeoDataSource *CopyGeoDataSource( GeoDataSource *poSrcDS, 
		const char *pszNewName, 
		char **papszOptions = NULL );

private:
	char* m_pszName;	//驱动的名字
};

#endif // end of __GEOSHAPEDRIVER_H_0A830A27_5A68_4962_B862_D31FEDE8F8E2__
